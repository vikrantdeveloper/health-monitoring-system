
/*
 * main.c
 *
 *  Created on: Jul 2, 2019
 *  Author: vikrant
 *  @open source project
 */
/*Standard files*/
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/*Header files*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "adc.h"
#include "gpio.h"
#include "uart.h"

ADC_HandleTypeDef hadc;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart4;
char res;

void SystemClock_Config(void);
int main(void)
{
	/* Initialise the HAL ,Systemclock ,GPIO,ADC, USART4, USART1, TIM6, TIM3, TIM7*/
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART4_UART_Init();
	MX_USART1_UART_Init();
	MX_TIM6_Init();
	MX_TIM3_Init();
	MX_TIM7_Init();
	MX_ADC_Init();
	HAL_Delay(100);

	uart_receive(&res);
	// Timer 6 with Enabling UART interrupt
	HAL_TIM_Base_Start_IT(&htim6);
	// Timer 3 Interrupt for capture the Input
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);

	while (1)
	{
	   // Convert sensors value using poll method
	   ADC_Conversion(0);
	   HAL_Delay(100);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /**Initializes the CPU, AHB and APB busses clocks*/
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14 |RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /**Initializes the CPU, AHB and APB busses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

   /**Configure the Systick interrupt time*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

   /**Configure the Systick*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

