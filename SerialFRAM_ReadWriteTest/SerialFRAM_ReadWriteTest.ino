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
  Serial.println("Hello.\nType return.");
  while(! Serial.available());
  while ( Serial.available() ) Serial.read();
  
  Serial.println(millis());
  randomSeed(millis());

  SPI.begin();
  fram.begin();
  Serial.println((int)fram.status(), HEX);

  char tmp[] = "This royal throne of kings, this scepter'd isle,\n"
    "This earth of majesty, this seat of Mars,\n"
    "This other Eden, demi-paradise,\n"
    "This fortress built by Nature for herself\n"
    "Against infection and the hand of war,";
  size_t length = strlen(tmp);
  char buf[256];
  uint32_t addr;
  for(int i = 0; i < 10; i++) {
    addr = random(0, 0x7fff);
    Serial.println(addr, HEX);

    strcpy(buf, "This is a pen!");
    Serial.println((int)fram.status(), HEX);
    fram.address(addr);
    fram.print(buf);
    //		fram.read(addr, (byte*) buf, length+1);
    Serial.println(buf);
    Serial.println((int)fram.status(), HEX);
    fram.address(addr);
    fram.write((byte*) tmp, length+1);
    fram.readBytes(buf, length+1);
    Serial.println(buf);
    Serial.println();
    delay(1000);
  }

}

void loop() {
}

