/*
===============================================================================
 Name        : Copy5-adcRom.c
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

// TODO: insert other definitions and declarations here
#define ANALOG_FIXED_PIN	SWM_FIXED_ADC0_1
#define ADC_CLOCK_RATE		2000000
#define BOARD_ADC_CH 1
#define TRIGPOL_BIT			(1<<18)
#define SEQA_ENA_BIT		(1<<31)
#define CHANNELS_BITS		(1<<1)
#define START_BIT			(1<<26)
#define pADC				LPC_ADC0

static ADC_CONFIG_T         adc_cfg[2];	/* Config Structure for ADC ROM API */
uint32_t rawSample;

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
//enable clock to ADC0 register
	LPC_SYSCON->SYSAHBCLKCTRL0 |= (1<<27);
//clear ADC0 peripheral reset
	LPC_SYSCON->PRESETCTRL &= (0<<27);

	//LPC_SWM->PINENABLE[ANALOG_FIXED_PIN>>7] &= ~(1 << ANALOG_FIXED_PIN);//?
//enabling PIO0_7 for ADC)_1
	LPC_SWM->PINENABLE &= (0<<1);
//Powering on ADC0
	LPC_SYSCON->PDRUNCFG &= (0<<10);

//Select low power mode to turn off ADC when not sampling
	//adc_cfg[0]

//hardware self calibration
	Chip_ADC_StartCalibration(LPC_ADC0);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC0))) {}







    /* Setup ADC for 12-bit mode and normal power */

//    PDRUNCFG &= (0<<10);

//1.
    LPC_SWM->PINENABLE[ANALOG_FIXED_PIN>>7] &= ~(1 << ANALOG_FIXED_PIN);//?
    Chip_ADC_Init(LPC_ADC0, 1);

	/* Assign ADC1_1 to PIO0_9 via SWM (fixed pin) */
	//Chip_SWM_EnableFixedPin(ANALOG_FIXED_PIN);*/

//2.
	/* ADC0 Config */
	adc_cfg[0].system_clock = SystemCoreClock;	/* System clock */
	adc_cfg[0].adc_clock = ADC_CLOCK_RATE;	/* ADC clock set to 500KHz for calibration*/

//3.
	adc_cfg[0].async_mode = 0;	//Synchronous mode

while(1){
//4-5-6.
	adc_cfg[0].seqa_ctrl = (TRIGPOL_BIT | SEQA_ENA_BIT | START_BIT | CHANNELS_BITS);

//7.
		rawSample = Chip_ADC_GetDataReg(pADC, 1);
		printf("ADC0_1: %d\n", rawSample);
	}
	return 0 ;
}
