/*
 * SPISRAM.h
 *
 *  Created on: 2013/06/04
 *      Author: sin
 */

#ifndef SPISRAM_H_
#define SPISRAM_H_

#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>

// COMMAND VALUE
enum {
	READ = 0x03, // Read data from memory
	WRITE = 0x02, // Write data to memory
	// EDIO, EQIO, RSTIO
	RDSR = 0x05, // Read Status register
	WRSR = 0x01, // Write Status register
};

// MODE REGISTER VALUE
enum {
	BYTE_MODE = 0x00, PAGE_MODE = 0x80, SEQ_MODE = 0x40,
};

// ADDRESS BUS WIDTH ( /8 = BYTES)
enum {
	BUSWIDTH_23K256 = 16, // 23K256
	BUSWIDTH_23K640 = 16,  // 23K640
	BUSWIDTH_23LC1024 = 24, // 23A/LC1024
	BUSWIDTH_16BITS = 16,
	BUSWIDTH_24BITS = 24
};

class SPISRAM {
	// Microchip 23LC1024, 23LCV1024, 24K256, etc.
	uint8_t CSpin; // chip select I/O address
	uint8_t busWidth;
//
//	volatile uint8_t *out;
//	uint8_t bit;

public:

	SPISRAM(uint8_t cs, uint8_t bw = BUSWIDTH_24BITS) : CSpin(cs), busWidth(bw) {
//		out = &PORTB;
//		bit = 1<<0;
	}

	void init();
	inline void begin() { init(); }

	uint8_t read(const uint32_t addr) ;
	void write(const uint32_t addr, uint8_t w) ;

	size_t readBytes(const uint32_t addr, uint8_t * data, size_t num);
	size_t writeBytes(const uint32_t addr, uint8_t * data, size_t num);

/*
	uint8_t OPstatus() {
		select();
		SPI.transfer(RDSR);
		opmode = SPI.transfer(0);
		deselect();
		return opmode;
	}
*/
	void select() {
		SPI.setDataMode(SPI_MODE0);
		SPI.setBitOrder(MSBFIRST);
		SPI.setClockDivider(SPI_CLOCK_DIV2);
		digitalWrite(CSpin, LOW);
	//	*out &= ~bit;
	}

	void deselect() {
		digitalWrite(CSpin, HIGH);
	//	*out |= bit;
	}
};

#endif /* SPISRAM_H_ */
