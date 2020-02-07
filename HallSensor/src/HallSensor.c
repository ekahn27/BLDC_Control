/*
===============================================================================
 Name        : HallSensor.c
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
static volatile int ticks;
static bool sequence0Complete, sequence1Complete, threshold1Crossed;

#define TICKRATE_HZ (100)	/* 100 ticks per second */



//#define PORT_MASK	((1<<29) | (1<<10) | (1<<16))//not for actual board used
#define ANALOG_FIXED_PIN1    SWM_FIXED_ADC0_0
#define ANALOG_FIXED_PIN2    SWM_FIXED_ADC0_1
#define ANALOG_FIXED_PIN3    SWM_FIXED_ADC0_2

#define ANALOG_INPUT_PORT1	0
#define ANALOG_INPUT_BIT1	8

#define ANALOG_INPUT_PORT2	0
#define ANALOG_INPUT_BIT2	7


#define BOARD_ADC_CH 1
#define ANALOG_INPUT_PORT   0
#define ANALOG_INPUT_BIT    9
#define ANALOG_FIXED_PIN    SWM_FIXED_ADC1_1//similar to above analog fixed pin define
uint32_t rawSample;

/*
static void app_setup_pin(void){
	#if defined(BOARD_NXP_LPCXPRESSO_1549)
	Chip_SWM_FixedPinAssign(SWM_ADC_10, 29);
	Chip_SWM_FixedPinAssign(SWM_ADC0_7, 10);
	Chip_SEM_FixedPinAssign(SWM_ADC0_6, 16);
#endif
}*/
void showValudeADC(LPC_ADC_T *pADC)
{
	int index, j;
	uint32_t rawSample;

	if (pADC == LPC_ADC0) {
		index = 0;
	}
	else {
		index = 1;
	}

	/* Get raw sample data for channels 0-11 */
	for (j = 0; j < 12; j++) {
		rawSample = Chip_ADC_GetDataReg(pADC, j);

		/* Show some ADC data */
		if ((rawSample & (ADC_DR_OVERRUN | ADC_SEQ_GDAT_DATAVALID)) != 0) {
			DEBUGOUT("ADC%d_%d: Sample value = 0x%x (Data sample %d)\r\n", index, j,
					 ADC_DR_RESULT(rawSample), j);

			/* Threshold events are only on ADC1 */
			if (index == 1) {
				DEBUGOUT("ADC%d_%d: Threshold range = 0x%x\r\n", index, j,
						 ADC_DR_THCMPRANGE(rawSample));
				DEBUGOUT("ADC%d_%d: Threshold cross = 0x%x\r\n", index, j,
						 ADC_DR_THCMPCROSS(rawSample));
			}
		}
	}
}

void SysTick_Handler(void)
{
	static uint32_t count;

	/* Every 1/2 second */
	count++;
	if (count >= (TICKRATE_HZ / 2)) {
		count = 0;

		/* Manual start for ADC1 conversion sequence A */
		Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
		Chip_ADC_StartSequencer(LPC_ADC1, ADC_SEQA_IDX);
	}
}

/**
 * @brief	Handle interrupt from ADC0 sequencer A
 * @return	Nothing
 */
void ADC0A_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC0);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequence0Complete = true;
	}

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, pending);
}

/**
 * @brief	Handle interrupt from ADC1 sequencer A
 * @return	Nothing
 */
void ADC1A_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC1);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequence1Complete = true;
	}

	/* Clear Sequence A completion interrupt */
	Chip_ADC_ClearFlags(LPC_ADC1, ADC_FLAGS_SEQA_INT_MASK);
}

/**
 * @brief	Handle threshold interrupt from ADC1
 * @return	Nothing
 */
void ADC1_THCMP_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC1);

	/* Threshold crossing interrupt on ADC input channel */
	if (pending & ADC_FLAGS_THCMP_MASK(BOARD_ADC_CH)) {
		threshold1Crossed = true;
	}

	/* Clear threshold interrupt */
	Chip_ADC_ClearFlags(LPC_ADC1, ADC_FLAGS_THCMP_MASK(BOARD_ADC_CH));
}

/**
 * @brief	main routine for ADC example
 * @return	Function should not exit
 */




int j;

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

    /* Setup ADC for 12-bit mode and normal power */
    Chip_ADC_Init(LPC_ADC0, 0);
    Chip_ADC_Init(LPC_ADC0, 1);

    /* Setup for maximum ADC clock rate */
   	Chip_ADC_SetClockRate(LPC_ADC0, ADC_MAX_SAMPLE_RATE);

	/* For ADC0, sequencer A will be used without threshold events.
	   It will be triggered manually by the sysTick interrupt and
	   only monitor the internal temperature sensor. */
	Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(0) |
													 ADC_SEQ_CTRL_MODE_EOS));

	/* Use higher voltage trim for both ADCs */
	Chip_ADC_SetTrim(LPC_ADC0, ADC_TRIM_VRANGE_HIGHV);
	Chip_ADC_SetTrim(LPC_ADC1, ADC_TRIM_VRANGE_HIGHV);

	/* For ADC1, sequencer A will be used with threshold events.
	   It will be triggered manually by the sysTick interrupt and
	   only monitors the ADC1 input. */
	Chip_ADC_SetupSequencer(LPC_ADC1, ADC_SEQA_IDX,
							(ADC_SEQ_CTRL_CHANSEL(BOARD_ADC_CH) | ADC_SEQ_CTRL_MODE_EOS));

	// Disables pullups/pulldowns and disable digital mode
	Chip_IOCON_PinMuxSet(LPC_IOCON, ANALOG_INPUT_PORT1, ANALOG_INPUT_BIT1,
		(IOCON_MODE_INACT | IOCON_DIGMODE_EN));

	Chip_IOCON_PinMuxSet(LPC_IOCON, ANALOG_INPUT_PORT2, ANALOG_INPUT_BIT2,
			(IOCON_MODE_INACT | IOCON_DIGMODE_EN));

	/* Assign ADC0_0 ,1,2 to PIO0_8 ,7,6 via SWM (fixed pin)*/
	Chip_SWM_EnableFixedPin(ANALOG_FIXED_PIN1); //ADC0_0, etc
	Chip_SWM_EnableFixedPin(ANALOG_FIXED_PIN2); //ADC0_1
	Chip_SWM_EnableFixedPin(ANALOG_FIXED_PIN3); //ADC0_2

	// Need to do a calibration after initialization and trim
		Chip_ADC_StartCalibration(LPC_ADC0);
		while (!(Chip_ADC_IsCalibrationDone(LPC_ADC0))) {}
		Chip_ADC_StartCalibration(LPC_ADC1);
		while (!(Chip_ADC_IsCalibrationDone(LPC_ADC1))) {}

	/* Setup threshold 0 low and high values to about 25% and 75% of max for
	     ADC1 only */
	Chip_ADC_SetThrLowValue(LPC_ADC0, 0, ((1 * 0xFFF) / 4));
	Chip_ADC_SetThrHighValue(LPC_ADC0, 0, ((3 * 0xFFF) / 4));

	Chip_ADC_SetThrLowValue(LPC_ADC0, 1, ((1 * 0xFFF) / 4));
	Chip_ADC_SetThrHighValue(LPC_ADC0, 1, ((3 * 0xFFF) / 4));

	/* Clear all pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
	Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));


	/* Enable sequence A completion interrupts for ADC0 */
		Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);

		/* Enable sequence A completion and threshold crossing interrupts for ADC1_1 */
		Chip_ADC_EnableInt(LPC_ADC1, ADC_INTEN_SEQA_ENABLE |
						   ADC_INTEN_CMP_ENABLE(ADC_INTEN_CMP_CROSSTH, BOARD_ADC_CH));

		/* Use threshold 0 for ADC channel and enable threshold interrupt mode for
		   channel as crossing */
		Chip_ADC_SelectTH0Channels(LPC_ADC1, ADC_THRSEL_CHAN_SEL_THR1(BOARD_ADC_CH));
		Chip_ADC_SetThresholdInt(LPC_ADC1, BOARD_ADC_CH, ADC_INTEN_THCMP_CROSSING);

		/* Enable related ADC NVIC interrupts */
		NVIC_EnableIRQ(ADC0_SEQA_IRQn);
		NVIC_EnableIRQ(ADC1_SEQA_IRQn);
		NVIC_EnableIRQ(ADC1_THCMP);

		/* Enable sequencers */
		Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);
		Chip_ADC_EnableSequencer(LPC_ADC1, ADC_SEQA_IDX);

		/* This example uses the periodic sysTick to manually trigger the ADC,
		   but a periodic timer can be used in a match configuration to start
		   an ADC sequence without software intervention. */
		SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE_HZ);

    while(1) {
    	/*for(j=0; j<12; j++){
    		rawSample = Chip_ADC_GetDataReg(LPC_ADC0, j);
    		printf("ADC0_%d: Sample Value = 0x%x\n", j, ADC_DR_RESULT(rawSample));
    		*/

    	/* Sleep until something happens */
    			__WFI();

    			if (threshold1Crossed) {
    				threshold1Crossed = false;
    				DEBUGSTR("********ADC1 threshold event********\r\n");
    			}

    			/* Is a conversion sequence complete? */
    			if (sequence0Complete) {
    				showValudeADC(LPC_ADC0);
    			}
    			if (sequence1Complete) {
    				showValudeADC(LPC_ADC1);
    			}
    	}
    return 0 ;
}
