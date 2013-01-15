/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

int theta = 0;
int period = 3.141592 * 2 * 400;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  
  Serial.begin(19200);
  Serial.println(period);
}

// the loop routine runs over and over again forever:
void loop() {
  float scale = (1.0 + sin( (float)(theta % period)/400 ))/2;
  int duration = 1600*scale;
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(duration + 2);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delayMicroseconds(1600 - duration + 2);               // wait for a second
  //delayMicroseconds(500);
  theta++;
  
}
