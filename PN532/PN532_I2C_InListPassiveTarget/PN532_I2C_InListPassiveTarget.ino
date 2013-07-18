#include <Wire.h>
#include "PN532_I2C.h"
#include "ISO14443.h"

const int IRQ = 2;
const int RST = 0xff;  // Not connected by default on the NFC Shield
// tied with CPU RESET

PN532 nfc(PN532::I2C_ADDRESS, IRQ, RST);
byte buff[80];
uint32_t lastCardDetect = 0;
ISO14443 card, lastcard;

const byte IizukaKey_b[] = {
  0xBB, 0x63, 0x45, 0x74, 0x55, 0x79, 0x4B };
const byte factory_a[] = {
  0xaa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void setup() {
  byte cnt;

  Wire.begin();
  nfc.begin();

  Serial.begin(9600);
  Serial.println("PN532 via I2C Firmware inspection/FeliCa read test.");

  if ( nfc.GetFirmwareVersion() 
    && nfc.getCommandResponse(buff) ) {
    Serial.print("IC version PN53'"); 
    Serial.print((char) buff[0]); 
    Serial.print("', firmware ver. "); 
    Serial.print(buff[1]); 
    Serial.print(" rev. "); 
    Serial.print(buff[2]);
    Serial.print(", supports "); 
    Serial.print((buff[3] & 0b001 ? "Type A, " :""));
    Serial.print((buff[3] & 0b010 ? "Type B, " :""));
    Serial.print((buff[3] & 0b001 ? "ISO 18092 " :""));
    Serial.println(".");
  } 
  else {
    Serial.println("PN532 not found.");
    while (1);
  }

  Serial.println("  SAMConfiguration ");
  if ( nfc.SAMConfiguration() && (0 == nfc.getCommandResponse(buff)) ) {
    Serial.println();
  } 
  else {
    Serial.println(" failed.");
  }
  if ( nfc.GetGeneralStatus() && (cnt = nfc.getCommandResponse(buff)) ) {
    nfc.printHexString(buff, cnt);
    Serial.println("  GetGeneralState >>");
  }
  nfc.PowerDown(0xff);
  Serial.print(nfc.getCommandResponse(buff));
  Serial.println("  PowerDown >>");
}

void loop() {
  byte c;
  byte polling[] = {
    2,
    ISO14443::TypeA,
    ISO14443::TypeF
  };

  Serial.print("InListPassive: ");
  Serial.println(nfc.InListPassiveTarget(1,PN532::Type_GenericPassiveTypeA, buff /*dummy*/, 0) );
  int cnt = nfc.getCommandResponse(buff, 200);
  Serial.print("response: ");
  Serial.println(cnt);
  for(int i = 0; i < cnt; i++) {
    Serial.print(buff[i], HEX);
    Serial.print(' ');
  }
  
  delay(1000);

}


