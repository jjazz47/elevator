/* Code Warrior Includes */
#include <hidef.h>
#include "derivative.h"

/* Standard Includes */
#include <stdlib.h>
#include "CAN.h"

unsigned char rxdata[8];
// Code adapted from AN3034

// ******************************************************************
//                        CANInit()
//        Configures and starts the CAN controller
// ******************************************************************

void CANInit(void)
{
	// Enter Initialization mode : CANCTL0 Register
	CANCTL0 = INIT_MODE;

	while (!(CANCTL1 & INIT_MODE)) {
		// Wait for acknowledgment that
		// initialization mode has started
	}

	// Set the CAN mode : CANCTL1 Register
	// Enable normal mode with bus clock
	CANCTL1 = CAN_ENABLED_MODE_BUS_CLOCK;

	// Enable Loopback mode with bus clock
	//CANCTL1 = LOOPBACK_MODE_BUS_CLOCK;

	// Set the baud rate to 125k : CANBTR0 and CANBTR1 Register
	CANBTR0 = BIT_0_125K;
	CANBTR1 = BIT_1_125K;

	// Set filters
	// Define four 16-bit filters
	CANIDAC = FOUR_16BIT_FILTERS;

	// Define 16 bit filters that accept all messages

	// Define a 16 bit filter for ID 0x100
	// High order bits of ACC_CODE_ID go into the first 8 bit register
	CANIDAR0 = ACC_CODE_ID100_HIGH;
	CANIDMR0 = MASK_CODE_ST_ID_HIGH;
	// Low order bits of ACC_CODE_ID go into the second 8 bit register
	CANIDAR1 = ACC_CODE_ID100_LOW;
	CANIDMR1 = MASK_CODE_ST_ID_LOW;

	// Exit initialization mode and enter normal mode
	// Send request
	CANCTL0 = NORMAL_MODE;

	while ((CANCTL1 & 0x01)) {
		// Wait for normal mode acknowledgment
		// (was 0x00)
	};
}

// ******************************************************************
//	CANTx(id, priority, length, *txdata)
//  Sends a CAN frame via polling
// ******************************************************************

unsigned char CANTx(unsigned long id, unsigned char priority,
					unsigned char length, unsigned char *txdata)
{
	unsigned char txbuffer;
	unsigned char index;

	// Check the transmit buffer to see if it is full
	if (!CANTFLG)
		return CAN_ERR_BUFFER_FULL;

	// This selects the lowest empty transmit buffer
	CANTBSEL = CANTFLG;
	// Keeps a backup of the selected transmit buffer
	txbuffer = CANTBSEL;

	// Fill in the different parts of the CAN frame
	// Load the ID to the appropriate ID register (e.g. IDR0)
	*((unsigned long *) ((unsigned long)(&CANTXIDR0))) = id;

	// Load the message data to the Tx buffer data segment registers
	for (index = 0; index < length; index++)
		*(&CANTXDSR0 + index) = txdata[index];

	// Set the data length code (DLC)
	CANTXDLR = length;

	// Set priority
	CANTXTBPR = priority;

	// Start transmission
	// Setting the flag bit starts the transmission
	CANTFLG = txbuffer;

	while ((CANTFLG & txbuffer) != txbuffer) {
		//
		// Wait for transmission to complete
	}

	// No error
	return CAN_NO_ERROR;
}

// ******************************************************************
//                        CANRxISR()
//        Interrupt handler for CAN Rx
// Function reads the data buffer and clears the reception flag to keep
// the module available to receive further messages
// ******************************************************************
interrupt VectorNumber_Vcanrx void CANRxISR(void)
{
	unsigned char length, index;

	length = (CANRXDLR & 0x0F);

	for (index = 0; index < length; index++)
		rxdata[index] = *(&CANRXDSR0 + index);   // Get received data

	// Clear the received flag RXF and check for NEW messages
	// -reset the interrupt
	CANRFLG |= 0x01;
	CANTx(ST_ID_100, 0x00, sizeof(rxdata), rxdata);
}
