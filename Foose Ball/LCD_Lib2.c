#include "LCD_Lib2.h"
// Away LCD Function Definition
#define	LOWNIB2(x)	P2OUT = (P2OUT & 0xF0) + (x & 0x0F)

void lcdInit2() {
	delay_ms2(100);
	// Wait for 100ms after power is applied.

	P2DIR = EN + RS + DATA;  // Make pins outputs
	P2OUT = 0x03;  // Start LCD (send 0x03)

	lcdTriggerEN2(); // Send 0x03 3 times at 5ms then 100 us
	delay_ms2(5);
	lcdTriggerEN2();
	delay_ms2(5);
	lcdTriggerEN2();
	delay_ms2(5);

	P2OUT = 0x02; // Switch to 4-bit mode
	lcdTriggerEN2();
	delay_ms2(5);

	lcdWriteCmd2(0x28); // 4-bit, 2 line, 5x8
	lcdWriteCmd2(0x08); // Instruction Flow
	lcdWriteCmd2(0x01); // Clear LCD
	lcdWriteCmd2(0x06); // Auto-Increment
	lcdWriteCmd2(0x0C); // Display On, No blink
}

void lcdTriggerEN2() {
	P2OUT |= EN;
	P2OUT &= ~EN;
}

void lcdWriteData2(unsigned char data) {
	P2OUT |= RS; // Set RS to Data
	LOWNIB2(data >> 4); // Upper nibble
	lcdTriggerEN2();
	LOWNIB2(data); // Lower nibble
	lcdTriggerEN2();
	delay_us2(50); // Delay > 47 us
}

void lcdWriteCmd2(unsigned char cmd) {
	P2OUT &= ~RS; // Set RS to Data
	LOWNIB2(cmd >> 4); // Upper nibble
	lcdTriggerEN2();
	LOWNIB2(cmd); // Lower nibble
	lcdTriggerEN2();
	delay_ms2(5); // Delay > 1.5ms
}

void lcdSetText2(char* text, int x, int y) {
	int i;
	if (x < 20) {// JUST CHANGE TO 20
		x |= 0x80; // Set LCD for first line write
		switch (y){
		case 1:
			x |= 0x40; // Set LCD for second line write
			break;
		case 2:
			x |= 0x94; // Set LCD for third line write reverse
			break;
		case 3:
			x |= 0xD4; // Set LCD for forth line write reverse
			break;
		}
		lcdWriteCmd2(x);
	}
	i = 0;

	while (text[i] != '\0') {
		lcdWriteData2(text[i]);
		i++;
	}
}

void lcdSetInt2(int val, int x, int y){
	char number_string[16];
	sprintf(number_string, "%d", val); // Convert the integer to character string
	lcdSetText2(number_string, x, y);
}

void lcdClear2()
{
	lcdWriteCmd2(CLEAR);
}



/*
void lcdWriteStrPause(char *str, long waitCnt)
{
        long cnt;
        int c = 0;
    while (*str != '\0')
    {
    	lcdSetText2(str++, c, 1);

        for(cnt = 0; cnt < waitCnt; cnt++)
        {
                __delay_cycles(1*2500);
        }
        c++;

        if (c >= 11)
        {
        	c = 0;
        }
    }
}



// Working progress.
// prints characters from left to right of display
void lcdAnimateRight(char *str, int dispWidth, char justify, int pause)
{
        int len = strlen(str);
        int pad = (dispWidth - len);
        int cnt;

        if (pad < 0)
        {
                pad = 0;
        }

        if (justify == 'C')
        {
                for(cnt = 0; cnt < (pad/2); cnt++)
                {
                	lcdSetText2(" ", 0, 1);
                }
        }

        if (justify == 'R')
        {
                for(cnt = 0; cnt < pad; cnt++)
                {
                	lcdSetText2(" ", 0, 1);
                }
        }

        lcdWriteStrPause(str, pause);
}

*/






