/*
 * uart.c
 *
 *  Created on: Jul 2, 2020
 *  Author: vikrant
 */
#include "main.h"
#include "uart.h"
#include "adc.h"

void error_handler(char *log, UART_HandleTypeDef *huart4, uint16_t size)
{
	HAL_UART_Transmit(&huart4,(uint8_t *)log,size,HAL_MAX_DELAY);
}
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if(huart->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }

}
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
  }
  else if(huart->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
  }

}
void MX_USART1_UART_Init(void)
{

  char *log = "UART_1_ERROR";
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    error_handler(log, huart4);
  }

}
void MX_USART4_UART_Init(void)
{
  huart4.Instance = USART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
    {
      error_handler(log, huart4);
    }
}

// USART AS Interrupt //
/*
 * The API is triggered when the data is received on the UART PORT
 * Send the convert analog data of the sensor
 * */
void USART3_4_IRQHandler()
{

     HAL_UART_IRQHandler(&huart4);
     char string_ecg[255];
     if(rece[0]=='1')
     {
    	 string_conversion(sensor_value.body_temperature, &string_ecg[0] , 5);
     	 HAL_UART_Transmit(&huart4,string_ecg, strlen(string_ecg),HAL_MAX_DELAY);

     }
     else if(rece[0]=='2')
     {
    	 if(sensor_value.body_ecg == 0)
    	 {
    		 string_ecg[1]="0";
    		 HAL_UART_Transmit(&huart4, string_ecg, strlen(string_ecg), HAL_MAX_DELAY);
    	 }
    	 else
    	 {
    		 string_conversion(sensor_value.body_ecg, &string_ecg[0] , 5);
    		 HAL_UART_Transmit(&huart4, string_ecg, strlen(string_ecg), HAL_MAX_DELAY);
    	 }
        }
     else if(rece[0]=='3')
     {
    	 	string_conversion(sensor_value.body_heartbeat, &string_ecg[0] , 5);
    	    HAL_UART_Transmit(&huart4, string_ecg, sizeof(string_ecg),HAL_MAX_DELAY);
     }
}
/*
 * receive the buffer from console
 * @param :- 0 - send data of temperature sensor
 *           1 - send data of ECG sensor
 *           2-  send data of heartbeat
 */
void uart_receive(uint8_t *res)
{
	HAL_UART_Receive_IT(&huart4, res, sizeof(res));
}
