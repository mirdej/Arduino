/*
 Copyright (c) 2010 by arms22 (arms22 at gmail.com)
 Microchip 23x256 SPI SRAM library for Arduino

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 23K256/640
 modified by Sin

 */

#ifndef SERIALSRAM_H
#define SERIALSRAM_H

#include <Arduino.h>
#include <SPI.h>

class SerialSRAM {
private:
	const byte _csPin;
	const byte _buswidth;
//	volatile long addr;
//	byte clock_divider;
//	byte spi_mode;
//	byte status_cache;

// INSTRUCTION SET
	static const byte READ = 0x03; // Read data from memory
	static const byte WRITE = 0x02; // Write data to memory
	// EDIO
	// EQIO
	// RSTIO
	static const byte RDSR = 0x05; // Read Status register
	static const byte WRSR = 0x01; // Write Status register

	void access(const byte mode, const long & address) {
		SPI.transfer(mode);
		if (_buswidth == BUS_24BITS)
			SPI.transfer(address >> 16 & 0xff);
		SPI.transfer(address >> 8 & 0xff);
		SPI.transfer(address & 0xff);
	}

public:
	// MODE REGISTER VALUE
	static const byte BYTE_MODE = 0x00;
	static const byte PAGE_MODE = 0x80;
	static const byte SEQ_MODE = 0x40;

	enum {
//		BUS_WIDTH_23K256 = 16, // 23K256
//		BUS_WIDTH_23K640 = 16,  // 23K640
//		BUS_WIDTH_23LC1024 = 24, // 23A/LC1024
		BUS_16BITS = 16,
		BUS_24BITS = 24
	};

	SerialSRAM(const byte csPin, const uint8_t bus = BUS_24BITS);

	void init();
	inline void begin() {
		init();
	}
	void setSPIMode();

	void writeMode(byte stat) {
		select();
		SPI.transfer(WRSR);
		SPI.transfer(stat);
		deselect();
	}

	byte readMode() {
		select();
		SPI.transfer(RDSR);
		return SPI.transfer(0);
		deselect();
	}

	byte read(const long & address);
	void read(const long & address, byte *buffer, const long & size);
	void write(const long & address, byte data);
	void write(const long & address, byte *buffer, const long & size);
/*
	size_t read(const long & address, byte *buffer, const long & size) {
		for(long i = 0; i < size; i++) {
			*buffer++ = read(address+i);
		}
		return size;
	}
	size_t write(const long & address, byte *buffer, const long & size) {
		for(long i = 0; i < size; i++) {
			write(address+i, *buffer++);
		}
		return size;
	}
*/
	void csLow();
	void csHigh();
	void select(void);
	void deselect(void);
};

#endif
