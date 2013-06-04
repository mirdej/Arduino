#include <string.h>
#include <SPI.h>
#include <SerialSRAM.h>
//#include "avrxcore.h"

/*
 SRAM   Arduino
 1 CS   10(CS)
 2 SO   12(MISO)
 3 -    -
 4 Vss  GND
 5 SI   11(MOSI)
 6 SCK  13(SCK)
 7 HOLD <-- 100k ohm -- 3.3V
 8 Vcc  3.3V
 */

SerialSRAM myRAM(8); // CS pin
uint8_t buffer[256];

void setup() {
  char teststr[] = "This royal throne of kings, this scepter'd isle,\n"
  "This earth of majesty, this seat of Mars,"
  "This other Eden, demi-paradise,\n"
  "This fortress built by Nature for herself\n"
  "Against infection and the hand of war,";
  
  int cs[] = { 4, 7, 10};
  for(int i = 0; i < 3; i++) {
    pinMode(cs[i], OUTPUT);
    digitalWrite(cs[i], HIGH);
  }
  
  Serial.begin(9600);
  
  
  SPI.begin();
  myRAM.begin();
  
 // myRAM.writeMode(myRAM.SEQ_MODE);
//  Serial.println((myRAM.readMode() == myRAM.BYTE_MODE ? "byte mode" : "sequential mode"));
  
  Serial.println("\nRandom byte read/write...");

  long err = 0;
  long n = strlen(teststr);
  uint32_t addr = 0;
//  myRAM.write(addr, teststr[0]);
//  myRAM.read(addr);
  for(int i=0; i < n; i++){
    addr = random() & 0x1ffff;
    byte val;
    Serial.print( addr, HEX );
    Serial.print( "\t" );
    Serial.print( teststr[i], HEX );
    Serial.print( ": " );
    myRAM.write(addr, teststr[i]);
    Serial.print( myRAM.read(addr), HEX );
    Serial.println();
    if (myRAM.read(addr) != teststr[i]) err++;
  }
  Serial.print("error count = ");
  Serial.print(err);
  Serial.print(" / ");
  Serial.println(n);

  Serial.println();
  Serial.println("\nRandom sequential read/write...");

  memset(buffer, 0xff, 128);
  addr = random() & 0x1ffff;
  Serial.print("ADDRESS ");
  Serial.println(addr, HEX);
  
  myRAM.write(addr, (byte*)teststr, 128);
  myRAM.read(addr, (byte*)buffer, 128);

  Serial.println("Original: ");
  for(int i = 0; i < 128; i++) {
    Serial.print((char)teststr[i]);
  }
  Serial.println();
  Serial.println("Read after write: ");
  for(int i = 0; i < 128; i++) {
    Serial.print((char) buffer[i]);
  }
  Serial.println();
}

void loop()
{
}

