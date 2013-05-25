/*
 * SerialRAM_Test.cpp
 *
 *  Created on: 2013/05/14
 *      Author: sin
 */

#include <string.h>
#include <SPI.h>
#include <SerialFRAM.h>

SerialFRAM fram(8);

void setup() {
	Serial.begin(9600);
	Serial.println("Hello.");
	delay(200);

	Serial.println(millis());
	randomSeed(millis());

	SPI.begin();
	fram.begin();
	Serial.println((int)fram.status(), HEX);

	char tmp[] = "No! someday, my prince will come, some day we'll meet again;";
	size_t length = strlen(tmp);
	char buf[128];
	uint32_t addr;
	for(int i = 0; i < 10; i++) {
		addr = random(0, 0x7fff);
		Serial.println(addr, HEX);

		strcpy(buf, "This is a pen!");
		Serial.println((int)fram.status(), HEX);
		fram.write(addr, (byte*) buf, length+1);
//		fram.read(addr, (byte*) buf, length+1);
		Serial.println(buf);
		Serial.println((int)fram.status(), HEX);
		fram.write(addr, (byte*) tmp, length+1);
		fram.read(addr, (byte*) buf, length+1);
		Serial.println(buf);
		Serial.println();
		delay(1000);
	}

}

void loop() {
}
