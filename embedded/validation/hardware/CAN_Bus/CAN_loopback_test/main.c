/* Code Warrior Includes */
#include <hidef.h>
#include "derivative.h"

#include "CAN.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"



void main(void)
{

	unsigned char errorflag = CAN_NO_ERROR;
	unsigned char txbuff[] = "ABDDEFGH";

	CANInit();

	while (!(CANCTL0 & CAN_SYNC)) {
		// Wait for MSCAN to synchronize with the CAN bus
	};

	// Enable CAN Rx interrupts
	CANRFLG = 0xC3;
	// Clear CAN Rx flag
	CANRIER = 0x01;
	EnableInterrupts;

	/*errorflag = CANTx(ST_ID_100, 0x00,sizeof(txbuff)-1, txbuff); */
	/* msDelay(250); */

	for (;;) {
		msDelay(250);
		msDelay(250);
		msDelay(250);
		msDelay(250);
		_FEED_COP(); /* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
}
