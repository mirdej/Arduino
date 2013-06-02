#include "avrxcore.h"

void setup() {
  Serial.begin(19200);
  Serial.println("start");
  
  portMode(D14, OUTPUT);
  
}

void loop() {
  portWrite(D14, HIGH);
  delay(500);
  portWrite(D14, LOW);
  delay(500);
}
