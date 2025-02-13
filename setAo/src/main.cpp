#include "Arduino.h"
#include <Servo.h>
#define ESC 7
#define PUL 10
#define DIR 11
#define CTHT2 3  
#define CTHT1 25 
#define MF 12    
int Ao = 0; 
Servo esc; 

void findLimitSwitch();
void goToHome();
void stepMotorX();
void moveMotor1();
void moveMotor2();
void phunThucAn();

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MF, OUTPUT);
  pinMode(CTHT2, INPUT_PULLUP);  
  pinMode(CTHT1, INPUT_PULLUP);
  digitalWrite(MF, LOW);
  Serial.begin(9600);
  esc.attach(ESC); 
  findLimitSwitch(); 
  goToHome();
   esc.writeMicroseconds(1000);
  delay(2000);     
  
  Ao = 2; 
}

void loop() {
  if (Ao == 1) {
    moveMotor1();
  }
  if(Ao == 2) {
    moveMotor2();
  }
}

void findLimitSwitch() {
  digitalWrite(DIR, HIGH);  
  
  while (digitalRead(CTHT2) != LOW) {
    stepMotorX();
  }
}

void goToHome() {
  long homeSteps = 52;  
  digitalWrite(DIR, LOW);  
  
  for (long i = 0; i < homeSteps; i++) { 
    stepMotorX();
  }
  Serial.println("Đã về vị trí home");
}

void stepMotorX() {
  digitalWrite(PUL, HIGH);
  delayMicroseconds(7000);  
  digitalWrite(PUL, LOW);
  delayMicroseconds(7000);
}

void moveMotor1() {
  long bufferSteps = 3; 
  long homeSteps = 52;
    digitalWrite(DIR, HIGH);
  for (long i = 0; i < (homeSteps - bufferSteps); i++) {
    stepMotorX();
  }
  digitalWrite(DIR, LOW);
  for (long i = 0; i < (homeSteps - bufferSteps); i++) {
    stepMotorX();
  }
}
void moveMotor2() {
  long bufferSteps = 2; 
  long homeSteps = 52;
    digitalWrite(DIR, LOW);
  for (long i = 0; i < (homeSteps - bufferSteps); i++) {
    
    stepMotorX();
    phunThucAn();
  }
  digitalWrite(DIR, HIGH);
  for (long i = 0; i < (homeSteps - bufferSteps); i++) {
   
    stepMotorX();
     phunThucAn();
  }
}
void phunThucAn() {
  esc.writeMicroseconds(2000);
}

