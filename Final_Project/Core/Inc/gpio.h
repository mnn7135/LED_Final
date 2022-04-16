/*
 * gpio.h
 *
 *  Created on: Nov 4, 2021
 *      Author: Mitesh Parikh
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

#define R1_Pin GPIO_PIN_9
#define R1_GPIO_Port GPIOA
#define G1_Pin GPIO_PIN_7
#define G1_GPIO_Port GPIOC

void GPIO_Init(void);

#endif /* INC_GPIO_H_ */
