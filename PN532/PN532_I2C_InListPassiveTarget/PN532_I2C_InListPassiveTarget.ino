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
    Serial.print("', Firmware "); 
    Serial.print(buff[1]); 
    Serial.print(" rev "); 
    Serial.print(buff[2]);
    Serial.print(", support types "); 
    Serial.print((buff[3] & 0b001 ? "Type A, " :""));
    Serial.print((buff[3] & 0b010 ? "Type B, " :""));
    Serial.print((buff[3] & 0b001 ? "ISO 18092 " :""));
    Serial.println(".");
  } 
  else {
    Serial.println("PN532 not found. Stop. ");
    while (1);
  }

  Serial.println("SAMConfiguration ");
  if ( nfc.SAMConfiguration() && (0 == nfc.getCommandResponse(buff)) ) {
    Serial.println(" succeeded.");
  } 
  else {
    Serial.println(" failed.");
  }
  /*
  if ( nfc.GetGeneralStatus() && (cnt = nfc.getCommandResponse(buff)) ) {
    nfc.printHexString(buff, cnt);
    Serial.println("  GetGeneralState >>");
  }
  */
  Serial.println("  Going to PowerDown >>");
  nfc.PowerDown(0xff);
  Serial.print(nfc.getCommandResponse(buff));
}

void loop() {
  int cnt;
  boolean detected = false;
  byte polling[] = {
    2,
    NFC::BaudrateType_106kbitTypeA,
    NFC::BaudrateType_212kbitFeliCa
  };


  card = lastcard;
  if (millis() > lastCardDetect + 1000) {
    if ( nfc.InAutoPoll(1, 1, polling+1, polling[0]) and 
      (cnt = nfc.getAutoPollResponse((byte*) buff) ) ) {
      card.setPassiveTarget(buff);
      Serial.print("InAutoPoll deteced: ");
      /*
      for(int i = 0; i < cnt; i++) {
        Serial.print(buff[i], HEX);
        Serial.print(' ');
      }
      Serial.println();
      Serial.println(cnt);
      */
      Serial.println(card);
      lastCardDetect = millis();
    } else {
      delay(500);
    }
  }
  
  if ( card != lastcard ) {
    lastcard = card;
    
    if ( card.atqa == NFC::ATQA_MIFARE_ULTRALIGHT ) {
      Serial.println("Block ");
      cnt = nfc.mifare_ReadDataBlock(8, buff);
      for(int i = 0; i < cnt; i++) {
        Serial.print(buff[i], HEX);
        Serial.print(' ');
      }
      Serial.println();
      Serial.println(cnt);
    }
    delay(1000);
  }
}


