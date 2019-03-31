/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#define TRAFFIC_LIGHTS_QTY 4

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

struct traffic_light {
	uint16_t r, y, g;
};

struct FSM {
	char *name;
	void (*handler)(void);
};

enum FSM_states {
	IDLE,
	TRAFFIC_LIGHT_G,
	TRAFFIC_LIGHT_Y
};

struct traffic_light lights[] = {
		{s0_r_Pin, s0_y_Pin, s0_g_Pin},
		{s1_r_Pin, s1_y_Pin, s1_g_Pin},
		{s2_r_Pin, s2_y_Pin, s2_g_Pin},
		{s3_r_Pin, s3_y_Pin, s3_g_Pin}
};

uint8_t curr_light;
enum FSM_states curr_state;

static void idle_handler(void) {
	HAL_GPIO_WritePin(GPIOA, 0xFFFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,
			lights[0].r |
			lights[1].r |
			lights[2].r |
			lights[3].r,
			GPIO_PIN_SET);

	HAL_Delay(1000);

	curr_state = TRAFFIC_LIGHT_G;
	curr_light = 0;
}

static void traffic_light_g(void) {
	HAL_GPIO_WritePin(GPIOA, lights[curr_light].r, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, lights[curr_light].g, GPIO_PIN_SET);

	HAL_Delay(10000);

	curr_state = TRAFFIC_LIGHT_Y;
}

static void traffic_light_y(void) {
	HAL_GPIO_WritePin(GPIOA, lights[curr_light].g, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, lights[curr_light].y, GPIO_PIN_SET);

	HAL_Delay(500);

	HAL_GPIO_WritePin(GPIOA, lights[curr_light].y, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, lights[curr_light].r, GPIO_PIN_SET);

	curr_state = TRAFFIC_LIGHT_G;

	curr_light++;
	if (curr_light > 3)
		curr_light = 0;
}

static struct FSM states[] = {
		{
				.name = "IDLE",
				.handler = idle_handler
		},
		{
				.name = "TRAFFIC_LIGHT_G",
				.handler = traffic_light_g
		},
		{
				.name = "TRAFFIC_LIGHT_Y",
				.handler = traffic_light_y
		}
};


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
  /* USER CODE BEGIN 2 */

  curr_light = 0;
  curr_state = IDLE;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  states[curr_state].handler();

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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, s0_r_Pin|s0_y_Pin|s0_g_Pin|s1_r_Pin 
                          |s1_y_Pin|s1_g_Pin|s2_r_Pin|s2_y_Pin 
                          |s2_g_Pin|s3_r_Pin|s3_y_Pin|s3_g_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : s0_r_Pin s0_y_Pin s0_g_Pin s1_r_Pin 
                           s1_y_Pin s1_g_Pin s2_r_Pin s2_y_Pin 
                           s2_g_Pin s3_r_Pin s3_y_Pin s3_g_Pin */
  GPIO_InitStruct.Pin = s0_r_Pin|s0_y_Pin|s0_g_Pin|s1_r_Pin 
                          |s1_y_Pin|s1_g_Pin|s2_r_Pin|s2_y_Pin 
                          |s2_g_Pin|s3_r_Pin|s3_y_Pin|s3_g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 check if the interrupt comes from TIM3
    if (htim->Instance==TIM2) {
    	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}*/
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
