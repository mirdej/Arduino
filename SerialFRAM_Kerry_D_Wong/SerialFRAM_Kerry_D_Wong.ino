/*
 * Kerry_D_Wong_FM25C160.cpp
 *
 *  Created on: 2013/05/15
 *      Author: sin
 */

#include <SPI.h>

const byte CMD_WREN = 0x06; //0000 0110 Set Write Enable Latch
const byte CMD_WRDI = 0x04; //0000 0100 Write Disable
const byte CMD_RDSR = 0x05; //0000 0101 Read Status Register
const byte CMD_WRSR = 0x01; //0000 0001 Write Status Register
const byte CMD_READ = 0x03; //0000 0011 Read Memory Data
const byte CMD_WRITE = 0x02; //0000 0010 Write Memory Data

const int FRAM_CS = 8; //chip select 1

int FRAMWrite(int addr, byte *buf, int count = 1);
int FRAMRead(int addr, byte *buf, int count = 1);


void setup() {
	Serial.begin(9600);
Serial.println("Start.");

      for(int i = 3; i <= 10; i++) {
	pinMode(i, OUTPUT);
	digitalWrite(i, HIGH);
      }
	//Setting up the SPI bus
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);

	//Test
	char buf[] = "This is a test";

	FRAMWrite(1, (byte*) buf, 14);
	memset(buf, 0, 14);
	FRAMRead(1, (byte*) buf, 14);

	for (int i = 0; i < 14; i++)
		Serial.print(buf[i]);
}

void loop() {
}

/**
 * Write to FRAM (assuming 2 FM25C160 are used)
 * addr: starting address
 * buf: pointer to data
 * count: data length.
 *        If this parameter is omitted, it is defaulted to one byte.
 * returns: 0 operation is successful
 *          1 address out of range
 */
int FRAMWrite(int addr, byte *buf, int count) {
	const int cs = FRAM_CS;

	byte addrMSB = (addr >> 8) & 0xff;
	byte addrLSB = addr & 0xff;

	digitalWrite(cs, LOW);
	SPI.transfer(CMD_WREN);  //write enable
	digitalWrite(cs, HIGH);

	digitalWrite(cs, LOW);
	SPI.transfer(CMD_WRITE); //write command
	SPI.transfer(addrMSB);
	SPI.transfer(addrLSB);

	for (int i = 0; i < count; i++)
		SPI.transfer(buf[i]);

	digitalWrite(cs, HIGH);

	return 0;
}

/**
 * Read from FRAM (assuming 2 FM25C160 are used)
 * addr: starting address
 * buf: pointer to data
 * count: data length.
 *        If this parameter is omitted, it is defaulted to one byte.
 * returns: 0 operation is successful
 *          1 address out of range
 */
int FRAMRead(int addr, byte *buf, int count) {
	const int cs = FRAM_CS;

	byte addrMSB = (addr >> 8) & 0xff;
	byte addrLSB = addr & 0xff;

	digitalWrite(cs, LOW);

	SPI.transfer(CMD_READ);
	SPI.transfer(addrMSB);
	SPI.transfer(addrLSB);

	for (int i = 0; i < count; i++)
		buf[i] = SPI.transfer(0x00);

	digitalWrite(cs, HIGH);

	return 0;
}

