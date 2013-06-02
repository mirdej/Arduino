/*
 * avrxcore.c
 *
 *  Created on: 2013/06/02
 *      Author: sin
 */

#include "avrxcore.h"

void portMode(const PortPin pins, uint8 mode, uint8 value) {
	if ( mode == OUTPUT ) {
		*((volatile uint8 *)((pins>>8) + _DDRxOFFSET)) |= (pins & 0xff);
	} else {
		// INPUT
		*((volatile uint8 *)((pins>>8) + _DDRxOFFSET)) &= ~(pins & 0xff);
	}
	if ( value == LOW )
		portBitClear(pins);
	else
		portBitSet(pins);
}

void portWrite(const PortPin pins, uint8 value) {
	if ( value == HIGH )
		portBitSet(pins);
	else
		portBitClear(pins);
}

uint8 portRead(const PortPin pins) {
	uint8 value = *((volatile uint8 *)(pins>>8));
	return value & (pins & 0xff);
}
