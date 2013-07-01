#include <stdio.h>

#include <Wire.h>
#include "CharacterLCD.h"
#include "ST7032i.h"
#include <DS1307.h>

#include <SPI.h>
#include <SCP1000D01.h>
#define SCP_CSPIN 10

#include "Sensirion.h"
#define SHT15_DATAPIN  3
#define SHT15_SCKPIN   4

// -----
const int m1 = 6;
const int m2 = 7;

Sensirion sht15(SHT15_DATAPIN, SHT15_SCKPIN);
int temp, humid;
float dewpt;

LCD_ST7032i i2clcd;
DS1307 i2crtc;
SCP1000 baro = SCP1000(SCP_CSPIN);

// -----

struct {
  float bp;
  int temp, humid;
  float dewpt;
  
  int mode;
  long lastmode;
  
  long lastsense;
  long lastdisplay;
  long lasttime;
} status;

void setup() {

  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  
  Wire.begin();
  i2clcd.begin();                          // Init the display, clears the display 
  i2crtc.begin();
  
  i2clcd.setContrast(42);
  i2clcd.clear();
  i2clcd.print("Hello World!");       // Classic Hello World!
  
  SPI.begin();
  baro.init();
  baro.startup();
  baro.run();

  sht15.measure(&temp, &humid);

  status.lastdisplay = millis();
  status.lastsense = millis();
  status.lastmode = millis();
}


void loop() {
  char buf[20];
    
  if ( millis() - status.lastdisplay >= 100 ) {
    status.lastdisplay = millis();
    i2crtc.updateTime();
    if ( i2crtc.time != status.lasttime ) {
      status.lasttime = i2crtc.time;
      i2crtc.updateTime();
      i2crtc.updateCalendar();
      
      i2clcd.setCursor(0,0);
      i2clcd.print(i2crtc.time>>20&0x03, HEX);
      i2clcd.print(i2crtc.time>>16&0x0f, HEX);
      i2clcd.print(':');
      i2clcd.print(i2crtc.time>>12&0x07, HEX);
      i2clcd.print(i2crtc.time>>8&0x0f, HEX);
      i2clcd.print(':');
      i2clcd.print(i2crtc.time>>4&0x07, HEX);
      i2clcd.print(i2crtc.time&0x0f, HEX);

      i2clcd.print(" ");
      i2clcd.print(i2crtc.cal&0x3f, HEX);
      i2clcd.print('/');
      i2clcd.print(i2crtc.cal>>8&0x1f, HEX);
      i2clcd.print(" ");

      i2clcd.setCursor(0,1);
      i2clcd.print((float)status.temp/100, 1);
      i2clcd.print("C ");
      i2clcd.print((float)status.humid/100, 1);
      i2clcd.print("% ");
    }
  }
  
  if ( millis() - status.lastmode >= 1000 ) {
    status.lastmode = millis();
    status.mode++;
    status.mode %= 60; // period
    switch(status.mode/10) {
      case 0:
      case 1:
        digitalWrite(m1, HIGH);
        digitalWrite(m2, LOW);
        i2clcd.setCursor(15,0);
        i2clcd.print('A');
        break;
      case 3:
      case 4:
        digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        i2clcd.setCursor(15,0);
        i2clcd.print('B');
        break;
      default:
        digitalWrite(m1, LOW);
        digitalWrite(m2, LOW);
        i2clcd.setCursor(15,0);
        i2clcd.print('C');
        break;
    }
  }
  
  if ( millis() - status.lastsense >= 2400 && millis() - status.lastdisplay < 100) {
    status.lastsense = millis();
    status.bp = (float) baro.pressure()/100;
    sht15.measure(&status.temp, &status.humid);
  }

}

