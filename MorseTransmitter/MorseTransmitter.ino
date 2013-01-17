/*
 MorseCodeTransmitter
 */

char codetable[][8] = {
  "0", // ' '
  "313133", //'!'
  "131131",
  "",
  "1113113", //$
  "",
  "13111", //&
  "133331",
  "31331",
  "313313",
  "",
  "13131", //+
  "331133",
  "311113", // -
  "131313", // .
  "31131", 

  "33333", //0  
  "13333", // 1
  "11333",
  "11133",
  "11113",
  "11111", //5
  "31111",
  "33111",
  "33311",
  "33331", // 9
  "333111", //:
  "313131", // ;
  "", // <
  "31114", // =
  "",
  "113311", // ?
  "133131", // @
  "13",   //A
  "3111",
  "3131",
  "311",   // D
  "1",     // E
  "1131",
  "331",
  "1111",
  "11",     // I
  "1333",   // J
  "313",    // K
  "1311",   // L
  "33",
  "31",
  "333",   // O
  "1331",
  "3313",
  "131",
  "111",   // S
  "3",     // T
  "113",
  "1113",
  "133",
  "3113",
  "3133",
  "3311", // Z
  "",
  "",
  "",
  "",
  "113313" // _
};

int LED13 = 13;
int PIEZZO = 7;
long duration1dot = 60; // in millisecs.

void signal(int length) {
  if ( length ) {
    digitalWrite(LED13, HIGH);
    tone(PIEZZO, 3100);
    delay(length * duration1dot);
    digitalWrite(LED13, LOW);
    noTone(PIEZZO);
  }
  delay(duration1dot);
}

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards.
  // Connect Piezzo Speaker between Pin 7 and GND,
  pinMode(LED13, OUTPUT);
  pinMode(PIEZZO, OUTPUT);

  Serial.begin(9600);
  Serial.println("Type and send message.");
}

void loop() {
  int codenum;
  int i;
  char rx;
  while ( Serial.available() ) {
    rx = toupper((char) Serial.read());
    if (rx == '\n') {
      Serial.println();
      continue;
    } else if ( rx < ' ' || rx > '_' ) // ignore
      continue;
    codenum = rx - ' ';
    for ( i = 0; codetable[codenum][i] != 0 ; i++) {
      switch(codetable[codenum][i]) {
      case '1':
        signal(1);
        Serial.print('.');
        break;
      case '3':
        signal(3);
        Serial.print('-');
        break;
      case '0': // pause
        signal(0);
        break;
      }
    }
    Serial.print(' ');
  }
}

/*
  SOS SOS SOS de GBTT GBTT GBTT Queen Elizabeth 2 psn 49.06.30 North, 04.30.20 West. Ship on fire, crew abandoning ship AR K.
*/

