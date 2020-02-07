/*
===============================================================================
 Name        : A_Test1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
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
#include<stdio.h>
#define TICKRATE_HZ (16)//why didn't 1 or 4 work here?
#define PORT_MASK   ((1 << 29) | (1 << 9) | (1 << 10) | (1 << 16) | (1 << 0) | (1 << 24))
//#define PORT_MASK		(1 <<3)
//#define PORT_MASK2		(1 << 1) | (1<<25))
//1 is for output 0 is for input
// TODO: insert other definitions and declarations here

int step = 0;
uint32_t state2;
int n=0;


void delay_ms (uint16_t ms){
uint16_t delay;
volatile uint32_t i;
for (delay = ms; delay >0 ; delay--)
//1ms loop with -Os optimisation
 {
 for (i=3500; i >0;i--){};
 }
}

void SysTick_Handler(void){
	if(step==6){
		step=0;
	}
	switch(step){
	   	case 0:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<0)));//low = off
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<29)));//high = on
	   		break;
	   	case 1:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<16)));
		    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<24)));
		    break;
	   	case 2:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<29)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<10)));
	   		break;
	   	case 3:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<24)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<9)));
	   		break;
	   	case 4:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<10)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<0)));
	   		break;
	   	case 5:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<9)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<16)));
	   		break;
	   	}
	   	step++;
}


int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    //Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here
    Chip_GPIO_SetPortDIROutput(LPC_GPIO, 0, PORT_MASK);
//    Chip_GPIO_SetPortDIROutput(LPC_GPIO, 1, PORT_MASK2);

    Chip_GPIO_SetPortMask(LPC_GPIO, 0, ~PORT_MASK);
//    Chip_GPIO_SetPortMask(LPC_GPIO, 1, ~PORT_MASK2);

    //Set rate with TICKRATE_HZ
    SysTick_Config(SystemCoreClock / TICKRATE_HZ);

    state2 = Chip_GPIO_GetMaskedPortValue(LPC_GPIO, 0);//get initial state of port mask

    //turn everything off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<9)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<10)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<29)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<24)));//low = off

//turn on two initial
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<0)));
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<16)));//half step here
    while(1) {
    	//delay_ms(500);
    	__WFI();
    }
    return 0;
}
