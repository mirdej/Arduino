#include <SPI.h>
#include <SPISRAM.h>

SPISRAM ram(10);

void setup() {
   char test[] = "This royal throne of kings, this sceptre'd isle, "
"This earth of majesty, this seat of Mars, "
"This other Eden, demi-paradise; "
"This fortress, built by nature for herself, "
"Against infection, and the hand of war; "
"This happy breed of men, this little world, "
"This precious stone set in the silver sea, "
"Which serves it in the office of a wall "
"Or as a moat defensive to a house, "
"Against the envy of less happier lands,-- "
"This blessed plot, this earth, this realm, this England.";
  
  int pullups[] = { 4, 7, 8, 10};
  for(int i = 0; i < sizeof(pullups); i++) {
    digitalWrite(i, HIGH);
  }
  
  Serial.begin(9600);
  Serial.println("Hi.");

  Serial.println();
  
  SPI.begin();
  ram.begin();
  
  randomSeed(millis());
  long addr = random(0, 0x1ffff);
  Serial.print("Byte write/read start address: ");
  Serial.println(addr, HEX);
  for (int i = 0; i < strlen(test); i++) {
    ram.write(addr+i, test[i]);
  }
  for (int i = 0; i < strlen(test); i++) {
    Serial.print((char) ram.read(addr+i));
  }
  
  char buf[264];
  addr = random(0, 0x1ffff);
  Serial.println();
  Serial.print("Continuous write/read start address: ");
  Serial.println(addr, HEX);
  ram.writeBytes(addr, (uint8_t*)test, 260);
  ram.readBytes(addr, (uint8_t*)buf, 260);
  buf[260] = 0;
  Serial.print(buf);
  
}

void loop() {
}


