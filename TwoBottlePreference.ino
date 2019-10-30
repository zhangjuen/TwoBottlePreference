#include <Wire.h>
#include "Adafruit_MPR121.h"
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t currtouched = 0;
int MPRtouchPin[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
int TouchMark[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int arduinoValvePin[12] = {22,23,24,25,26,27,28,29,30,31,32,33};
int ValveMode[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long ValveOpenStartTime[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long ValveOpenDuration[12] = {20,20,20,20,20,20,20,20,20,20,20,20};
int inputs = 0;
int i = 0;
boolean SessionStartCheck = 0;
boolean LpumpMode = 0;
unsigned long TouchStartTime[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long timeNow = 0;
unsigned long SessionStartTime = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 
for(i=2;i<=13;i++){  pinMode(i,OUTPUT);  }
for (i = 22;i<=53;i++){  pinMode(i,OUTPUT);  digitalWrite(i,LOW);}
cap.begin(0x5A);
}//setup end

void loop() {
timeNow = millis();
//Serial.println(timeNow);
if (SessionStartCheck){
  currtouched = cap.touched();
  for(i=0;i<=11;i++)
    {if ((currtouched & (1<<MPRtouchPin[i]))&&TouchMark[i]==0) {  Serial.println(timeNow-SessionStartTime);Serial.println(i); TouchMark[i] = 1; ValveMode[i] = 1; ValveOpenStartTime[i] = timeNow; digitalWrite(arduinoValvePin[i],HIGH); }
      else if ((!(currtouched & (1<<MPRtouchPin[i])))&&TouchMark[i]==1) {Serial.println(timeNow-SessionStartTime);Serial.println(i+100); TouchMark[i] = 0;}
    }
  }//start check end
 
  // read serial input
if (Serial.available() > 0 ) 
{
  inputs = Serial.read();  
  if (inputs==0) {  for (i = 22;i<=53;i++)  {digitalWrite(i,LOW); }  }//0, reset
  else if (inputs == 101) {SessionStartCheck=1;}
  else if (inputs == 102) {SessionStartCheck=0;}
  else if (inputs==100) {cap.begin(0x5A);}//reset touch sensor
// all pin Gate mode, on/off mode
  else if (inputs>=22&&inputs<=53) { digitalWrite(inputs,HIGH); }
  else if (inputs-100>=22&&inputs-100<=53) { digitalWrite(inputs-100,LOW); }
    //trigger Mode
  else   {
       for(i=0;i<=12;i++)
        {if (inputs-200 == arduinoValvePin[i]){ ValveMode[i] = 1; ValveOpenStartTime[i] = timeNow; digitalWrite(arduinoValvePin[i],HIGH);break;}
        } }//else end
}//serial read end

//valve Trig end
  for(i=0;i<=12;i++)
  {if (ValveMode[i] == 1&&(timeNow - ValveOpenStartTime[i]>ValveOpenDuration[i])){ ValveMode[i] = 0; digitalWrite(arduinoValvePin[i],LOW);}
  }
}// loop end
