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

int IN1 = 16;
int IN2 = 0;
int IN3 = 24;
int EN1 = 29;
int EN2 = 9;
int EN3 = 10;

#include<stdio.h>
#define TICKRATE_HZ (500)//why didn't 1 or 4 work here?
#define PORT_MASK   ((1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << EN1) | (1 << EN2) | (1 << EN3))
//#define PORT_MASK		(1 <<3)
//#define PORT_MASK2		(1 << 1) | (1<<25))
//1 is for output 0 is for input


#define BOARD_ADC_CH 1
#define ANALOG_INPUT_PORT   0
#define ANALOG_INPUT_BIT    9
#define ANALOG_FIXED_PIN    SWM_FIXED_ADC1_1
// TODO: insert other definitions and declarations here

int step = 0;
uint32_t state2;
int n=0;

static volatile int ticks;
static bool sequence0Complete, sequence1Complete, threshold1Crossed;



/*void delay_ms (uint16_t ms){
uint16_t delay;
volatile uint32_t i;
for (delay = ms; delay >0 ; delay--)
//1ms loop with -Os optimisation
 {
 for (i=3500; i >0;i--){};
 }
}*/


void showValudeADC(LPC_ADC_T *pADC)
{
	int index, j;
	uint32_t rawSample;

/*	if (pADC == LPC_ADC0) {
		index = 0;
	}
	else {
		index = 1;
	}*/
	index =1;//added
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
//		Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
		Chip_ADC_StartSequencer(LPC_ADC1, ADC_SEQA_IDX);
	}
}


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


void SysTick_Handler(void){
	if(step==6){
		step=0;
	}
	switch(step){
	   	case 0:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN1)));//high = on
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN2)));//low = off
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN3)));//low = off
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN1)));//high = on
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN2)));//high = on
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN3)));//low = off
	   		break;
	   	case 1:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN3)));
		    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN1)));
		    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN2)));
		    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN3)));
		    break;
	   	case 2:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN3)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN3)));
	   		break;
	   	case 3:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN3)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN3)));
	   		break;
	   	case 4:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN3)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN3)));
	   		break;
	   	case 5:
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<IN3)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN1)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN2)));
	   		Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<EN3)));
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
    Chip_ADC_Init(LPC_ADC1, 0);
    Chip_ADC_SetClockRate(LPC_ADC1, ADC_MAX_SAMPLE_RATE);
    Chip_ADC_SetTrim(LPC_ADC1, ADC_TRIM_VRANGE_HIGHV);
	/* For ADC1, sequencer A will be used with threshold events.
	   It will be triggered manually by the sysTick interrupt and
	   only monitors the ADC1 input. */
	Chip_ADC_SetupSequencer(LPC_ADC1, ADC_SEQA_IDX,
							(ADC_SEQ_CTRL_CHANSEL(BOARD_ADC_CH) | ADC_SEQ_CTRL_MODE_EOS));
	/* Disables pullups/pulldowns and disable digital mode */
	Chip_IOCON_PinMuxSet(LPC_IOCON, ANALOG_INPUT_PORT, ANALOG_INPUT_BIT,
		(IOCON_MODE_INACT | IOCON_DIGMODE_EN));
	Chip_ADC_StartCalibration(LPC_ADC1);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC1))) {}
	/* Setup threshold 0 low and high values to about 25% and 75% of max for
	     ADC1 only */
	Chip_ADC_SetThrLowValue(LPC_ADC1, 0, ((1 * 0xFFF) / 4));
	Chip_ADC_SetThrHighValue(LPC_ADC1, 0, ((3 * 0xFFF) / 4));
	Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
	/* Enable sequence A completion and threshold crossing interrupts for ADC1_1 */
	Chip_ADC_EnableInt(LPC_ADC1, ADC_INTEN_SEQA_ENABLE |
					   ADC_INTEN_CMP_ENABLE(ADC_INTEN_CMP_CROSSTH, BOARD_ADC_CH));
	/* Use threshold 0 for ADC channel and enable threshold interrupt mode for
	   channel as crossing */
	Chip_ADC_SelectTH0Channels(LPC_ADC1, ADC_THRSEL_CHAN_SEL_THR1(BOARD_ADC_CH));
	Chip_ADC_SetThresholdInt(LPC_ADC1, BOARD_ADC_CH, ADC_INTEN_THCMP_CROSSING);
	/* Enable related ADC NVIC interrupts */
	NVIC_EnableIRQ(ADC1_SEQA_IRQn);
	NVIC_EnableIRQ(ADC1_THCMP);
	Chip_ADC_EnableSequencer(LPC_ADC1, ADC_SEQA_IDX);






    Chip_GPIO_SetPortDIROutput(LPC_GPIO, 0, PORT_MASK);
//    Chip_GPIO_SetPortDIROutput(LPC_GPIO, 1, PORT_MASK2);

    Chip_GPIO_SetPortMask(LPC_GPIO, 0, ~PORT_MASK);
//    Chip_GPIO_SetPortMask(LPC_GPIO, 1, ~PORT_MASK2);

    //Set rate with TICKRATE_HZ
    SysTick_Config(SystemCoreClock / TICKRATE_HZ);

    state2 = Chip_GPIO_GetMaskedPortValue(LPC_GPIO, 0);//get initial state of port mask

    //turn everything off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN1)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN2)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<IN3)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN1)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN2)));//low = off
    Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 &= ~(1<<EN3)));//low = off



//turn on two initial
    //Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<0)));
    //Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, (state2 |= (1<<16)));//half step here
    while(1) {
    	//delay_ms(500);
    	__WFI();



		if (threshold1Crossed) {
			threshold1Crossed = false;
			DEBUGSTR("********ADC1 threshold event********\r\n");
		}

		/* Is a conversion sequence complete? */
		if (sequence1Complete) {
			showValudeADC(LPC_ADC1);
		}
	}
    return 0;
}
