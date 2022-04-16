/*
 * gpio.c
 *
 *  Created on: Nov 4, 2021
 *      Author: Mitesh Parikh
 */


/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include <stdbool.h>
#include "stm32l4xx.h"

// External Global Variables that we will need access to


/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = R1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(R1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = G1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(G1_GPIO_Port, &GPIO_InitStruct);

	/* EXTI15_10_IRQn interrupt init*/
	// Note you will have to add EXTI15_10_IRQn Interrupt handler function as well
	// This is the interrupt handler for the blue button
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* EXTI9_5_IRQn interrupt init*/
	// Note you will have to add EXTI9_5_IRQn Interrupt handler function as well
	// This is the interrupt handler for the "Red" and "Green" buttons (R1 and G1)
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/*
 * This function is responsible for handling an interrupt from the blue
 * button on the board. It invokes the GPIO handler function with the
 * blue button pin.
 */
void EXTI15_10_IRQHandler() {
	HAL_GPIO_EXTI_IRQHandler(B1_Pin);
}

/*
 * This function is responsible for handling an interrupt from either
 * the Green LED button or Red LED button. It invokes the GPIO handler
 * function with the correct pin that has been activated.
 */
void EXTI9_5_IRQHandler() {
	if(__HAL_GPIO_EXTI_GET_FLAG(G1_Pin)) {
		HAL_GPIO_EXTI_IRQHandler(G1_Pin);
	}
	if(__HAL_GPIO_EXTI_GET_FLAG(R1_Pin)) {
		HAL_GPIO_EXTI_IRQHandler(R1_Pin);
	}
}
