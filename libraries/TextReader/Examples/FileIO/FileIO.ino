#include <string.h>

#include <SPI.h>
#include <SD_SPI.h>

#include <TextReader.h>
#include <SerialRAM.h>
#include <SerialFRAM.h>

enum PINS {
	CS_SD = 4, CS_FRAM = 8, CS_ETHER = 10,
};

uint8_t CSPins[] = { CS_SD, CS_FRAM, CS_ETHER, 0 };

SDClass SD(CS_SD);
SerialFRAM fram(CS_FRAM);
uint32_t key_count;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);

	for (int i = 0; CSPins[i]; i++) {
		pinMode(CSPins[i], OUTPUT);
		digitalWrite(CSPins[i], HIGH);
	}

	SPI.begin();
	fram.begin();
	if ( !SD.begin(CS_SD) ) {
		Serial.println("micro SD begin failed.");
		while (1)
			;
	}

	char buf[128];
	File file = SD.open("setup.txt");
	if (file) {
		TextReader setupfile(file);
		while (setupfile.available()) {
			setupfile.readToken(buf, 128);
			if ( buf[0] != '$' ) {
				setupfile.readLine();
				continue;
			}
			Serial.print(buf);
			setupfile.readLine(buf, 128);
			Serial.print(buf);
			Serial.println();
		}
		file.close();
	}

	file = SD.open("regkey.txt");
	if (file) {
		TextReader keyfile(file);
		while (keyfile.available()) {
			keyfile.readToken(buf, 128);
			if (strncmp(buf, "$regkey", 7) != 0)
				continue;
			keyfile.readToken(buf, 10);
//			Serial.print(buf);
			keyfile.readToken(buf+10, 118);
//			Serial.print(buf);
			uint32_t date = strtol(buf+10, NULL, 16);
			*((uint32_t*)(buf+10)) = date;
			fram.write(key_count*16, (byte*) &date, 16);
//			Serial.println();
			key_count++;
		}
		file.close();
	}
	
	Serial.print("key count = ");
	Serial.println(key_count);
	
	for(int i = 0; i < key_count; i++) {
		Serial.print(i);
		Serial.print(": ");
		fram.read(key_count*16, (byte*) buf, 16);
		for(int p = 0; p < 10; p++) Serial.print((char)buf[p]);
		Serial.print(" ");
		Serial.print(*((uint32_t*)buf+10));
		Serial.println();
	}
}

void loop() {
	// put your main code here, to run repeatedly: 

}
