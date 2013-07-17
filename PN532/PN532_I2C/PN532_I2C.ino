#include <Wire.h>
#include "PN532_I2C.h"

const int IRQ = 7;
const int RST = 0xff;  // Not connected by default on the NFC Shield
// tied with CPU RESET

PN532 nfc(PN532::I2C_ADDRESS, IRQ, 0xff);
byte respbuff[80];

void setup() {
  byte cnt;

  Wire.begin();
  nfc.begin();

  Serial.begin(9600);
  Serial.println("PN532 via I2C Firmware inspection/FeliCa read test.");
  
  if ( nfc.GetFirmwareVersion() 
      && nfc.getCommandResponse(respbuff) ) {
  Serial.print("IC version PN53'"); 
  Serial.print((char) respbuff[0]); 
  Serial.print("', firmware ver. "); 
  Serial.print(respbuff[1]); 
  Serial.print(" rev. "); 
  Serial.print(respbuff[2]);
  Serial.print(", supports "); 
  Serial.print((respbuff[3] & 0b001 ? "Type A, " :""));
  Serial.print((respbuff[3] & 0b010 ? "Type B, " :""));
  Serial.print((respbuff[3] & 0b001 ? "ISO 18092 " :""));
  Serial.println(".");
      }

  Serial.println("  SAMConfiguration ");
  if ( nfc.SAMConfiguration() && (0 == nfc.getCommandResponse(respbuff)) ) {
    Serial.println();
  } else {
    Serial.println(" failed.");
  }
  if ( nfc.GetGeneralStatus() && (cnt = nfc.getCommandResponse(respbuff)) ) {
    nfc.printHexString(respbuff, cnt);
    Serial.println("  GetGeneralState >>");
  }
  nfc.PowerDown(0xff);
  Serial.print(nfc.getCommandResponse(respbuff));
  Serial.println("  PowerDown >>");
}

void loop() {
  byte cnt;
  byte payload[] = {
    0x00, 0xff, 0xff, 0x01, 0x00    };
    
  *((word*) &payload[1]) = (word) 0x00fe;
  if ( nfc.InListPassiveTarget(1,PN532::Type_GenericPassive212kbFeliCa, payload, 5) ) {
    cnt = nfc.getCommandResponse(respbuff);
    if ( cnt)
      nfc.targetSet(PN532::Type_FeliCa212kb, respbuff+4, 8);
    word ver = nfc.felica_RequestService(0x1a8b);
    if ( ver != 0xffff ) {
      word blklist[] = {
        0x00      };
      cnt = nfc.felica_ReadWithoutEncryption(respbuff, (word)0x1a8b, 1, blklist );
      if ( cnt != 0 ) {
        nfc.printHexString(respbuff, cnt*16);
        Serial.println("  >> R w/o Enc");
      }
    }
  }
  delay(1000);
}







