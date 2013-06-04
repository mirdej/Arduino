#include <SPI.h>
#include <SPISRAM.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Hi.");
  
  SPISRAM ram(10);

Serial.println(ram.CSpin);
Serial.println(ram.address, HEX);
Serial.println(ram.rwmode, HEX);

Serial.println();
for(int i = 0; i < sizeof(SPISRAM); i++) {
  Serial.print( ((byte*)&ram)[i], HEX );
  Serial.print(", ");
}
Serial.println();
}
void loop() {
}

