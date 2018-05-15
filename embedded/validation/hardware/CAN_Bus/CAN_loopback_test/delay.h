#include <hidef.h>
#include "derivative.h"

void msDelay(unsigned int delay_time)
{
	TSCR1 = 0x90;
	TSCR2 = 0x01;
	TIOS |= 0x01;
	TC0 = TCNT + delay_time * 12;
	while (!(TFLG1 & 0x01)) {
		//wait for flags
		//
	}
}
