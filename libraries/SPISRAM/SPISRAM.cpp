/*
 * SPISRAM.cpp
 *
 *  Created on: 2013/06/04
 *      Author: sin
 */

#include <SPISRAM.h>

void SPISRAM::init() {
	pinMode(CSpin, OUTPUT);
	deselect(); // ensure
	select();
	// set default mode to Sequential mode.
	SPI.transfer(WRSR);
	SPI.transfer(SEQ_MODE);
	deselect();
}


uint8_t SPISRAM::read(const uint32_t addr) {
	uint8_t r;
	select();
	SPI.transfer(READ);
	if ( busWidth > 16 )
		SPI.transfer(addr>>16 & 0xff);
	SPI.transfer(addr>>8 & 0xff);
	SPI.transfer(addr & 0xff);
	r = SPI.transfer(0xff);
	deselect();

	return r;
}

void SPISRAM::write(const uint32_t addr, uint8_t w) {
	select();
	SPI.transfer(WRITE);
	if ( busWidth > 16 )
		SPI.transfer(addr>>16 & 0xff);
	SPI.transfer(addr>>8 & 0xff);
	SPI.transfer(addr & 0xff);
	SPI.transfer(w);
	deselect();
}


size_t SPISRAM::readBytes(const uint32_t addr, uint8_t * data, size_t num) {
	size_t n = num;

	select();
	SPI.transfer(READ);
	if ( busWidth > 16 )
		SPI.transfer(addr>>16 & 0xff);
	SPI.transfer(addr>>8 & 0xff);
	SPI.transfer(addr & 0xff);
	while(num--)
		*data++ = SPI.transfer(0xff);
	deselect();
	return num;
}

size_t SPISRAM::writeBytes(const uint32_t addr, uint8_t * data, size_t num) {
	size_t n = num;
	select();

	SPI.transfer(WRITE);
	if ( busWidth > 16 )
		SPI.transfer(addr>>16 & 0xff);
	SPI.transfer(addr>>8 & 0xff);
	SPI.transfer(addr & 0xff);
	while (num--)
		SPI.transfer(*data++);
	deselect();
}
