#include <Wire.h>
#include "PN532_I2C.h"
#include "ISO14443.h"
#include "TextStream.h"

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

TextStream ser(Serial);

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
<<<<<<< HEAD
    NFC::TypeA,
    NFC::TypeF
=======
    ISO14443::TypeF,
    ISO14443::TypeA,
>>>>>>> 6511b98c82a3f39e5df8d2cb8e7ed22567160b29
  };

  if ( (millis() > lastCardDetect + 1000) and 
    (c = nfc.InAutoPoll(1, 1, polling+1, polling[0])) and
    (c = nfc.getAutoPollResponse((byte*) buff)) ) {
    //mon << mon.printArray(tmp, 8) << mon.endl;
    // NbTg, type1, length1, [Tg, ...]
    card.set(buff[1], buff+3);
    ser << "Detected " << card << ser.endl;
    if ( card != lastcard ) { 
      lastcard = card;
      lastCardDetect = millis();
      if ( card.type == ISO14443::Mifare ) {
        //tone(4, 1800, 100);
        Serial.println("Mifare: ");
          for(int i = 0; i < card.IDLength; i++) {
            ser << (byte) card.ID[i] << ' ';
          }
          ser << ser.endl;
        nfc.targetSet(0x10, card.ID, card.IDLength);
        if ( nfc.mifare_AuthenticateBlock(4, IizukaKey_b) ) {
          Serial.println("Auth Success.");
          nfc.mifare_ReadDataBlock(4, buff);
          for(int i = 0; i < 16; i++) {
            Serial.print(buff[i], HEX);
            Serial.print(' ');
          }
          Serial.println();
          nfc.mifare_ReadDataBlock(5, buff);
          for(int i = 0; i < 16; i++) {
            Serial.print(buff[i], HEX);
            Serial.print(' ');
          }
          Serial.println();
          nfc.mifare_ReadDataBlock(6, buff);
          for(int i = 0; i < 16; i++) {
            Serial.print(buff[i], HEX);
            Serial.print(' ');
          }
          Serial.println();
        } 
        else {
          //tone(4, 1200, 100);
          Serial.println("Failure.");
        }
        Serial.println();
      }
    } 
  }
  else {
    card.clear();
  }

}








