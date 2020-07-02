/*
 * gpio.h
 *
 *  Created on: Jul 2, 2020
 *      Author: vikrant
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#define TEMPERATURE_LED GPIO_PIN_8
#define ECG_LED  GPIO_PIN_9
#define HEART_BEAT GPIO_PIN_6

extern void GPIO_Blink(uint8_t num);
extern void MX_GPIO_Init(void);

#endif /* INC_GPIO_H_ */
