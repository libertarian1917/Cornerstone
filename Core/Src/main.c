/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306_tests.h"
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void getTimeFromMS(uint32_t ms, int *clock) {
	int hours = ms / 3600000;
	int minutes = (ms % 3600000) / 60000;
	//int seconds = ((*ms % 3600000) % 60000) / 1000;
	clock[0] = hours;
	clock[1] = minutes;
	/*else if(hours == 0) {
		clock[0] = minutes;
		clock[1] = seconds;
	}*/
}

void SaveToFlash() {
    // Открываем доступ к флэш-памяти для записи
    HAL_FLASH_Unlock();

    // Стираем сектор 6 перед записью
    FLASH_Erase_Sector(FLASH_SECTOR_6, VOLTAGE_RANGE_3);

    // Writing Interval value to permanent memory
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Interval);

    // Writing Duration
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address + 4, Duration);

    // Writing how many times irrigation system has poured
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address + 8, TimesWasPoured);

    // Закрываем доступ к флэш-памяти после записи
    HAL_FLASH_Lock();
}

void LoadFromFlash() {
    // Reading of Interval value and saving in variable at address 0x08040000
    Interval = *(__IO uint32_t*)Address;
    NewInterval = Interval;
    TimeLeft = Interval;

    // Reading Duration from the address 0x08040004
    Duration = *(__IO uint32_t*)(Address + 4);
    NewDuration = Duration;

    // Reading TimesWasPoured at the address 0x08040008
    TimesWasPoured = *(__IO uint32_t*)(Address + 8);
}

void ApplyNewInterval() {
	Interval = NewInterval;

	TimePassed = 0;
	TimeLeft = NewInterval;
}

void ApplyNewDuration() {
	Duration = NewDuration;
}

void ResetNewInterval() {
	NewInterval = Interval;
}

void ResetNewDuration() {
	NewDuration = Duration;
}

void UpdateLastTime() {
	if(WasPoured == false) {
		menu_list[3][0] = "noch nicht";
		menu_list[3][1] = "verwendet";
	}
	else {
		getTimeFromMS(TimePassed, timeBar);
		sprintf(str, "vor %02d:%02d", timeBar[0], timeBar[1]);
		menu_list[3][0] = str;
		menu_list[3][1] = "verwendet";
	}
}

void UpdateNextTime() {
	getTimeFromMS(TimeLeft, timeBar);
	sprintf(str, "noch %02d:%02d", timeBar[0], timeBar[1]);
	menu_list[4][0] = str;
	//menu_list[4][1] = "zum Giessen";
}

void SetPouringCountInMenuList() {
	sprintf(str, "%d mal", TimesWasPoured);
	menu_list[5][0] = str;

}

void ResetPouringCount() {
	TimesWasPoured = 0;
}

void WakeUp() {
	DisplayShutDownTime = 0;
	if(menu_option == OFF) {
		menu_option = READY;
		setting_option = MENU;
		ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
	}
}

char *menu_list[6][2] = {
						{"Fertig", ""},           // 0
						{"Giess-", "Intervall"},  // 1
						{"Giess-", "Dauer"},      // 2
						{""},                     // 3
						{"", "zum Giessen"},      // 4
						{"", "gegossen"}          // 5
};

Menu_Option *menu_option = OFF;

char str[11] = "";

Setting_Option *setting_option = MENU;

const int MENU_COUNT = 5;

bool pressedUpButton = false, pressedDownButton = false, pressedLeftButton = false, pressedRightButton = false;

uint32_t Interval = 86400000; // 24 hours // 86,400,000 milliseconds //
uint32_t NewInterval = 86400000;
const uint32_t MaxInterval = 345600000; // 4 days // 345,600,000 milliseconds //

uint32_t Duration = 20000; // 20 seconds // 20,000 milliseconds //
uint32_t NewDuration = 20000;
const uint32_t MaxDuration = 900000; // 900 seconds // 900,000 milliseconds //

int timeBar[2] = {0, 0};

uint32_t TimesWasPoured = 0;
uint32_t Address = 0x08040000;

uint32_t TimePassed = 0;
uint32_t TimeLeft = 86400000; // default Interval value
int DisplayShutDownTime = 0;

bool WasPoured = false;
bool IsPouring = false;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init();
  LoadFromFlash();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(true)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // UP - BEGIN
	  HAL_Delay(100);
	  if(HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET)
      {
		  pressedUpButton = true;
		  continue;
      }
	  else if (pressedUpButton == true)
	  {
		  pressedUpButton = false;
		  DisplayShutDownTime = 0;
		  if(menu_option == OFF) {
			  WakeUp();
			  continue;
		  }

		  if(setting_option == MENU) {
			  if(menu_option > 0) {
				  menu_option --;
				  if(menu_option == 3) {
					  UpdateLastTime();
				  }
				  else if(menu_option == 4) {
					  UpdateNextTime();
				  }
				  else if(menu_option == 5) {
					  SetPouringCountInMenuList();
				  }
				  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
			  }
		  }
		  else if(setting_option == INTERVAL_SETTING) {
			  if(intervalSection == HOURS) {
				  if(NewInterval + 3600000 <= MaxInterval) {
					  NewInterval += 3600000;
				  }
			  }
			  else if(intervalSection == MINUTES) {
				  if(NewInterval + 60000 <= MaxInterval) {
					  NewInterval += 60000;
				  }
			  }
			  else if(intervalSection == SET_INTERVAL) {
				  // nothing
			  }
			  getTimeFromMS(NewInterval, timeBar);
			  ssd1306_TestIntervalSetting(timeBar, NewInterval);
		  }
		  else if(setting_option == DURATION_SETTING) {
			  if(durationSection == SECONDS) {
				  if(NewDuration + 1000 <= MaxDuration) {
					  NewDuration += 1000;
				  }
			  }
			  else if(durationSection == SET_DURATION) {
				  // nothing
			  }

			  ssd1306_TestDurationSetting(NewDuration);
		  }
		  else if(setting_option == TIMES_POURED_ZEROING) {
			  ////////////////
		  }

		  continue;
	  }
	  // UP - END

	  // DOWN - BEGIN
	  if(HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET)
      {
		  pressedDownButton = true;
		  continue;
      }
	  else if (pressedDownButton == true)
	  {
		  pressedDownButton = false;
		  DisplayShutDownTime = 0;
		  if(menu_option == OFF) {
			  WakeUp();
			  continue;
		  }

		  if(setting_option == MENU) {
			  if(menu_option < MENU_COUNT) {
				  menu_option ++;

				  if(menu_option == 3) {
					  UpdateLastTime();
				  }
				  else if(menu_option == 4) {
					  UpdateNextTime();
				  }
				  else if(menu_option == 5) {
					  SetPouringCountInMenuList();
				  }
				  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
			  }
		  }
		  else if(setting_option == INTERVAL_SETTING) {
			  if(intervalSection == HOURS) {
				  if(NewInterval >= 3600000) {
					  NewInterval -= 3600000;
				  }
			  }
			  else if(intervalSection == MINUTES) {
				  if(NewInterval >= 60000) {
					  NewInterval -= 60000;
				  }
			  }
			  else if(intervalSection == SET_INTERVAL) {
				  // nothing
			  }
			  getTimeFromMS(NewInterval, timeBar);
			  ssd1306_TestIntervalSetting(timeBar, NewInterval);
		  }
		  else if(setting_option == DURATION_SETTING) {
			  if(durationSection == SECONDS) {
				  if(NewDuration >= 1000) {
					  NewDuration -= 1000;
				  }
			  }
			  else if(durationSection == SET_DURATION) {
				  // nothing
			  }

			  ssd1306_TestDurationSetting(NewDuration);
		  }
		  else if(setting_option == TIMES_POURED_ZEROING) {
			  /////////////////
		  }

		  continue;
	  }
	  // DOWN - END

	  // LEFT (BACK) - BEGIN
	  if(HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
      {
		  pressedLeftButton = true;
		  continue;
      }
	  else if (pressedLeftButton == true)
	  {
		  pressedLeftButton = false;
		  DisplayShutDownTime = 0;
		  if(menu_option == OFF) {
			  WakeUp();
			  continue;
		  }

		  if(setting_option == MENU) {
			  // nothing
		  }
		  else if(setting_option == INTERVAL_SETTING) {
			  if(intervalSection > 0) {
				  intervalSection--;
				  ssd1306_TestIntervalSetting(timeBar, NewInterval);
			  }
			  else {
				  ResetNewInterval();
				  intervalSection = HOURS;
				  setting_option = MENU;
				  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
			  }
		  }
		  else if(setting_option == DURATION_SETTING) {
			  if(durationSection > 0) {
				  durationSection--;
				  ssd1306_TestDurationSetting(NewDuration);
			  }
			  else {
				  ResetNewDuration();
				  durationSection = SECONDS;
				  setting_option = MENU;
				  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
			  }
		  }
		  else if(setting_option == TIMES_POURED_ZEROING) {
			  setting_option = MENU;
			  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
		  }

		  continue;
	  }
	  // LEFT (BACK) - END

	  // RIGHT (OKAY) - BEGIN
	  if(HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET)
      {
		  pressedRightButton = true;
		  continue;
      }
	  else if (pressedRightButton == true)
	  {
		  pressedRightButton = false;
		  DisplayShutDownTime = 0;
		  if(menu_option == OFF) {
			  WakeUp();
			  continue;
		  }

		  if(setting_option == MENU) {

			  if(menu_option == READY) {
				  menu_option = OFF;
				  ssd1306_TurnOff();
			  }
			  else if(menu_option == INTERVAL) {
				  setting_option = INTERVAL_SETTING;
				  getTimeFromMS(NewInterval, timeBar);
				  ssd1306_TestIntervalSetting(timeBar, NewInterval);
			  }
			  else if(menu_option == DURATION) {
				  setting_option = DURATION_SETTING;
				  ssd1306_TestDurationSetting(NewDuration);
			  }
			  else if(menu_option == LAST_TIME) {

			  }
			  else if(menu_option == NEXT_TIME) {

			  }
			  else if(menu_option == TIMES_POURED) {
				  setting_option = TIMES_POURED_ZEROING;
				  ssd1306_TestZeroingTimes();
			  }
		  }
		  else if(setting_option == INTERVAL_SETTING) {
			  if(intervalSection < 2) {
				  intervalSection++;
				  ssd1306_TestIntervalSetting(timeBar, NewInterval);
			  }
			  else {
				  ApplyNewInterval();
				  SaveToFlash();
				  intervalSection = HOURS;
				  setting_option = MENU;
				  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
			  }
		  }
		  else if(setting_option == DURATION_SETTING) {
			  if(durationSection < 1) {
				  durationSection++;
				  ssd1306_TestDurationSetting(NewDuration);
			  }
			  else {
				  ApplyNewDuration();
				  SaveToFlash();
				  durationSection = SECONDS;
				  setting_option = MENU;
				  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
			  }
		  }
		  else if(setting_option == TIMES_POURED_ZEROING) {
			  ResetPouringCount();
			  SetPouringCountInMenuList();
			  setting_option = MENU;
			  ssd1306_TestMenu(menu_list[(int)menu_option][0], menu_list[(int)menu_option][1], menu_option);
		  }

		  continue;
	  }
	  // RIGHT (OKAY) - END


	  // TIME COUNT //
	 //     \/     //

	  if(IsPouring) {
		  // PA4 - VALVE // PA5 - PUMP //
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		  HAL_Delay(500);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_Delay(Duration);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

		  TimeLeft = Interval;
		  TimePassed = 0;
		  WasPoured = true;
		  TimesWasPoured++;
		  SaveToFlash();
		  IsPouring = false;
	  }

	  if(menu_option == OFF) {
		  TimeLeft -= 100;
		  TimePassed += 100;

		  if(TimeLeft == 0) {
			  IsPouring = true;
		  }
	  }

	  if(DisplayShutDownTime == 199) {
		  menu_option = OFF;
		  ssd1306_TurnOff();
	  }
	  if(DisplayShutDownTime < 200 && setting_option == MENU) {
		  DisplayShutDownTime++;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Blue_Led_GPIO_Port, Blue_Led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Valve_Pin|Pump_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Blue_Led_Pin */
  GPIO_InitStruct.Pin = Blue_Led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Blue_Led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Valve_Pin Pump_Pin */
  GPIO_InitStruct.Pin = Valve_Pin|Pump_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Up_Pin Down_Pin Left_Pin Right_Pin */
  GPIO_InitStruct.Pin = Up_Pin|Down_Pin|Left_Pin|Right_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
