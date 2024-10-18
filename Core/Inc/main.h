/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Blue_Led_Pin GPIO_PIN_13
#define Blue_Led_GPIO_Port GPIOC
#define Valve_Pin GPIO_PIN_4
#define Valve_GPIO_Port GPIOA
#define Pump_Pin GPIO_PIN_5
#define Pump_GPIO_Port GPIOA
#define Up_Pin GPIO_PIN_12
#define Up_GPIO_Port GPIOB
#define Down_Pin GPIO_PIN_13
#define Down_GPIO_Port GPIOB
#define Left_Pin GPIO_PIN_14
#define Left_GPIO_Port GPIOB
#define Right_Pin GPIO_PIN_15
#define Right_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

void getTimeFromMS(unsigned int*, int*);

void ApplyNewInterval(void);
void ApplyNewDuration(void);

void ResetNewInterval(void);
void ResetNewDuration(void);

void UpdateLastTime(void);
void UpdateNextTime(void);

void WakeUp(void);


// ENUM DEFINITION //

typedef enum {
	OFF = -1,
	READY = 0,
	INTERVAL = 1,
	DURATION = 2,
	LAST_TIME = 3,
	NEXT_TIME = 4
} Menu_Option;

typedef enum {
	MENU = 0,
	INTERVAL_SETTING = 1,
	DURATION_SETTING = 2

} Setting_Option;
// //

// MENU VARIABLES // //
extern const int MENU_COUNT;

extern char *menu_list[5][2];

extern Menu_Option *menu_option;
extern char str[11];
// //

// Logical variables for button state tracking
extern bool pressedUpButton, pressedDownButton, pressedLeftButton, pressedRightButton;

extern Setting_Option *setting_option;

// TIME TRACKING VARIABLES // // //
extern unsigned int Interval;
extern unsigned int NewInterval;
extern const unsigned int MaxInterval;

extern unsigned int Duration;
extern unsigned int NewDuration;
extern const unsigned int MaxDuration;

extern int timeBar[2];

extern unsigned int TimePassed;
extern unsigned int TimeLeft;
extern bool WasPoured;
extern bool IsPouring;
// // //

extern int DisplayShutDownTime;



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
