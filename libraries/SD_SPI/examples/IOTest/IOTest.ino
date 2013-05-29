#include <string.h>

#include <SPI.h>
#include <SD_SPI.h>
#include <SerialRAM.h>
#include <TextStream.h>

#include "Wire.h"
#include "PN532_I2C.h"
#include "M41T62.h"
#include "KeyID.h"

enum PINS {
	CS_SD = 4, CS_FRAM = 8, CS_ETHER = 10, IRQ_PN532 = 7,
};

// global vars
uint8_t CSPins[] = { CS_SD, CS_FRAM, CS_ETHER, 0 };
uint8_t address[] = {127, 0, 0, 1};
uint8_t interface[] = {0xDE,0xAD,0xBE,0xEF,0xFE,0xED};
uint32_t key_count = 0;
const int KeyIDsize = sizeof(KeyID);

SDClass SD(CS_SD);
SerialRAM fram(CS_FRAM);

//void twi_setup();
PN532 nfc(PN532::I2C_ADDRESS, IRQ_PN532);
M41T62 rtc;

TextStream serout(Serial);

void setup() {
	TextStream reader;
	File file;
	char buf[128];
	uint32_t swatch;
	KeyID key;

	// put your setup code here, to run once:
	Serial.begin(115200);

	for (int i = 0; CSPins[i]; i++) {
		pinMode(CSPins[i], OUTPUT);
		digitalWrite(CSPins[i], HIGH);
	}

	//TWI
	Wire.begin();
	rtc.begin();
	rtc.update();
	serout << "RTC time ";
	serout.print(rtc.time, HEX);
	serout << ", ";
	serout.println(0x20000000UL + rtc.cal, HEX);
	
	nfc.begin();
    Serial.print("PN53x "); 
	if ( nfc.GetFirmwareVersion() && nfc.getCommandResponse((byte*)buf) ) {
		serout << (char) buf[0] << ", firmware " 
				<< int(buf[1]) << '.' << int(buf[2]) 
				<< " /w types " << int(buf[3]) << TextStream::endl;
		nfc.SAMConfiguration();
		if ( (nfc.getCommandResponse((byte*) buf) == 0) and 
				(nfc.status() == nfc.RESP_RECEIVED) )
			serout << "SAM in normal mode, use P70_IRQ." << serout.endl;
		nfc.PowerDown(0xff);
	} else {
		Serial.println(" failed.");
	}
	
	// SPI peripherals
	SPI.begin();
	fram.begin();
	Serial.print("FMRAM status ");
	Serial.print(fram.status(), BIN);
	fram.write((uint32_t)0, 0xff);
	uint8_t d = fram.read(0);
	if ( d == 0xff ) {
		Serial.print(" capacity ");
		Serial.println(SerialRAM::ADDRESS_MAX/KeyIDsize);
	}

	Serial.print("Reading SD Card...");
	if ( !SD.begin() ) {
		Serial.println(" failed.");
		while (1)
			;
	} else {
		file = SD.open("setup.txt");
		if (file) {
			reader.resetTo(file);
			while (reader.available()) {
				reader.getToken(buf, 127);
				if ( buf[0] != '$' ) {
					reader.skipLine();
					continue;
				}
				if ( strcmp(buf, "$address") == 0) {
					for(int i = 0; i < 4; i++) {
						address[i] = reader.parseInt();
						Serial.print((int)address[i]);
						Serial.print(".");
					}
				} else if ( strcmp(buf, "$interface") == 0) {
					for(int i = 0; i < 6; i++) {
						interface[i] = reader.parseHex();
						Serial.print((int)interface[i], HEX);
						Serial.print(":");
					}
				}
				Serial.print(", ");
			}
			file.close();
		} else {
			Serial.println("setup open failed.");
		}
		Serial.println();
				
		file = SD.open("regkey.txt");
		if (file) {
			Serial.print("registering key...");
			reader.resetTo(file);
			key_count = 0;
			swatch = millis();
			while (reader.available()) {
				reader.getToken(buf, 127);
				if (strncmp(buf, "$regkey", 7) != 0)
					continue;
				reader.getToken(buf, 16);
				reader.getToken(buf+10,16);
				buf[14] = 0;
				key.set(buf);
				fram.write(key_count*KeyIDsize, (byte*) &key, KeyIDsize);
				key_count++;
			}
			Serial.print(millis() - swatch);
			Serial.println(" msec.");
			file.close();
		} else {
			Serial.println(" file open failed.");
		}
	}

	Serial.println("Ready.");
}

void loop() {
	// put your main code here, to run repeatedly: 
	char buf[32];
	KeyID key;
	uint32_t swatch;

	int i = 0;
	if ( Serial.available() ) 
		i = Serial.readBytesUntil('\n', buf, 16);
	buf[i] = 0;
	switch(buf[0]) {
	case 0:
		break;
	case 'S':
		Serial.print("scanning ");
		Serial.print('"'); Serial.print(buf); Serial.print('"'); 
		Serial.println("...");

		swatch = millis();
		for(int i = 0; i < key_count; i++) {
			fram.read(i*KeyIDsize, (byte*) &key, KeyIDsize);
			if ( key.idmatch(buf) ) {
				Serial << i << ": " << key << TextStream::endl;
			}
		}
		Serial.print(millis() - swatch);
		Serial.println(" msec.");
	break;
	case 'l':
		for(int i = 0; i < key_count; i++) {
			fram.read(i*KeyIDsize, (byte*) &key, KeyIDsize);
			Serial.print(i);
			Serial.print(": ");
			Serial.print(key);
			Serial.println();
		}
		Serial.print("Keys totally ");
		Serial.println(key_count);
		break;
	default:
		Serial.println("?");
		break;
	}
}


/*
void sortKeyIDs(SerialRAM & ram, uint16_t begin, uint16_t end) {
	KeyID preckey, suckey, swap;
	uint16_t sortedlast, scanhead;

	for (sortedlast = begin; sortedlast < end; sortedlast++) {
		ram.read(sortedlast*KeyID::size, (byte*)&preckey, KeyID::size);
e
		for(scanhead = sortedlast+1; scanhead < end; scanhead++) {
			ram.read(scanhead*KeyID::size, (byte*)&suckey, KeyID::size);

			if ( suckey < preckey ) {
				swap = suckey;
				suckey = preckey;
				preckey = swap;
				ram.write(scanhead*KeyID::size, (byte*)&suckey, KeyID::size);
			}
		}
		ram.write(sortedlast*KeyID::size, (byte*)&preckey, KeyID::size);
//		Serial.println();
	}

}
*/