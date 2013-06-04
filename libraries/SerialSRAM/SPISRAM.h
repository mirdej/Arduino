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

struct SPISRAM {
	// Microchip 23LC1024, 23LCV1024, 24K256, etc.
	uint8_t opmode;
	uint8_t rwmode;
	uint32_t address; // rw command, address bit 23 - 16, 15 - 8. 7 - 0
	uint8_t CSpin; // chip select I/O address

	void writeSR(uint8_t m) {
		SPI.transfer(WRSR);
		SPI.transfer(m);
	}

	uint8_t readSR(void) {
		SPI.transfer(RDSR);
		return SPI.transfer(0xaa); // aa dummy
	}
public:

	SPISRAM(uint8_t cs) : CSpin(cs) {
		opmode = SEQ_MODE;
		rwmode = READ;
		address = 0;
	}

	void init() {
		pinMode(CSpin, OUTPUT);
		deselect(); // ensure
	}
	inline void begin() { init(); }

	void access(uint8_t rw, uint32_t addr) {
		rwmode = rw;
		address = addr;
		SPI.transfer(rwmode);
		SPI.transfer(static_cast<uint8_t>(address>>16));
		SPI.transfer(static_cast<uint8_t>(address>>8));
		SPI.transfer(static_cast<uint8_t>(address));
	}

	void OPstatus(uint8_t op) {
		opmode = op;
		writeSR(opmode);
	}

	uint8_t OPstatus() {
		opmode = readSR();
		return opmode;
	}

	void select() {
		digitalWrite(CSpin, LOW);
	}

	void deselect() {
		digitalWrite(CSpin, HIGH);
	}
};

#endif /* SPISRAM_H_ */
