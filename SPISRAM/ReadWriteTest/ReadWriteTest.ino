#include <SPI.h>
#include <SPISRAM.h>

#include "avrxcore.h"

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
const uint8 SRAM_CS = 8;

SPISRAM myRAM(SRAM_CS, SPISRAM::BUS_24BITS); // CS pin
uint8 buffer[256];

void setup() {
  char teststr[] = "This royal throne of kings, this scepter'd isle,\n"
  "This earth of majesty, this seat of Mars,\n"
  "This other Eden, demi-paradise,\n"
  "This fortress built by Nature for herself\n"
  "Against infection and the hand of war,";
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  
  Serial.begin(19200);
  
  
  SPI.begin();
  myRAM.begin();
  
  Serial.println("\nRandom byte read/write...");

  long err = 0;
  long n = strlen(teststr);
  uint32_t addr;
  for(int i=0; i < n; i++){
    addr = random() & 0x1ffff;
    byte val;
    Serial.print( addr, HEX );
    Serial.print( "\t" );
    Serial.print( teststr[i], HEX );
    Serial.print( " " );
    if ( isprint(teststr[i]) )
      Serial.print( (char) teststr[i] );
    Serial.print( ": " );
    myRAM.write(addr, teststr[i]);
    val = myRAM.read(addr);
    Serial.print( val, HEX );
    Serial.print( " " );
    if ( isprint(val) )
      Serial.print( (char) val );
    Serial.println();
    if (val != teststr[i]) err++;
  }
  Serial.print("error count = ");
  Serial.print(err);
  Serial.print(" / ");
  Serial.println(n);
  
  Serial.print("mode = ");
  Serial.println(myRAM.readMode(), HEX);
  Serial.println();

  Serial.println();
  Serial.println("\nRandom sequential read/write...");

  memset(buffer, 0x20, 128);
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
    Serial.print("[");
    Serial.print((byte)buffer[i], HEX);
    Serial.print("] ");
  }
  Serial.println();
}

void loop()
{
}

