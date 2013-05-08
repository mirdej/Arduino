
const int BUZZ_PIN = 7;
const int TUBE_PIN = 2;
const int DEFAULTLED_PIN = 13;


long watch, secs, triggered;
long count[12];

void setup() {
  Serial.begin(9600);
  Serial.println(0);
  
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(TUBE_PIN, INPUT);
  pinMode(DEFAULTLED_PIN, OUTPUT);
  digitalWrite(DEFAULTLED_PIN, LOW);
  
  for(int i = 0; i < 12; i++) {
    count[i] = 0;
  }
  tone(7, 2000, 200);
  watch = millis();
  secs = millis()/1000;
}


void loop() {
  if ( digitalRead(TUBE_PIN) == LOW ) {
    triggered = micros();
    digitalWrite(DEFAULTLED_PIN, HIGH);
    tone(BUZZ_PIN, 3300, 20);
    count[millis()/1000 / 5 % 12]++;
    while ( triggered + 20 > micros() && digitalRead(TUBE_PIN) == LOW);
  }
  if ( digitalRead(DEFAULTLED_PIN) == HIGH && micros() > triggered + 5000 )
      digitalWrite(DEFAULTLED_PIN, LOW);
  if (secs != millis()/1000) {
    if ( secs / 5 %12 == 0 ) {
      long sum =0;
      for(int i = 0; i < 12; i++) {
        sum += count[i];
      }
      Serial.println(sum);
      count[secs/5 % 12] = 0;
    }
    secs = millis()/1000;
  }
}


