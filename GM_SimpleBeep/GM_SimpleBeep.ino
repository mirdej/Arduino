
const int BUZZ_PIN = 7;
const int TUBE_PIN = 2;
const int DEFAULTLED_PIN = 13;


long watch, triggered;
long count;

void setup() {
  Serial.begin(9600);
  Serial.println("Hi.");
  
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(TUBE_PIN, INPUT);
  pinMode(DEFAULTLED_PIN, OUTPUT);
  digitalWrite(DEFAULTLED_PIN, LOW);
  
  tone(7, 2000, 200);
  watch = millis();
  count = 0;
}


void loop() {
  if ( digitalRead(TUBE_PIN) == LOW ) {
    triggered = micros();
    digitalWrite(DEFAULTLED_PIN, HIGH);
    tone(BUZZ_PIN, 3300, 20);
    count++;
    while ( triggered + 20 > micros() && digitalRead(TUBE_PIN) == LOW);
  }
  if ( digitalRead(DEFAULTLED_PIN) == HIGH && micros() > triggered + 5000 )
      digitalWrite(DEFAULTLED_PIN, LOW);
  if ( millis() > watch + 59999 ) {
    watch = millis();
    Serial.print(millis()/1000);
    Serial.print(": ");
    Serial.print(count);
    Serial.println(" count per min.");
    count = 0;
  }
}


