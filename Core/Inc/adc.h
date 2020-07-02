/*
 * adc.h
 *
 *  Created on: Jul 2, 2020
 *  Author: vikrant
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

typedef enum{
	SENSOR_TEMPERATURE,
	SENSOR_ECG,
	STOP_ADC,
}sensor_adc;

typedef struct
{
	float body_temperature;
	uint32_t body_ecg;
	uint32_t body_heartbeat;
}sensor_t;

extern MX_ADC_Init(void);

/*
 * The API uses Poll method for conversion
 * First it will generate temperature , ECG and then stops the cycle continue
 */
extern void ADC_Conversion(sensor_adc status);

/*
 * integer to string conversion API
 */
extern void string_conversion(float n1, char *res, uint8_t afterpoint);

extern sensor_t sensor_value;

#endif /* INC_ADC_H_ */
