/*
===============================================================================
 Name        : Timer1.c
 Author      : Eric
 Version     :
 Copyright   : $(copyright)
 Description : Trapezoidal Control
===============================================================================
*/
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

void SysTick_Handler(void)
{
	Board_LED_Toggle(0);
	Board_LED_Toggle(1);
}



int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
#endif
#endif

    // TODO: insert code here
    uint32_t sysTickRate;
	/* The sysTick counter only has 24 bits of precision, so it will
	   overflow quickly with a fast core clock. You can alter the
	   sysTick divider to generate slower sysTick clock rates. */
	Chip_Clock_SetSysTickClockDiv(1);

	/* A SysTick divider is present that scales the sysTick rate down
	   from the core clock. Using the SystemCoreClock variable as a
	   rate reference for the SysTick_Config() function won't work,
	   so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	sysTickRate = Chip_Clock_GetSysTickClockRate();

	//Set two speeds equal initially
	float speedInit = 15;//15 ticks per sec
	float speedNew = 15;

	/* LEDs toggle in interrupt handlers */
	while(1){
		//get analog input for speed and set equal to speedNew, hallSensor?
		if (speedInit != speedNew ){
			speedInit = speedNew;
			float tickRate_HZ1 = 15;//figure out this assignment
		}

		/* Enable and setup SysTick Timer at a periodic rate */
		SysTick_Config(sysTickRate / tickRate_HZ1);
		__WFI();
	}

    return 0 ;
}
