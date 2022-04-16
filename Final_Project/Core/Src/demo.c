#include <stdio.h>
#include <string.h>

#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "stm32l4xx.h"
#include "systick.h"
#include "helper.h"
#include "gpio.h"
#include "stm32l4xx_hal_gpio.h"

static volatile uint8_t one_second_elapsed;
static volatile uint8_t half_second_elapsed;
static volatile uint8_t three_times_flash;
static int remoteMode = 1;
static unsigned char *remote = (unsigned char*) "***REMOTE MODE ACTIVE***\r\n\0";
static unsigned char *local = (unsigned char*) "***MANUAL OVERRIDE MODE ACTIVE***\r\n\0";

/*
 * This function sets up all the registers with correct bit values
 * to enable the necessary interrupt handlers. It also outputs
 * the initial help information for the project.
 */
static void init()
{
	// SysTick Interrupt Setup
	systick_registers_t *systick = (systick_registers_t*) 0xE000E010;
	systick->CSR &= ~0x7;
	systick->RVR &= ~0xFFFFFF;
	systick->RVR |= 0x1387F;
	systick->CSR = (systick->CSR | 1<<2);
	systick->CSR = (systick->CSR | 1<<0);
	systick->CSR = (systick->CSR | 1<<1);

	// USART Interrupt Setup
	USART2->CR1 |= 1<<5;
	NVIC_EnableIRQ(USART2_IRQn);

	// Project Setup
	GPIO_Init();
	printHelp();
	writeBulk(remote);
}

/*
 * This function is responsible for handling the internal system
 * clock. It counts for a half second, which is used to flash
 * the LEDs for switching to local mode. The one second counter
 * is used for all other normal operations.
 */
void SysTick_Handler() {
	static uint32_t counter = 0;
	static uint32_t counter2 = 0;
	counter++;
	counter2++;
	if(counter2 == 500) {
		half_second_elapsed = 1;
		counter2 = 0;
	}

	if(counter == 1000) {
		one_second_elapsed = 1;
		counter = 0;
	}
}

/*
 * This function is responsible for handling any character input
 * from the command line. It will echo the character, and store
 * it into a command. The command will adapt to backspaces and
 * re-writes. When enter is hit, the command will be processed.
 */
void USART2_IRQHandler() {
	if(remoteMode) {
		static char command[7];
		static char *cmd_ptr = command;
		uint8_t one_char = 0 ;
		one_char = USART_Read_NB(USART2);
		if(one_char) {
			USART_Write( USART2, &one_char, 1 ) ;
			*cmd_ptr = one_char;
			if(one_char == '\r') {
				// If enter is hit, print the next line, handle the command,
				// and then reset the command.
				*cmd_ptr = '\0';
				one_char = '\n';
				USART_Write( USART2, &one_char, 1 ) ;
				handleCommand(command);
				memset(command, ' ', sizeof(char)*7);
				cmd_ptr = command;
			} else if(one_char == '\b') {
				// To handle backspaces, replace the current selected
				// character on the command line with space and then
				// output another backspace character.
				*cmd_ptr = '\0';
				one_char = ' ';
				USART_Write( USART2, &one_char, 1 ) ;
				one_char = '\b';
				USART_Write( USART2, &one_char, 1 ) ;
				if(!(cmd_ptr <= command)) {
					cmd_ptr--;
				}
			} else {
				cmd_ptr++;
			}
		}
	} else {
		// to clear the USART2 interrupt
		USART_Read_NB(USART2);
	}
}

/*
 * This is the callback interrupt handler for the external
 * interfaces (EXTI). It handles both when the blue button is hit,
 * as well as when the red and green LED buttons are hit. If the blue
 * button is hit, the state of the system will be switched between
 * local and remote. If the red/green LED button is hit, the respective
 * buttons will be turned on.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == B1_Pin) {
		allOff();
		if(remoteMode) { // on remote mode switching to manual override
			remoteMode = 0;
			three_times_flash = 0;
			writeBulk(local);
		} else { // on manual override switching to remote
			remoteMode = 1;
			writeBulk(remote);
		}
	}

	if(!remoteMode) {
		// Reading the pin allows for separate functionality on rising/falling edges
		if(GPIO_Pin == G1_Pin && HAL_GPIO_ReadPin(GPIOC, G1_Pin)) {
			GREEN_LED_On();
		} else if(GPIO_Pin == G1_Pin && !HAL_GPIO_ReadPin(GPIOC, G1_Pin)) {
			GREEN_LED_Off();
		}

		if(GPIO_Pin == R1_Pin && HAL_GPIO_ReadPin(GPIOA, R1_Pin)) {
			RED_LED_On();
		} else if(GPIO_Pin == R1_Pin && !HAL_GPIO_ReadPin(GPIOA, R1_Pin)) {
			RED_LED_Off();
		}
	}
}

/*
 * This function handles running the project. It first initializes the registers, then
 * it handles the system based on the state (local or remote) in an infinite loop. In
 * remote mode, if a second has passed, and an LED is set to be flashing, then toggle its
 * state, and reset the second flag. If the state is local, then flick the LEDs on and
 * off three times in three seconds, and then reset the half second flag.
 */
void run_demo()
{
	init();
	while (1) {
		 if(remoteMode) {
			 if(one_second_elapsed) {
			 	if(getRedFlashing()) {
			 		RED_LED_Toggle();
			 	}
			 	if(getGreenFlashing()) {
			 		GREEN_LED_Toggle();
			 	}
			 	one_second_elapsed = 0;
			 }
		 } else {
			if(half_second_elapsed && three_times_flash < 6) {
				RED_LED_Toggle();
				GREEN_LED_Toggle();
				three_times_flash++;
				half_second_elapsed = 0;
			}
		}
	}
}
