#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Number of tickrate per second */
#define TICKRATE_HZ (10)

#if defined(BOARD_NXP_LPCXPRESSO_1549)
/* LPCXpresso LPC1549 board has LEDs on P0_0, P0_1, and P0_24. We'll toggle them
   all at once. */
#define PORT_MASK       ((1 << 0) | (1 << 1) | (1 << 24))

#else
#error "No PORT_MASK defined"
#endif /* defined(BOARD_NXP_LPCXPRESSO_1549) */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	uint32_t states;

	/* Get current masked port states */
	states = Chip_GPIO_GetMaskedPortValue(LPC_GPIO, 0);

	/* Toggle all the states */
	states = ~states;

	/* Write states back via masked set function. Only the enanled
	   (masked states) will be changed. */
	Chip_GPIO_SetMaskedPortValue(LPC_GPIO, 0, states);
}

/**
 * @brief	Main program body
 * @return	Does not return
 */
int main(void) {

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Chip_GPIO_Init(LPC_GPIO) is called as part of Board_Init() */

	/* Set port 0 pins to the output direction */
	Chip_GPIO_SetPortDIROutput(LPC_GPIO, 0, PORT_MASK);
//Chip_GPIO_SetPortDIRInput();

	/* Set GPIO port mask value to make sure only port 0
	    selected pins are activated during writes */
	Chip_GPIO_SetPortMask(LPC_GPIO, 0, ~PORT_MASK);

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* All work happens in the systick interrupt handler */
	while (1) {
		__WFI();
	}

	return 0;
}
