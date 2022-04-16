#ifndef __NUCLEO476_LED_H
#define __NUCLEO476_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void GREEN_LED_Off(void);
void GREEN_LED_On(void);
void GREEN_LED_Toggle(void);

void RED_LED_Off(void);
void RED_LED_On(void);
void RED_LED_Toggle(void);

#endif /* __NUCLEO476_LED_H */
