/*
 * avrxcore.h
 *
 *  Created on: 2013/06/02
 *      Author: sin
 */

#ifndef AVRXCORE_H_
#define AVRXCORE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <avr/io.h>

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;

typedef volatile uint8 * PORTREF;

#define PINxOFFSET 0
#define DDRxOFFSET 1
#define PORTxOFFSET 2

enum PortPin {
	PB0 = 0x2301,
	PB1 = 0x2302,
	PB2 = 0x2304,
	PB3 = 0x2308,
	PB4 = 0x2310,
	PB5 = 0x2320,

	PC0 = 0x2601,
	PC1 = 0x2602,
	PC2 = 0x2604,
	PC3 = 0x2608,
	PC4 = 0x2610,
	PC5 = 0x2620,
	PC6 = 0x2640,
	PC7 = 0x2680,

	PD0 = 0x2901,
	PD1 = 0x2902,
	PD2 = 0x2904,
	PD3 = 0x2908,
	PD4 = 0x2910,
	PD5 = 0x2920,
	PD6 = 0x2940,
	PD7 = 0x2980,

};

void portMode(const PortPin pp, uint8 mode, uint8_t updown = 0) {
	if ( mode ) {
		*((PORTREF) (pp>>8) + DDRxOFFSET) |= (pp & 0xff);
	} else {
		*((PORTREF) (pp>>8) + DDRxOFFSET) &= ~(pp & 0xff);
	}
	if ( updown ) {
		*((PORTREF) (pp>>8) + PORTxOFFSET) |= (pp & 0xff);
	} else {
		*((PORTREF) (pp>>8) + PORTxOFFSET) &= ~(pp & 0xff);
	}
}

void portBitSet(const PortPin pp) {
	*((PORTREF)((pp>>8) + PORTxOFFSET)) |= pp & 0xff;
}

void portBitClear(const PortPin pp) {
	*((PORTREF)((pp>>8) + PORTxOFFSET)) &= ~(pp & 0xff);
}

void portBitToggle(const PortPin pp) {
	*((PORTREF)((pp>>8) + PORTxOFFSET)) ^= (pp & 0xff);
}

// returns PIN register value, does not care in-out direction
uint8 portRead(const PortPin pp) {
	uint8 value = *((PORTREF)(pp>>8));
	return value & (uint8)pp;
}

void portWrite(const PortPin pp, uint8 val) {
	if ( val )
		portBitSet(pp);
	else
		portBitClear(pp);
}


#ifdef __cplusplus
}
#endif


#endif /* AVRCMX_H_ */
