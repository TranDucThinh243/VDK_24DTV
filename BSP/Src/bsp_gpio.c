/* Includes ----------------------------------------------------------- */
#include "bsp_gpio.h"

/* Private function prototypes --------------------------------------- */
static void bsp_gpio_enable_clock(GPIO_TypeDef *gpio_port);

/* Private function definitions -------------------------------------- */
static void bsp_gpio_enable_clock(GPIO_TypeDef *gpio_port)
{
	if (gpio_port == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (gpio_port == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (gpio_port == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (gpio_port == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
#if defined(GPIOE)
	else if (gpio_port == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
#endif
#if defined(GPIOF)
	else if (gpio_port == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
#endif
#if defined(GPIOG)
	else if (gpio_port == GPIOG)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
#endif
}

/* Public function definitions ---------------------------------------- */
void bsp_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init = {0};

	bsp_gpio_enable_clock(BSP_GPIO_LED_PORT);
	bsp_gpio_enable_clock(BSP_GPIO_KEY1_PORT);

	HAL_GPIO_WritePin(BSP_GPIO_LED_PORT, BSP_GPIO_LED_PIN, GPIO_PIN_RESET);

	gpio_init.Pin = BSP_GPIO_KEY1_PIN | BSP_GPIO_KEY2_PIN | BSP_GPIO_KEY3_PIN;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(BSP_GPIO_KEY1_PORT, &gpio_init);

	gpio_init.Pin = BSP_GPIO_LED_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BSP_GPIO_LED_PORT, &gpio_init);
}

bsp_gpio_status_t bsp_gpio_config_input(GPIO_TypeDef *gpio_port,
																				uint16_t gpio_pin,
																				uint32_t pull)
{
	GPIO_InitTypeDef gpio_init = {0};

	if ((gpio_port == NULL) || (gpio_pin == 0U))
	{
		return BSP_GPIO_ERROR;
	}

	bsp_gpio_enable_clock(gpio_port);

	gpio_init.Pin = gpio_pin;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Pull = pull;

	HAL_GPIO_Init(gpio_port, &gpio_init);
	return BSP_GPIO_OK;
}

bsp_gpio_status_t bsp_gpio_config_output(GPIO_TypeDef *gpio_port,
																				 uint16_t gpio_pin,
																				 GPIO_PinState initial_state,
																				 uint32_t pull,
																				 uint32_t speed)
{
	GPIO_InitTypeDef gpio_init = {0};

	if ((gpio_port == NULL) || (gpio_pin == 0U))
	{
		return BSP_GPIO_ERROR;
	}

	bsp_gpio_enable_clock(gpio_port);
	HAL_GPIO_WritePin(gpio_port, gpio_pin, initial_state);

	gpio_init.Pin = gpio_pin;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = pull;
	gpio_init.Speed = speed;

	HAL_GPIO_Init(gpio_port, &gpio_init);
	return BSP_GPIO_OK;
}

bsp_gpio_status_t bsp_gpio_write(GPIO_TypeDef *gpio_port,
																 uint16_t gpio_pin,
																 GPIO_PinState pin_state)
{
	if ((gpio_port == NULL) || (gpio_pin == 0U))
	{
		return BSP_GPIO_ERROR;
	}

	HAL_GPIO_WritePin(gpio_port, gpio_pin, pin_state);
	return BSP_GPIO_OK;
}

bsp_gpio_status_t bsp_gpio_toggle(GPIO_TypeDef *gpio_port,
																	uint16_t gpio_pin)
{
	if ((gpio_port == NULL) || (gpio_pin == 0U))
	{
		return BSP_GPIO_ERROR;
	}

	HAL_GPIO_TogglePin(gpio_port, gpio_pin);
	return BSP_GPIO_OK;
}

GPIO_PinState bsp_gpio_read(GPIO_TypeDef *gpio_port,
														uint16_t gpio_pin)
{
	if ((gpio_port == NULL) || (gpio_pin == 0U))
	{
		return GPIO_PIN_RESET;
	}

	return HAL_GPIO_ReadPin(gpio_port, gpio_pin);
}
