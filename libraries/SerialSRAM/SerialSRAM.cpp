/*
 *
 */

#include <Arduino.h>
#include <SPI.h>

#include "SerialSRAM.h"

SerialSRAM::SerialSRAM(const byte csPin, const byte buswidth) :
		_csPin(csPin), _buswidth(buswidth) {
	//addr = 0;
}

void SerialSRAM::init() {
	pinMode(_csPin, OUTPUT);
	csHigh();
	writeMode(SEQ_MODE);
}

byte SerialSRAM::read(const long & address) {
	byte data;
	select();
	access(READ, address);
	data = SPI.transfer(0);
	deselect();
	return data;
}

void SerialSRAM::write(const long & address, byte data) {
	select();
	access(WRITE, address);
	SPI.transfer(data);
	deselect();
}

void SerialSRAM::read(const long & address, byte *buffer, const long & size) {
	select();
	access(READ, address);
	for (unsigned int i = 0; i < size; i++)
		*buffer++ = SPI.transfer(0);
	deselect();
}

void SerialSRAM::write(const long & address, byte *buffer, const long & size) {
	select();
	access(WRITE, address);
	for (unsigned int i = 0; i < size; i++)
		SPI.transfer(*buffer++);
	deselect();
}

void SerialSRAM::setSPIMode(void) {
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);
}

void SerialSRAM::csLow() {
	digitalWrite(_csPin, LOW);
}

void SerialSRAM::csHigh() {
	digitalWrite(_csPin, HIGH);
}

void SerialSRAM::select(void) {
	setSPIMode();
	csLow();
}

void SerialSRAM::deselect(void) {
	csHigh();
}

