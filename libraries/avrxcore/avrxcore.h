/*
 * avrxcore.h
 *
 *  Created on: 2013/06/02
 *      Author: sin
 */

#ifndef AVRCMX_H_
#define AVRCMX_H_

#include <stdint.h>
#include <Arduino.h>

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;

enum PortPin {
	D4 = 0x29<<8 | 1<<4 ,
	D8 = 0x23<<8 | 1<<2,
	D10 = 0x23<<8 | 1<<4,
	D14 = 0x23<<8 | 1<<0,
	D15 = 0x23<<8 | 1<<1,
};

#define _PINxOFFSET 0
#define _DDRxOFFSET 1
#define _PORTxOFFSET 2

void portMode(const PortPin pins, uint8 mode, uint8 value = LOW);

inline void portBitSet(const PortPin pins) {
	*((volatile uint8 *)((pins>>8) + _PORTxOFFSET)) |= pins & 0xff;
}

inline void portBitClear(const PortPin pins) {
	*((volatile uint8 *)((pins>>8) + _PORTxOFFSET)) &= ~(pins & 0xff);
}


void portWrite(const PortPin pins, uint8 value = HIGH);
uint8 portRead(const PortPin pins);


#endif /* AVRCMX_H_ */
