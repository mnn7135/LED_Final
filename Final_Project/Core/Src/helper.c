#include <stdio.h>
#include <string.h>

#include "helper.h"
#include "UART.h"
#include "LED.h"

static int redFlashing = 0;
static int greenFlashing = 0;
static unsigned char *output = (unsigned char*) "***REMOTE LED CONTROL MENU***\r\n"
		"RON - Turn on RED LED\r\n"
		"ROFF - Turn off RED LED\r\n"
		"GON - Turn on GREEN LED\r\n"
		"GOFF - Turn off GREEN LED\r\n"
		"RFLASH - Start flashing RED LED\r\n"
		"GFLASH - Start flashing GREEN LED\r\n"
		"ALLOFF - TURNOFF LEDs\r\n\0";
static unsigned char *invalid = (unsigned char*) "Invalid command\r\n\0";

/*
 * A helper function that compares the contents of
 * two strings by value.
 */
int compare_str(char* str1, char* str2) {
	while(*str1 == *str2) {
		if(*str1 == '\0') {
			return 1;
		}
		str1++;
		str2++;
	}
	return 0;
}

/*
 * This helper function takes in a string and outputs each character
 * in a while loop.
 */
void writeBulk(unsigned char* text) {
	unsigned char* text_ptr = text;
	while(*text_ptr != '\0') {
		USART_Write( USART2, text_ptr, 1 );
		text_ptr++;
	}
}

/*
 * This helper function handles all commands that a user can input.
 * It also provides handling for if a user inputs something not
 * recognized as a valid command.
 */
void handleCommand(char* command) {
	if(compare_str(command, "HELP\0") == 1) {
		printHelp();
	} else if(compare_str(command, "RON\0") == 1) {
		redOn();
	} else if(compare_str(command, "ROFF\0") == 1) {
		redOff();
	} else if(compare_str(command, "GON\0") == 1) {
		greenOn();
	} else if(compare_str(command, "GOFF\0") == 1) {
		greenOff();
	} else if(compare_str(command, "RFLASH\0") == 1) {
		redFlashing = 1;
	} else if(compare_str(command, "GFLASH\0") == 1) {
		greenFlashing = 1;
	} else if(compare_str(command, "ALLOFF\0") == 1) {
		allOff();
	} else {
		writeBulk(invalid);
	}
}

/*
 * A helper function that prints out the available remote commands
 */
void printHelp() {
	writeBulk(output);
}

/*
 * A helper function that turns the red LED on.
 */
void redOn() {
	redFlashing = 0;
	RED_LED_On();
}

/*
 * A helper function that turns the red LED off.
 */
void redOff() {
	redFlashing = 0;
	RED_LED_Off();
}

/*
 * A helper function that turns the green LED on.
 */
void greenOn() {
	greenFlashing = 0;
	GREEN_LED_On();
}

/*
 * A helper function that turns the green LED off.
 */
void greenOff() {
	greenFlashing = 0;
	GREEN_LED_Off();
}

/*
 * A helper function that turns all LEDs off by calling their
 * respective off functions.
 */
void allOff() {
	redOff();
	greenOff();
}

/*
 * A helper function that flips the flag that indicates
 * whether or not to flash the Green LED.
 */
void setGreenFlashing() {
	greenFlashing = ~greenFlashing;
}

/*
 * A helper function that returns the whether or not to flash
 * the Green LED.
 */
int getGreenFlashing() {
	return greenFlashing;
}

/*
 * A helper function that flips the flag that indicates
 * whether or not to flash the Red LED.
 */
void setRedFlashing() {
	redFlashing = ~redFlashing;
}

/*
 * A helper function that returns the whether or not to flash
 * the Red LED.
 */
int getRedFlashing() {
	return redFlashing;
}
