#include <SPI.h>

#include <SD_SPI.h>

SDClass SD(4);

struct {
  boolean sd;
} status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  SPI.begin();
  status.sd = SD.begin(4);
  if ( status.sd) {
    File file = SD.open("setup.txt");
    while (file.available()) {
      Serial.print((char)file.read());
    }
    file.close();
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
