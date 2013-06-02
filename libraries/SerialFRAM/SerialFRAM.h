/*
 Copyright (c) 2010 by arms22 (arms22 at gmail.com)
 Microchip 23x256 SPI SRAM library for Arduino

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 23K256/640
 modified by Sin

 */

#ifndef SerialFRAM_H
#define SerialFRAM_H

#include <Arduino.h>
#include <SPI.h>
#include <Stream.h>

class SerialFRAM : public Stream {
private:
	const byte _cspin;
	uint32_t _readix, _writeix;

// Op-codes
	enum OPCODE {
		FM25_WREN = B00000110, //Set Write Enable Latch 0000 0110b
		FM25_WRDI = B00000100, //Write Disable 0000 0100b
		FM25_RDSR = B00000101, //Read Status Register 0000 0101b
		FM25_WRSR = B00000001, //Write Status Register 0000 0001b
		FM25_READ = B00000011, //Read Memory Data 0000 0011b
		FM25_WRITE = B00000010, //Write Memory Data 0000 0010b
	};

	enum STATUSBIT {
		FM25_WEL = 1<<1,
		FM25_WPEN = 1<<7,
		FM25_BP1 = 1<<3,
		FM25_BP0 = 1<<2
	};

public:
	static const uint32_t ADDRESS_MAX = 0x8000 - 1;

public:

	SerialFRAM(const byte cs) : _cspin(cs), _readix(0), _writeix(0) {}

	void setSPIMode(void) {
		SPI.setBitOrder(MSBFIRST);
		SPI.setClockDivider(SPI_CLOCK_DIV2);
		SPI.setDataMode(SPI_MODE0);
	}

	void init();
	inline void begin() { init(); }
	byte status();
	void status(byte val);

	int read(const uint32_t & addr);
	size_t readBytes(const uint32_t & addr, char *buffer, size_t length);
	size_t write(const uint32_t & address, byte *buffer, const size_t size);
	size_t write(const uint32_t & address, byte data);

	void address(const uint32_t addr) { _readix = addr; _writeix = addr; }
	int read() { return read(_readix); }
	size_t readBytes(char *buffer, size_t length) { return readBytes(_readix, buffer, length); }
	size_t write(const byte d) { return write(_writeix, d); }
	size_t write(byte * data, size_t n) { return write(_writeix, data, n); }

	virtual void flush() {};
	int available() { return 1; }
	int peek() { int c = read(); _readix--; return c; }

	inline void csLow() { digitalWrite(_cspin, LOW); }
	inline void csHigh() { digitalWrite(_cspin, HIGH); }
	inline void select(void) {
		setSPIMode();
		csLow();
	}
	inline void deselect(void) { csHigh(); }

};

#endif
