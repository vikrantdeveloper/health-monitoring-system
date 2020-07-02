/*
 *  adc.c
 *  Created on: Jul 2, 2019
 *  Author: Vikrant
 */
#include <math.h>
#include "stm32f0xx_hal_adc.h"
#include "main.h"
#include "gpio.h"

sensor_t sensor_value;
static void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC1)
  {
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC GPIO Configuration
    PA4     ------> ADC_IN4 Temperature sensor
    PA5     ------> ADC_IN5 ECG sensor
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}
static void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC GPIO Configuration
    PA4     ------> ADC_IN4
    PA5     ------> ADC_IN5
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);
  }

}

static void adc_init(void)
{

  char *log = "ADC_ERROR";
  ADC_ChannelConfTypeDef sConfig;
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = ENABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    error_handler(log, &huart4, sizeof(log));
  }
  else
  {
	  HAL_ADC_MspInit(hadc);
  }
  /**Configure for the selected ADC regular channel to be converted.*/
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
	  error_handler(log, &huart4, sizeof(log));
  }

}
void MX_ADC_Init(void)
{
	// initialise the ADC
	adc_init();
	// Calibrate the ADC
	HAL_ADCEx_Calibration_Start(&hadc);
}

/* START OF INT TO STRING */
static void reverse(char *str, int len)
  {
      int i=0, j=len-1, temp;
      while (i<j)
      {
          temp = str[i];
          str[i] = str[j];
          str[j] = temp;
          i++; j--;
      }
  }
static int intToStr(int x, char str[], int d)
{
      int i = 0;
      while (x)
      {
          str[i++] = (x%10) + '0';
          x = x/10;
      }
      while (i < d)
          str[i++] = '0';
      reverse(str, i);
      str[i] = '\0';
      return i;
}
void string_conversion(float n1, char * res, uint8_t afterpoint)
{
        int ipart = (int)n1;
        float fpart = n1 - (float)ipart;
        int i = intToStr(ipart, res, 0);
        if (afterpoint != 0)
        {
            res[i] = '.';
            fpart = fpart * pow(10, afterpoint);
            intToStr((int)fpart, res + i + 1, afterpoint);
        }
}
/*	END OF INT TO STRING */

static void calcuate_temperature(uint32_t *temp5)
{
	sensor_value.body_temperature = 3.3 * (*temp5)  * 100 / 4095;
	GPIO_Blink(TEMPERATURE_LED);

}
static void heartecg(uint32_t ecg)
{
	 if((HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==1) && (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==1))
	 {
		 sensor_value.body_ecg = 0;   // No value
		 char *string_ecg = (char *)malloc(1* sizeof(char));
		 string_conversion(ecg, &string_ecg[0] , 1);
	 }
	 else
	 {
		 sensor_value.body_ecg = ecg;
		 char *string_ecg = (char *)malloc(8* sizeof(char));
		 string_conversion(ecg, string_ecg, 5);
	 }
 }
void ADC_Conversion(sensor_adc status)
{
	uint32_t adc_get_value = 0;
	char *log = "WRONG_CONVERSION";
	switch(status)
	{
	case SENSOR_TEMPERATURE:

		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 100);
		adc_get_value = HAL_ADC_GetValue(&hadc);
		calculate_temperature(&adc_get_value);
		status = SENSOR_ECG;
		break;

	case SENSOR_ECG:

		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 100);
		adc_get_value = HAL_ADC_GetValue(&hadc);
		heartecg(adc_get_value);
		status = STOP_ADC;
		break;

	case STOP_ADC:

		HAL_Delay(100);
		HAL_ADC_Stop(&hadc);
		break;

	default:

		error_handler(log, &huart4, sizeof(log));
		break;
	}
}


