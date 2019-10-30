#include <Wire.h>
#include <Servo.h>
#include "Adafruit_MPR121.h"
Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t currtouched = 0;
int MPRtouchPin[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
int TouchMark[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int arduinoValvePin[12] = {22,23,24,25,26,27,28,29,30,31,32,33};
int ValveMode[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long ValveOpenStartTime[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int inputs = 0;
int i = 0;
boolean SessionStartCheck = 0;


boolean LpumpMode = 0;

int Mtouch = 0;
int Ltouch = 0;
int Rtouch = 0;
int ServoMoveM = 1;
boolean ServoMoveL = 1;
boolean ServoMoveR = 1;
unsigned long ServoStepTimeM = 100;
unsigned long ServoInitialTimeM = 10;
unsigned long ServoMoveTimeM = 0;
unsigned long ServoStepTimeL = 80;
unsigned long ServoMoveTimeL = 0;
unsigned long ServoStepTimeR = 80;
unsigned long ServoMoveTimeR = 0;
unsigned long LPumpTrigTime = 22;
unsigned long RPumpTrigTime = 22;
unsigned long MPumpTrigTime1 = 20;
unsigned long MPumpTrigTime2 = 30;
unsigned long MPumpTrigTime3 = 20;
unsigned long MPumpTrigTime4 = 20;
unsigned long MPumpTrigTime5 = 20;
unsigned long MPumpTrigTime6 = 20;
unsigned long MPumpTrigTime7 = 20;

unsigned long MPumpStartTime1 = 0;
unsigned long MPumpStartTime2 = 0;
unsigned long MPumpStartTime3 = 0;
unsigned long MPumpStartTime4 = 0;
unsigned long MPumpStartTime5 = 0;
unsigned long MPumpStartTime6 = 0;
unsigned long MPumpStartTime7 = 0;
unsigned long LPumpStartTime = 0;
unsigned long RPumpStartTime = 0;

unsigned long puffTime = 100;
unsigned long timeServoM = 0;
unsigned long timeTrialStart = 0;
unsigned long timeNow = 0;

unsigned long PuffStartTime = 0;
unsigned long LaserOnTime = 0;
unsigned long LaserOffTime = 0;
unsigned long laserPulse = 5;
unsigned long laserOff = 45;
unsigned long PulseDur[4] = {1,2,5,10};
unsigned long Frequency[9] = {1,2,5,10,20,40,50,80,100};

unsigned long LEDOnTime = 0;
unsigned long LEDOffTime = 0;
unsigned long LEDPulse = 50;
unsigned long LEDOff = 50;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 
for(i=2;i<=13;i++)
{
  pinMode(i,OUTPUT);
  }

for (i = 22;i<=53;i++)
{
  {pinMode(i,OUTPUT);
  digitalWrite(i,LOW);}
}

cap.begin(0x5A);

}

void loop() {
  //read Serial input, and set trial parameters
// touch sensor
//delay(100);
//int16_t a = cap.filteredData(2);
//int16_t b = cap.baselineData(2);
//Serial.print(a-b); Serial.print("\t");
//

timeNow = millis();
//Serial.println(timeNow);
currtouched = cap.touched();
for(i=0;i<=11;i++)
{
if ((currtouched & (1<<MPRtouchPin[i]))&&TouchMark[i]==0) {  Serial.println(i); TouchMark[i] = 1;  }
else if ((!(currtouched & (1<<MPRtouchPin[i])))&&TouchMark[i]==1) {Serial.println(i+100); TouchMark[i] = 0;}
}


  
  // read serial input

if (Serial.available() > 0 ) 
{
  inputs = Serial.read();  
  if (inputs==0)
  {
    // all out put set zero    
    for (i = 22;i<=53;i++)  {digitalWrite(i,LOW); }  //0, reset    
   if (inputs==100) {cap.begin(0x5A);}//reset touch sensor
  // tone cue
// all pin Gate mode, on/off mode
if (inputs>=22&&inputs<=53) { digitalWrite(inputs,HIGH); }
else if (inputs-100>=22&&inputs-100<=53) { digitalWrite(inputs-100,LOW); }
//trigger Mode
else
    for(i=0;i<=12;i++)
    {if (inputs-200 == arduinoValvePin[i]){ ValveMode[i] = 1; ValveOpenStartTime[i] = timeNow; digitalWrite(arduinoValvePin[i],HIGH);break;}
    }
    
if (inputs-200 == MpumpPin1) { MpumpMode1 = 1; MPumpStartTime1 = timeNow; digitalWrite(MpumpPin1,HIGH);}
else if (inputs-200 == MpumpPin2) { MpumpMode2 = 1; MPumpStartTime2 = timeNow; digitalWrite(MpumpPin2,HIGH);}
else if (inputs-200 == MpumpPin3) { MpumpMode3 = 1; MPumpStartTime3 = timeNow; digitalWrite(MpumpPin3,HIGH);}
else if (inputs-200 == MpumpPin4) { MpumpMode4 = 1; MPumpStartTime4 = timeNow; digitalWrite(MpumpPin4,HIGH);}
else if (inputs-200 == MpumpPin5) { MpumpMode5 = 1; MPumpStartTime5 = timeNow; digitalWrite(MpumpPin5,HIGH);}
else if (inputs-200 == MpumpPin6) { MpumpMode6 = 1; MPumpStartTime6 = timeNow; digitalWrite(MpumpPin6,HIGH);}
else if (inputs-200 == MpumpPin7) { MpumpMode7 = 1; MPumpStartTime7 = timeNow; digitalWrite(MpumpPin7,HIGH);}
else if (inputs-200 == LpumpPin)  { LpumpMode = 1;  LPumpStartTime = timeNow;  digitalWrite(LpumpPin,HIGH); } //close should be out of serial read loop  //
else if (inputs-200 == RpumpPin)  { RpumpMode = 1;  RPumpStartTime = timeNow;  digitalWrite(RpumpPin,HIGH); } //close should be out of serial read loop



  //air puff
//if (inputs == airPuffPin ) {digitalWrite(airPuffPin,HIGH);}
//else if (inputs-100 == airPuffPin ) {digitalWrite(airPuffPin,LOW);}
if (inputs-200 == airPuffPin) {  airPuffMode = 1;  PuffStartTime = timeNow;  digitalWrite(airPuffPin,HIGH);    }
//close should be out of serial read loop
  //servo
//if (inputs==MportPin)     {digitalWrite(MservoGatePin,HIGH);ServoMoveM = 1; MservoPosOld = JuenSservoM.read(); MservoPosNew = MservoPos[0];ServoMoveTimeM = timeNow;}//JuenSservoM.write(MservoPos[0]);
//else if (inputs-100==MportPin) {digitalWrite(MservoGatePin,HIGH);ServoMoveM = 1; MservoPosOld = JuenSservoM.read(); MservoPosNew = MservoPos[1];ServoMoveTimeM = timeNow;}//JuenSservoM.write(MservoPos[1]);

if (inputs==MportPin)     {digitalWrite(MservoGatePin,HIGH);ServoMoveM = 2;ServoMoveTimeM = millis();JuenSservoM.attach(MportPin);} //initial
else if (inputs-100==MportPin) {digitalWrite(MservoGatePin,HIGH);ServoMoveM = 3;ServoMoveTimeM = millis();JuenSservoM.attach(MportPin);}

if (inputs==LportPin)     {digitalWrite(LservoGatePin,HIGH);ServoMoveL = 1; LservoPosOld = JuenSservoL.read(); LservoPosNew = LservoPos[0];ServoMoveTimeL = timeNow;}//JuenSservoL.write(LservoPos[0]);
else if (inputs-100==LportPin) {digitalWrite(LservoGatePin,HIGH);ServoMoveL = 1; LservoPosOld = JuenSservoL.read(); LservoPosNew = LservoPos[1];ServoMoveTimeL = timeNow;}//JuenSservoL.write(LservoPos[1]);  
if (inputs==RportPin)     {digitalWrite(RservoGatePin,HIGH);ServoMoveR = 1; RservoPosOld = JuenSservoR.read(); RservoPosNew = RservoPos[0];ServoMoveTimeR = timeNow;}//JuenSservoR.write(RservoPos[0]);
else if (inputs-100==RportPin) {digitalWrite(RservoGatePin,HIGH);ServoMoveR = 1; RservoPosOld = JuenSservoR.read(); RservoPosNew = RservoPos[1];ServoMoveTimeR = timeNow;}//JuenSservoR.write(RservoPos[1]);
//suck  
if (inputs==suckPin1||inputs==suckPin2) {digitalWrite(inputs,HIGH);}
else if (inputs-100==suckPin1||inputs-100==suckPin2) {digitalWrite(inputs-100,LOW);}


//laser
if (inputs == laserPin)  { digitalWrite(laserPin,HIGH); }
else if (inputs-200 == laserPin) {LaserOnTime = timeNow; LaserMode = 1; digitalWrite(laserPin,HIGH);LaserOnMode = 1; }//pulse control out of serial input
else if (inputs-100 == laserPin) {LaserMode = 0;digitalWrite(laserPin,LOW);LaserOnMode = 0; LaserOffMode = 0;}
//change laser parameter
else if (inputs-80>=0&&inputs-80<=3){laserPulse = PulseDur[inputs-80];}
else if (inputs-90>=0&&inputs-90<=8) {laserOff = 1000/Frequency[inputs-90]-laserPulse;}

}//serial read end

//pump Trig end
if (MpumpMode1 == 1&&(timeNow - MPumpStartTime1>MPumpTrigTime1)){ MpumpMode1 = 0; digitalWrite(MpumpPin1,LOW);}
if (MpumpMode2 == 1&&(timeNow - MPumpStartTime2>MPumpTrigTime2)){ MpumpMode2 = 0; digitalWrite(MpumpPin2,LOW);}
if (MpumpMode3 == 1&&(timeNow - MPumpStartTime3>MPumpTrigTime3)){ MpumpMode3 = 0; digitalWrite(MpumpPin3,LOW);}
if (MpumpMode4 == 1&&(timeNow - MPumpStartTime4>MPumpTrigTime4)){ MpumpMode4 = 0; digitalWrite(MpumpPin4,LOW);}
if (MpumpMode5 == 1&&(timeNow - MPumpStartTime5>MPumpTrigTime5)){ MpumpMode5 = 0; digitalWrite(MpumpPin5,LOW);}
if (MpumpMode6 == 1&&(timeNow - MPumpStartTime6>MPumpTrigTime6)){ MpumpMode6 = 0; digitalWrite(MpumpPin6,LOW);}
if (MpumpMode7 == 1&&(timeNow - MPumpStartTime7>MPumpTrigTime7)){ MpumpMode7 = 0; digitalWrite(MpumpPin7,LOW);}
if (LpumpMode  == 1&&(timeNow - LPumpStartTime >LPumpTrigTime )){ LpumpMode  = 0; digitalWrite(LpumpPin, LOW);} 
if (RpumpMode  == 1&&(timeNow - RPumpStartTime >RPumpTrigTime )){ RpumpMode  = 0; digitalWrite(RpumpPin, LOW);} 
// air puff trig end
if (airPuffMode==1&&timeNow-PuffStartTime>puffTime)  {airPuffMode = 0;digitalWrite(airPuffPin,LOW);}
//laser control
if (LaserMode==1&&timeNow-LaserOnTime>=laserPulse&&LaserOnMode==1) {digitalWrite(laserPin,LOW); LaserOnMode = 0; LaserOffTime = timeNow; LaserOffMode = 1;  }
else if (LaserMode==1&&timeNow-LaserOffTime>=laserOff&&LaserOffMode==1) {digitalWrite(laserPin,HIGH); LaserOnMode = 1; LaserOnTime = timeNow; LaserOffMode = 0;}


//servo move step
if (ServoMoveM==1){ 
if (MservoPosNew!=MservoPosOld&&timeNow-ServoMoveTimeM>=ServoStepTimeM) {
  if (MservoPosOld==JuenSservoM.read()){
  MservoPosOld=JuenSservoM.read()+abs(MservoPosNew-MservoPosOld)/(MservoPosNew-MservoPosOld);JuenSservoM.write(MservoPosOld);ServoMoveTimeM =  timeNow;}
  } // if new larger than old, old + 1; if new smaller than old, old-1
else if (MservoPosNew==MservoPosOld&&timeNow-ServoMoveTimeM>=ServoStepTimeM) {
  if (MservoPosOld==JuenSservoM.read()){
  ServoMoveM = 0;
  JuenSservoM.detach();
//  digitalWrite(MservoGatePin,LOW);
  }
  }// servo at the target position, stop it

}
else if (ServoMoveM==2&&timeNow-ServoMoveTimeM>=ServoInitialTimeM){ServoMoveM = 1; MservoPosOld = JuenSservoM.read(); MservoPosNew = MservoPos[0];ServoMoveTimeM = timeNow;}//servo on
else if (ServoMoveM==3&&timeNow-ServoMoveTimeM>=ServoInitialTimeM){ServoMoveM = 1; MservoPosOld = JuenSservoM.read(); MservoPosNew = MservoPos[1];ServoMoveTimeM = timeNow;}//servo off


if (ServoMoveL==1){
if (LservoPosNew!=LservoPosOld&&timeNow-ServoMoveTimeL>=ServoStepTimeL) {LservoPosOld=JuenSservoL.read()+abs(LservoPosNew-LservoPosOld)/(LservoPosNew-LservoPosOld);JuenSservoL.write(LservoPosOld);ServoMoveTimeL = timeNow;} 
else if (LservoPosNew==LservoPosOld&&timeNow-ServoMoveTimeL>=ServoStepTimeL) {ServoMoveL = 0;digitalWrite(LservoGatePin,LOW);}
}
if (ServoMoveR==1){
if (RservoPosNew!=RservoPosOld&&timeNow-ServoMoveTimeR>=ServoStepTimeR) {RservoPosOld=JuenSservoR.read()+abs(RservoPosNew-RservoPosOld)/(RservoPosNew-RservoPosOld);JuenSservoR.write(RservoPosOld);ServoMoveTimeR = timeNow;} 
else if (RservoPosNew==RservoPosOld&&timeNow-ServoMoveTimeR>=ServoStepTimeR) {ServoMoveR = 0;digitalWrite(RservoGatePin,LOW);}
}

}// loop end


