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
 
 char test[] = "This royal throne of kings, this sceptred isle,"
"This earth of majesty, this seat of Mars,"
"This other Eden, demi-paradise;"
"This fortress, built by nature for herself,"
"Against infection, and the hand of war;";

void setup(void) {
  uint16_t addr;
  uint8_t i;
 
  Serial.begin(9600);
  SPI.begin();
  for (addr=0; addr<64; addr++) {
    SpiRAMWrite8(addr, (uint8_t)test[addr]);
    Serial.print("Addr: ");
    Serial.print(addr);
    i = SpiRAMRead8(addr);
    Serial.print(" | Read: ");
    Serial.println((char)i);
  }
}
 
void loop() {
}
