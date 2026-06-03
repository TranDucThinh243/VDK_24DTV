/**
 * @file app_main.h
 * @brief Application Layer Main Logic Header
 * @version 1.0.0
 */

#ifndef APP_MAIN_H
#define APP_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the application and all underlying services.
 *        Called once in main() before the while(1) loop.
 */
void App_Init(void);

/**
 * @brief The main application loop.
 *        Called continuously in main() inside the while(1) loop.
 */
void App_Loop(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_MAIN_H */
