long Rnumber;
unsigned long timeNow = 0;
unsigned long SessionStartTime = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 
SessionStartTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeNow = millis();
Serial.println(timeNow-SessionStartTime);
Rnumber = random(0,12);
Serial.println(Rnumber);
delay(random(0,10));
  timeNow = millis();
Serial.println(timeNow-SessionStartTime);
Serial.println(Rnumber+100);
delay(random(50,10000));
}
