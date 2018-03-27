#ifndef LCD_LIB2_H_
#define LCD_LIB2_H_

#include <msp430g2553.h>
#include <string.h>
#include <stdio.h>

// Delay Functions
#define delay_ms2(x)		__delay_cycles((long) x* 1000)
#define delay_us2(x)		__delay_cycles((long) x)

// Pins
#define EN		BIT4
#define RS		BIT5
#define DATA	0x0F

// Just Made
#define CPUSPEED	16MHZ
// Commands
#define CLEAR	0x01

// Functions
void lcdInit2();									// Initialize LCD
void lcdTriggerEN2();							// Trigger Enable
void lcdWriteData2(unsigned char data);			// Send Data (Characters)
void lcdWriteCmd2(unsigned char cmd);			// Send Commands
void lcdClear2();								// Clear LCD
void lcdSetText2(char * text, int x, int y);		// Write string
void lcdSetInt2(int val, int x, int y);			// Write integer

void lcdAnimateRight(char *str, int dispWidth, char justify, int pause);
void lcdWriteStrPause(char *str, long waitCnt);

#endif /* LCD_LIB2_H_ */
