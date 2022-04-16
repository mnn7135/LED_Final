// SWEN 340 Demo code header file
// Larry Kiser copyright 2021



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEMO_H
#define __DEMO_H


void run_demo( void ) ;
/* Added for Final Project */
void redOn(void);
void redOff(void);
void greenOn(void);
void greenOff(void);
void redFlash(void);
void greenFlash(void);
void allOff(void);
void handleCommand(char* command);
void writeBulk(unsigned char* text);
int compare_str(char* str1, char* str2);
int getHalfSecond(void);

#endif /* __DEMO_H */
