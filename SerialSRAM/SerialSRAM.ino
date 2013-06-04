#include <SPI.h>
 
//SRAM opcodes
#define RDSR 5
#define WRSR 1
#define READ 3
#define WRITE 2
 
uint8_t SpiRAMRead8(uint16_t address) {
  uint8_t read_byte;
 
  PORTB &= ~(1<<PORTB2); //set SPI_SS low
  SPI.transfer(READ);
  SPI.transfer((char)(address >> 16));
  SPI.transfer((char)(address >> 8));
  SPI.transfer((char)address);
  read_byte = SPI.transfer(0xFF);
  PORTB |= (1<<PORTB2); //set SPI_SS high
  return read_byte;
}
 
void SpiRAMWrite8(uint16_t address, uint8_t data_byte) {
  PORTB &= ~(1<<PORTB2); //set SPI_SS low
  SPI.transfer(WRITE);
  SPI.transfer((char)(address >> 16));
  SPI.transfer((char)(address >> 8));
  SPI.transfer((char)address);
  SPI.transfer(data_byte);
  PORTB |= (1<<PORTB2); //set SPI_SS high
}
 
 char test[] = "This royal throne of kings, this sceptre'd isle,\n"
"This earth of majesty, this seat of Mars,\n"
"This other Eden, demi-paradise;\n"
"This fortress, built by nature for herself,\n"
"Against infection, and the hand of war;";

void setup(void) {
  uint16_t addr;
  uint8_t i;
 
  Serial.begin(9600);
  Serial.println("\n\nstarts.\n");
  SPI.begin();

  randomSeed(millis());
  long address = random(0, 0x1ffff);
  Serial.print("address ");
  Serial.println(address, HEX);
  for (addr=0; addr<128; addr++) {
    SpiRAMWrite8(address+addr, (uint8_t)test[addr]);
  }
  for (addr=0; addr<128; addr++) {
    i = SpiRAMRead8(address+addr);
    Serial.print((char)i);
  }
  Serial.println();
}
 
void loop() {
}
