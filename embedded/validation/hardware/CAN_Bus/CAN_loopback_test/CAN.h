// CAN.h Header file containing macros and constants

// Definitions
// Ref: MC9S12C128V1 pg. 291: CANCTL0
// Ref: MC9S12C128V1 pg. 291: CANCTL0
// Ref: MC9S12C128V1 pg. 291: CANCTL1
// Ref: MC9S12C128V1 pg. 291: CANCTL1
// k = 8, SJW = 0
// TSEG1 = 4, TSEG2 = 3
// Defines four 16 bit filters
#define INIT_MODE                   0x01
#define NORMAL_MODE                 0x00
#define CAN_ENABLED_MODE_BUS_CLOCK  0xC0
#define LOOPBACK_MODE_BUS_CLOCK     0xE0
#define BIT_0_125K                  0x07
#define BIT_1_125K                  0x23
#define FOUR_16BIT_FILTERS          0x10
#define CAN_SYNC                    0x10

// Error codes
#define CAN_NO_ERROR                0x00
#define CAN_ERR_BUFFER_FULL         0x01


// Definitions for filters
// Acceptance code definitions
// ID100 (see AN3034 pg 10-11 for explanation)
// Select high order bits then bit shift by 8 bits
#define ACC_CODE_ID100          0x2000
#define ACC_CODE_ID100_HIGH     ((ACC_CODE_ID100 & 0xFF00) >> 8)
#define ACC_CODE_ID100_LOW      (ACC_CODE_ID100 & 0x00FF)

// Mask code definitions
// Last 3 bits are garbage bits (not used, dont care)
#define MASK_CODE_ST_ID 0x0007
#define MASK_CODE_ST_ID_HIGH    ((MASK_CODE_ST_ID & 0xFF00) >> 8)
#define MASK_CODE_ST_ID_LOW     (MASK_CODE_ST_ID & 0x00FF)

// Definition of IDs
// Standard ID 0x100 formatted to be loaded in IDRx registers in Tx buffer
#define ST_ID_100 0x20000000


// Function for initializing the CAN module
void CANInit(void);

// Function for transmitting CAN messages
unsigned char CANTx(unsigned long id, unsigned char priority,
					unsigned char length, unsigned char *txdata);

// Function for receiveing CAN messages via interrupt
void interrupt CANRxISR(void);


