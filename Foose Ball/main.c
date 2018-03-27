#include <msp430.h> 
#include "LCD_Lib.h"
#include "LCD_Lib2.h"

int Home_score = 0, Away_score = 0, Game_St = 0;
int flag = 0;
int left = 2;		// Timer left digit: minuites.
int right1 = 0;		// Timer right digit: seconds (higher).
int right2 = 0;	    // Timer right most digit: seconds (lower).
unsigned int pulse_count = 0; // Pluse counter: counting pulse input.

// XIN is P2.6 and XOUT is P2.7. Must de-select them in order to use them as normal pins.
#define BUTTON BIT6

void Home_LCD_Setup(void);	// Home screen function prototype
void Away_LCD_Setup(void);	// Away screen function prototype

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P1OUT = 0;
    P2OUT = 0;
    P1DIR = 0;
    P2DIR = 0;
    P2SEL &= ~BIT6;

    //********************************************************
    //		Interrupt To trigger goal	count		         *
    //********************************************************
    P1IE |= (BIT6 | BIT7);	 // P1.6, P1.7 interrupt enabled *
    P1IES &= ~BIT6;			 // P1.6 Lo/Hi edge		         *
    P1IES &= ~BIT7;	 		 // P1.7 Lo/Hi edge	   	         *
	P1IFG &= ~BIT6;			 // P1.6 IFG cleared             *
	P1IFG &= ~BIT7;			 // P1.6 IFG cleared	         *
	//********************************************************


	//******************************************************
	//		Interrupt To trigger Timer		 		       *
	//******************************************************
	P2DIR &= ~BUTTON;	// P2.6 						   *
	P2IE |= BUTTON;		// P2.6 interrupt enabled	       *
	P2REN |= BUTTON;	// P2.6 internal resistor enable   *
	P2OUT &= ~BUTTON;	// P2.6 pull/down resitor selected *
	P2IES &= ~BUTTON;	//								   *
	P2IFG &= ~BUTTON;	//								   *
	//******************************************************

    lcdInit();				// Initializing LCD1: Home
    lcdInit2();				// Initializing LCD2: Away
    lcdWriteCmd(0x01); 		// Clear LCD1
    lcdWriteCmd2(0x01);		// Clear LCD2

    delay_ms(200);			// Wait about 200ms for it to be ready.
    Home_LCD_Setup();		// Home screen LCD setup function.
    Away_LCD_Setup();		// Home screen LCD setup function.

    __enable_interrupt();

    //*********************************************************
    //			Main Loop Starts...							  *
    //					Forever Loop...						  *
    //*********************************************************

    for(;;)
    {

    		if (flag) // Timer starts when the button is pressed: interrupt
    		{
    			right2--;
    			if (right2 <= -1)
    			{
    				right1--;
    				right2 = 9;
    			}

    			if (right1 <= -1)
    			{
    				left--;
    				right1 = 5;
    				right2 = 9;
    			}

    			if (left <= -1)
    			{
    				left = 2;
    				right1 = 0;
    				right2 = 0;
    				flag = 0;

    				if (Home_score > Away_score)
    				{
    					 lcdWriteCmd(0x01); 		// Clear LCD1
    					 lcdWriteCmd2(0x01);		// Clear LCD2
    					 delay_ms(5);
    					 lcdSetText("HOME ",8, 3);
    					 lcdSetText("----->", 0, 0);
    					 lcdSetText("WINNER..!!!", 5, 1);
    					 lcdSetText2("AWAY ",8, 3);
    					 lcdSetText2("<-----", 0, 0);
    					 lcdSetText2("LOSER..!!!", 5, 1);
    					 delay_ms(5004);
    				}

    				else if (Away_score > Home_score)
    				{
    				    lcdWriteCmd(0x01); 		// Clear LCD1
    				    lcdWriteCmd2(0x01);		// Clear LCD2
    				    delay_ms(5);
    				    lcdSetText2("AWAY",8, 3);
    				    lcdSetText2("<-----", 0, 0);
    				    lcdSetText2("WINNER..!!!", 5, 1);
    				    lcdSetText("HOME",8, 3);
    				    lcdSetText("----->", 0, 0);
    				    lcdSetText("LOSER..!!!", 5, 1);
    				    delay_ms(5004);
    				 }

    				else if (Home_score == Away_score)
    				{
    				    lcdWriteCmd(0x01); 		// Clear LCD1
    				    lcdWriteCmd2(0x01);		// Clear LCD2
    				    delay_ms(5);
    				    lcdSetText("HOME ",8, 3);
    				    lcdSetText("GAME..DRAW..!!", 4, 1);
    				    lcdSetText2("AWAY ",8, 3);
    				    lcdSetText2("GAME..DRAW..!!", 4, 1);
    				    delay_ms(6004);
    				 }

    				Home_score = 0;          // Home score reset.
    				Away_score = 0;          // Away score reset.
    				lcdWriteCmd(0x01); 		// Clear LCD1
    				lcdWriteCmd2(0x01);		// Clear LCD2
    				Home_LCD_Setup();		// Home screen LCD setup function.
    				Away_LCD_Setup();		// Home screen LCD setup function.
    			}

    			delay_ms(1004);

    			if (flag)
    			{
    				lcdSetInt(left, 14, 1);		// Displaying the letf digt:mins on the "HOME" LCD screen.
    				lcdSetInt2(left, 14, 1);	// Displaying the letf digt:mins on the "AWAY" LCD screen.
    				lcdSetInt(right1, 16, 1);	// Displaying the right digt:secs on the "HOME" LCD screen.
    				lcdSetInt2(right1, 16, 1);	// Displaying the right digt:secs on the "AWAY" LCD screen.
    				lcdSetInt(right2, 17, 1);	// Displaying the right most digt:secs on the "HOME" LCD screen.
    				lcdSetInt2(right2, 17, 1);	// Displaying the right most digt:secs on the "AWAY" LCD screen.
    			}
    		//lcdSetInt(Home_score, 5, 1);
    		//lcdSetInt2(Away_score, 5, 1);

    		}


    }

}

void Home_LCD_Setup(void)
{
	//*******************************************************
    //			Set-up "Home" screen LCD diplay				*
	//*******************************************************
	lcdSetText("HOME ", 8, 3);							   //
	lcdSetText("Score: ", 2,0); 					       //
	lcdSetText("Time:", 13, 0);						       //
	lcdSetInt(Home_score, 5, 1);						   //
	lcdSetInt(left, 14, 1);								   //
	lcdSetText(": ", 15, 1);							   //
	lcdSetInt(right1, 16, 1);							   //
	lcdSetInt(right2, 17, 1);							   //
    //*******************************************************
}

void Away_LCD_Setup(void)
{
	 //*******************************************************
	 //		  Set-up "Away" screen LCD diplay				 *
	 //*******************************************************
	 lcdSetText2("AWAY ",8, 3);								//
	 lcdSetText2("Score: ", 2,0);							//
	 lcdSetText2("Time:", 13, 0);							//
	 lcdSetInt2(Away_score, 5, 1);							//
	 lcdSetInt2(left, 14, 1);								//
	 lcdSetText2(": ", 15, 1);								//
	 lcdSetInt2(right1, 16, 1);								//
	 lcdSetInt2(right2, 17, 1);								//
	 //*******************************************************
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	__disable_interrupt();

	 if (P1IFG & BIT6)
	{
		if (flag)
		{
		Home_score++;
		delay_ms(3);
		lcdSetInt(Home_score, 5, 1);
		delay_us(30);
		}
		P1IFG &= ~BIT6;
	}

	else if (P1IFG & BIT7)
	{
		if (flag)
		{
		Away_score++;
		delay_ms(3);
		lcdSetInt2(Away_score, 5, 1);
		delay_us(30);
		}
		P1IFG &= ~BIT7;
	}

	__enable_interrupt();

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	__disable_interrupt();
	Game_St = 5;

	if (P2IFG & BUTTON)
		{
			if (flag )	// Game stop/paused
			{
				flag = 0;
				delay_ms(2);
				lcdWriteCmd(0x01); 		// Clear LCD1
			    lcdWriteCmd2(0x01); 	// Clear LCD2
			    lcdSetText("Game Paused", 3, 1);
			    lcdSetText2("Game Paused", 3, 1);
				//delay_ms(100);
				P2IFG &= ~BUTTON;
			}

			else if (!flag)	// Game starts
			{
				delay_ms(2);
				lcdWriteCmd(0x01); 		// Clear LCD1
				lcdWriteCmd2(0x01); 	// Clear LCD2
				lcdSetText("Game Starts In:", 1, 0);
				lcdSetText2("Game Starts In:", 1, 0);

				while (Game_St != -1)
				{
					lcdSetInt(Game_St, 14, 2);
					lcdSetInt2(Game_St, 14, 2);
					delay_ms(1004);
					Game_St--;
				}
				flag = 1;
				delay_ms(2);
				lcdWriteCmd(0x01);
				lcdWriteCmd2(0x01);
				Home_LCD_Setup();
				Away_LCD_Setup();
				P2IFG &= ~BUTTON;
			}

		}

		__enable_interrupt();
}
