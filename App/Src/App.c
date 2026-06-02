/**
 * @file App.c
 * @brief Core Application Logic for HC-05 Bluetooth and OLED features
 */

#include "App.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "bsp_adc.h"
#include "bsp_pwm.h"
#include "bsp_uart.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* External handles defined in main.c */
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;

/* UART Circular (Ring) Buffer for HC-05 RX */
#define RX_BUFFER_SIZE 256
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;
static volatile uint8_t rx_byte = 0; // Temp holder for HAL UART IT read

/* App Telemetry and State variables */
static uint32_t last_telemetry_tick = 0;
static uint32_t last_display_tick = 0;
static uint32_t last_btn_tick = 0;

static char last_command[32] = "None";
static char display_msg[24] = "Waiting for BT...";
static bool led_state = false;
static int pwm2_duty = 0;
static int pwm3_duty = 0;
static int pwm4_duty = 0;
static uint32_t adc_val = 0;

static bool b12_prev = true;
static bool b13_prev = true;
static bool b14_prev = true;
static uint32_t animation_counter = 0;

/* --- Private helper function declarations --- */
static void rx_buffer_push(uint8_t data);
static bool rx_buffer_pop(uint8_t *data);
static void bt_send_string(const char *str);
static void parse_command(const char *cmd);
static void draw_bluetooth_splash(void);
static void update_oled_dashboard(void);

/* --- Public Application Interface --- */

/**
 * @brief Initializes application peripherals, OLED display, and UART interrupts
 */
void App_Init(void) {
    // 1. Initialize OLED screen
    ssd1306_Init();
    
    // 2. Render gorgeous splash screen
    draw_bluetooth_splash();
    bsp_delay_ms(1500); // Display splash screen for 1.5 seconds
    
    // 3. Start PWM channels (TIM2 CH2 on PA1, TIM2 CH3 on PA2, TIM2 CH4 on PA3)
    bsp_pwm_start(&htim2, TIM_CHANNEL_2);
    bsp_pwm_start(&htim2, TIM_CHANNEL_3);
    bsp_pwm_start(&htim2, TIM_CHANNEL_4);
    
    bsp_pwm_set_duty(&htim2, TIM_CHANNEL_2, 0.0f);
    bsp_pwm_set_duty(&htim2, TIM_CHANNEL_3, 0.0f);
    bsp_pwm_set_duty(&htim2, TIM_CHANNEL_4, 0.0f);

    // 4. Initial state reading for buttons
    b12_prev = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_12) == BSP_GPIO_PIN_SET);
    b13_prev = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_13) == BSP_GPIO_PIN_SET);
    b14_prev = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_14) == BSP_GPIO_PIN_SET);

    // 5. Fire initial UART receiver interrupt
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&rx_byte, 1);
    
    bt_send_string("SYSTEM READY. VDK_24DTV HC-05 ONLINE.\r\n");
}

/**
 * @brief Infinite loop handler called from main while(1)
 */
void App_Loop(void) {
    uint32_t current_tick = HAL_GetTick();

    // --- 1. Serial Incoming Packets Assembly & Processing ---
    static char cmd_line[64];
    static uint16_t cmd_idx = 0;
    uint8_t c;

    while (rx_buffer_pop(&c)) {
        if (c == '\r' || c == '\n') {
            if (cmd_idx > 0) {
                cmd_line[cmd_idx] = '\0';
                parse_command(cmd_line);
                cmd_idx = 0; // Reset for next command
            }
        } else if (cmd_idx < sizeof(cmd_line) - 1) {
            cmd_line[cmd_idx++] = c;
        }
    }

    // --- 2. Button Input Monitoring with debouncing (20ms) ---
    if (current_tick - last_btn_tick >= 20) {
        last_btn_tick = current_tick;
        
        bool b12 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_12) == BSP_GPIO_PIN_SET);
        bool b13 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_13) == BSP_GPIO_PIN_SET);
        bool b14 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_14) == BSP_GPIO_PIN_SET);

        if (b12 != b12_prev) {
            b12_prev = b12;
            char msg[32];
            sprintf(msg, "BTN:12:%d\r\n", b12 ? 1 : 0);
            bt_send_string(msg);
        }
        if (b13 != b13_prev) {
            b13_prev = b13;
            char msg[32];
            sprintf(msg, "BTN:13:%d\r\n", b13 ? 1 : 0);
            bt_send_string(msg);
        }
        if (b14 != b14_prev) {
            b14_prev = b14;
            char msg[32];
            sprintf(msg, "BTN:14:%d\r\n", b14 ? 1 : 0);
            bt_send_string(msg);
        }
    }

    // --- 3. Telemetry Periodic Broadcast (1000ms) ---
    if (current_tick - last_telemetry_tick >= 1000) {
        last_telemetry_tick = current_tick;
        
        // Read ADC Channel 0 (PA0)
        uint32_t val = 0;
        if (bsp_adc_read_channel_blocking(&hadc1, &val, 10) == BSP_ADC_OK) {
            adc_val = val;
        }

        // Send POT telemetry
        char msg[32];
        sprintf(msg, "POT:%lu\r\n", adc_val);
        bt_send_string(msg);
    }

    // --- 4. OLED Dashboard Refresh (100ms) ---
    if (current_tick - last_display_tick >= 100) {
        last_display_tick = current_tick;
        animation_counter++;
        
        // Periodic ADC Update for rendering
        uint32_t val = 0;
        if (bsp_adc_read_channel_blocking(&hadc1, &val, 5) == BSP_ADC_OK) {
            adc_val = val;
        }
        
        update_oled_dashboard();
    }
}

/* --- Interrupt Vector Overrides --- */

/**
 * @brief HAL UART RX Complete Callback override to queue bytes non-blockingly
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        rx_buffer_push(rx_byte);
        // Restart UART RX interrupt immediately
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&rx_byte, 1);
    }
}

/* --- Private helper function implementations --- */

/**
 * @brief Thread-safe push of a byte to the UART ring buffer
 */
static void rx_buffer_push(uint8_t data) {
    uint16_t next = (rx_head + 1) % RX_BUFFER_SIZE;
    if (next != rx_tail) {
        rx_buffer[rx_head] = data;
        rx_head = next;
    }
}

/**
 * @brief Thread-safe pop of a byte from the UART ring buffer
 */
static bool rx_buffer_pop(uint8_t *data) {
    if (rx_head == rx_tail) {
        return false;
    }
    *data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    return true;
}

/**
 * @brief Transmission wrapper to send strings blockingly back to Bluetooth master
 */
static void bt_send_string(const char *str) {
    bsp_uart_transmit(&huart1, (uint8_t*)str, strlen(str), 100);
}

/**
 * @brief Simple custom protocol command parser and dispatcher
 */
static void parse_command(const char *cmd) {
    // 1. Record command in last received command ticker
    strncpy(last_command, cmd, sizeof(last_command) - 1);
    last_command[sizeof(last_command) - 1] = '\0';

    // 2. Command: LED:1 or LED:0
    if (strncmp(cmd, "LED:1", 5) == 0) {
        led_state = true;
        bsp_gpio_pin_write(GPIOA, GPIO_PIN_8, BSP_GPIO_PIN_SET);
        bt_send_string("ACK:LED:1\r\n");
    } 
    else if (strncmp(cmd, "LED:0", 5) == 0) {
        led_state = false;
        bsp_gpio_pin_write(GPIOA, GPIO_PIN_8, BSP_GPIO_PIN_RESET);
        bt_send_string("ACK:LED:0\r\n");
    }
    // 3. Command: PWM:<ch>:<val> e.g. PWM:2:65
    else if (strncmp(cmd, "PWM:", 4) == 0) {
        int ch = 0;
        int val = 0;
        if (sscanf(cmd, "PWM:%d:%d", &ch, &val) == 2) {
            if (val >= 0 && val <= 100) {
                if (ch == 2) {
                    pwm2_duty = val;
                    bsp_pwm_set_duty(&htim2, TIM_CHANNEL_2, (float)val);
                    bt_send_string("ACK:PWM:2:OK\r\n");
                } else if (ch == 3) {
                    pwm3_duty = val;
                    bsp_pwm_set_duty(&htim2, TIM_CHANNEL_3, (float)val);
                    bt_send_string("ACK:PWM:3:OK\r\n");
                } else if (ch == 4) {
                    pwm4_duty = val;
                    bsp_pwm_set_duty(&htim2, TIM_CHANNEL_4, (float)val);
                    bt_send_string("ACK:PWM:4:OK\r\n");
                } else {
                    bt_send_string("ERR:INVALID_PWM_CHANNEL\r\n");
                }
            } else {
                bt_send_string("ERR:PWM_DUTY_OUT_OF_RANGE\r\n");
            }
        } else {
            bt_send_string("ERR:PWM_FORMAT_ERR\r\n");
        }
    }
    // 4. Command: GET:ADC
    else if (strcmp(cmd, "GET:ADC") == 0) {
        char reply[32];
        sprintf(reply, "POT:%lu\r\n", adc_val);
        bt_send_string(reply);
    }
    // 5. Command: GET:STATUS
    else if (strcmp(cmd, "GET:STATUS") == 0) {
        char reply[128];
        bool b12 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_12) == BSP_GPIO_PIN_SET);
        bool b13 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_13) == BSP_GPIO_PIN_SET);
        bool b14 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_14) == BSP_GPIO_PIN_SET);
        
        sprintf(reply, "LED:%d,PWM2:%d,PWM3:%d,PWM4:%d,ADC:%lu,B12:%d,B13:%d,B14:%d\r\n",
                led_state ? 1 : 0, pwm2_duty, pwm3_duty, pwm4_duty, adc_val,
                b12 ? 1 : 0, b13 ? 1 : 0, b14 ? 1 : 0);
        bt_send_string(reply);
    }
    // 6. Command: DISP:<msg>
    else if (strncmp(cmd, "DISP:", 5) == 0) {
        strncpy(display_msg, cmd + 5, sizeof(display_msg) - 1);
        display_msg[sizeof(display_msg) - 1] = '\0';
        bt_send_string("ACK:DISP:OK\r\n");
    }
    // 7. Unknown command
    else {
        bt_send_string("ERR:UNKNOWN_COMMAND\r\n");
    }
}

/**
 * @brief Renders a stylized Bluetooth logo and welcome screen on the SSD1306 OLED
 */
static void draw_bluetooth_splash(void) {
    ssd1306_Fill(Black);
    
    // Coordinate definitions for symmetric BT logo
    // E(54, 22), D(74, 42), F(54, 42), C(74, 22), A(64, 12), B(64, 52)
    ssd1306_Line(54, 22, 74, 42, White);
    ssd1306_Line(54, 42, 74, 22, White);
    ssd1306_Line(74, 22, 64, 12, White);
    ssd1306_Line(64, 12, 64, 52, White);
    ssd1306_Line(64, 52, 74, 42, White);
    
    ssd1306_SetCursor(12, 54);
    ssd1306_WriteString((char*)"ANTIGRAVITY BT READY", Font_6x8, White);
    
    ssd1306_UpdateScreen();
}

/**
 * @brief Updates the OLED screen with a high-fidelity dashboard containing active telemetry
 */
static void update_oled_dashboard(void) {
    ssd1306_Fill(Black);

    // 1. Centered Header: [ HC-05 BT SYSTEM ]
    ssd1306_SetCursor(14, 0);
    ssd1306_WriteString((char*)"[ HC-05 BT SYSTEM ]", Font_6x8, White);
    ssd1306_Line(0, 11, 127, 11, White); // Header dividing line

    // 2. Rotating Dash Heartbeat Micro-animation in header corner
    char anim[] = {'|', '/', '-', '\\'};
    char anim_str[2] = { anim[(animation_counter) % 4], '\0' };
    ssd1306_SetCursor(120, 0);
    ssd1306_WriteString(anim_str, Font_6x8, White);

    // 3. Column 1: System outputs (LED, PWM levels)
    char buffer[24];
    
    // LED State
    ssd1306_SetCursor(0, 15);
    if (led_state) {
        ssd1306_WriteString((char*)"LED: [ON ]", Font_6x8, White);
    } else {
        ssd1306_WriteString((char*)"LED: [OFF]", Font_6x8, White);
    }

    // PWM Levels
    sprintf(buffer, "PWM2: %02d%%", pwm2_duty);
    ssd1306_SetCursor(0, 25);
    ssd1306_WriteString(buffer, Font_6x8, White);

    sprintf(buffer, "PWM3: %02d%%", pwm3_duty);
    ssd1306_SetCursor(0, 35);
    ssd1306_WriteString(buffer, Font_6x8, White);

    // Button status summary PB12, PB13, PB14
    bool b12 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_12) == BSP_GPIO_PIN_SET);
    bool b13 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_13) == BSP_GPIO_PIN_SET);
    bool b14 = (bsp_gpio_pin_read(GPIOB, GPIO_PIN_14) == BSP_GPIO_PIN_SET);
    sprintf(buffer, "BTNS:%c %c %c", b12 ? 'H' : 'L', b13 ? 'H' : 'L', b14 ? 'H' : 'L');
    ssd1306_SetCursor(0, 45);
    ssd1306_WriteString(buffer, Font_6x8, White);

    // 4. Column 2: Analog Sensor levels (ADC PA0 potentiometer)
    sprintf(buffer, "POT: %04lu", adc_val);
    ssd1306_SetCursor(66, 15);
    ssd1306_WriteString(buffer, Font_6x8, White);

    // Analog Progress Bar (60x6 pixels) at (66, 25)
    ssd1306_DrawRectangle(66, 25, 126, 31, White);
    // Fill the progress bar corresponding to the 12-bit ADC value (0 to 4095)
    uint8_t fill_width = (uint8_t)((adc_val * 58) / 4095);
    if (fill_width > 58) fill_width = 58;
    ssd1306_FillRectangle(67, 26, 67 + fill_width, 30, White);

    // Display message or info box
    sprintf(buffer, "M3: %02d%%", pwm4_duty);
    ssd1306_SetCursor(66, 35);
    ssd1306_WriteString(buffer, Font_6x8, White);

    // 5. Footer: Custom Bluetooth Message / Last command
    ssd1306_Line(0, 54, 127, 54, White); // Footer dividing line
    
    // Determine what to display in the live ticker box
    char ticker[32];
    if (strcmp(last_command, "None") != 0) {
        sprintf(ticker, "Rx: %s", last_command);
    } else {
        sprintf(ticker, "%s", display_msg);
    }
    
    ssd1306_SetCursor(0, 56);
    ssd1306_WriteString(ticker, Font_6x8, White);

    // Draw active indicator frame if screen updates
    ssd1306_UpdateScreen();
}
