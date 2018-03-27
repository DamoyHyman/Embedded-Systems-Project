
#ifndef LCD_LIB_H_
#define LCD_LIB_H_

#include <msp430g2553.h>
#include <string.h>
#include <stdio.h>

// Delay Functions
#define delay_ms(x)		__delay_cycles((long) x* 1000)
#define delay_us(x)		__delay_cycles((long) x)

// Pins
#define EN		BIT4
#define RS		BIT5
#define DATA	0x0F

// Commands
#define CLEAR	0x01

// Functions
void lcdInit();									// Initialize LCD
void lcdTriggerEN();							// Trigger Enable
void lcdWriteData(unsigned char data);			// Send Data (Characters)
void lcdWriteCmd(unsigned char cmd);			// Send Commands
void lcdClear();								// Clear LCD
void lcdSetText(char * text, int x, int y);		// Write string
void lcdSetInt(int val, int x, int y);			// Write integer



#endif /* LCD_LIB_H_ */
