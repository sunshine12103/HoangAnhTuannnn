#include "Arduino.h"
#include "Wire.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SPI.h>
#include "HX711.h"
#include <EEPROM.h>
#include <ArduinoJson.h>
#define ESC_PIN 7
#define PUL 10 // chân step
#define DIR 11 // chân hướng
#define CTHT2 3  
#define CTHT1 25 
#define MF 12 // chân enable
#define DT_PIN 65
#define SCK_PIN 64  
#define BUTTON_PIN 22
#define speedBLDC 1200
#define DT_PIN 65
#define SCK_PIN 64  
#define BUTTON_PIN 22
#define RULO_PIN 6
LiquidCrystal_I2C lcd(0x27, 20, 4);

HX711 scale;
Servo esc;
int homeSteps = 52;
int buoclui = 5;
float calibration_factor = 38.3333333;  
float tare_weight = 0;  
unsigned long buttonPressStart = 0;  
bool buttonPressed = false;
int lake1 = 0;
int lake2 = 0;
int hourOn1 , minuteOn1;
int hourOff1 , minuteOff1;
int hourOn2 , minuteOn2;
int hourOff2 , minuteOff2;
float kg, temp;
int emergency = 1;
int stateLake1;
int stateLake2;
float weight_kg;
float new_weight_kg;
bool stateLoadCell = false;
RTC_DS1307 rtc;
typedef struct  {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
}time;
time thoiGian;
// khai bao function
void setupDC();
void setHomeDC();
void setupBLDC();
void controlDC(int lake1, int lake2);
void setupds1307();
void readTime();
void caculateOFF1();
bool caculatetime1(int currentHour1, int currentMinute1, int startHour1, int startMinute1, int endHour1, int endMinute1);
void caculateOFF2();
bool caculatetime2(int currentHour2, int currentMinute2, int startHour2, int startMinute2, int endHour2, int endMinute2);

void setupLCD();
void displayLCD();
void setupLoadCell();
float readEEPROMFloat(int address);
void readLoadCell();
void writeEEPROMFloat(int address, float value);
void setupReceive();
void receiveSerialData();
void setupLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Trong luong: ");
  lcd.setCursor(0, 1);
  lcd.print("0.00 kg");
}
void displayLCD()
{
  lcd.setCursor(12, 0);
  lcd.print(new_weight_kg, 2);
  lcd.print(" kg");
}
void setupLoadCell()
{
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  tare_weight = readEEPROMFloat(0); // Đọc tare từ EEPROM
}
void readLoadCell()
{
  weight_kg = scale.get_units(3) / 1000.0 * -1.0; // Giảm số lần đọc xuống 3
  if (digitalRead(BUTTON_PIN) == LOW) { 
    if (!buttonPressed) {  
      buttonPressStart = millis();  
      buttonPressed = true; 
    } else {
      if (millis() - buttonPressStart >= 5000) {  
        tare_weight = weight_kg;  
        writeEEPROMFloat(0, tare_weight);  // Lưu tare vào EEPROM
        buttonPressed = false; 
      }
    }
  } else { 
    buttonPressed = false; 
  }

  new_weight_kg = weight_kg - tare_weight + 1.3;  
  if(new_weight_kg < 0 )
  {
    new_weight_kg =0;
  }
}
float readEEPROMFloat(int address) {
  float value;
  byte* p = (byte*)&value;
  for (int i = 0; i < sizeof(float); i++) {
    *p++ = EEPROM.read(address + i);
  }
  return value;
}

void writeEEPROMFloat(int address, float value) {
  byte* p = (byte*)&value;
  for (int i = 0; i < sizeof(float); i++) {
    EEPROM.write(address + i, *p++);
  }
}
void setupReceive()
{
  Serial2.begin(115200);
}
void setupDC()
{
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MF, OUTPUT);
  pinMode(CTHT2, INPUT_PULLUP);  
  pinMode(CTHT1, INPUT_PULLUP);
  digitalWrite(MF, LOW);
}
void setupBLDC()
{
  esc.attach(ESC_PIN);
  esc.writeMicroseconds(1000);
  delay(2000);
}
void setupds1307()
{
  if (! rtc.begin())
 {
   Serial.print("Couldn't find RTC");
   while (1);
 }

  if (! rtc.isrunning())
 {
   Serial.print("RTC is NOT running!");
   Serial.println();
 }
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}
void setHomeDC(){
  // tìm công tắc hành trình
   digitalWrite(DIR, HIGH); 
   while (digitalRead(CTHT2) != LOW) {
       digitalWrite(PUL, HIGH);
       delayMicroseconds(7000);  
       digitalWrite(PUL, LOW);
       delayMicroseconds(7000);
   }
   // quay về nhà yêu dấu 
   digitalWrite(DIR, LOW);  
   for (long i = 0; i < homeSteps; i++) { 
       digitalWrite(PUL, HIGH);
       delayMicroseconds(7000);  
       digitalWrite(PUL, LOW);
       delayMicroseconds(7000);
   }            
}
void receiveSerialData() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    Serial.println(data);
    StaticJsonDocument <2048> doc;
    DeserializationError error = deserializeJson(doc, data);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }
    lake1 = doc["lake1"];
    lake2 = doc["lake2"];
    hourOn1 = doc["hourOn1"];
    minuteOn1 = doc["minuteOn1"];
    hourOff1 = doc["hourOff1"];
    minuteOff1 = doc["minuteOff1"];
    hourOn2 = doc["hourOn2"];
    minuteOn2 = doc["minuteOn2"];
    hourOff2 = doc["hourOff2"];
    minuteOff2 = doc["minuteOff2"];
    emergency = doc["emergency"];
    stateLake1 = doc["stateLake1"];
    stateLake2 = doc["stateLake2"];

    // Serial.println(lake);
    // Serial.println(hourOn);
    // Serial.println(minuteOn);
    // Serial.println(hourOff);
    // Serial.println(minuteOff);

    }
  }
// điều khiển step
void controlDC1(int lake1){
  if(lake1 == 0){
      digitalWrite(MF,HIGH);
      digitalWrite(RULO_PIN, LOW);
  }
  else if (lake1 == 1) {
      digitalWrite(MF,LOW);
      digitalWrite(DIR, HIGH);
      for (int i = 0; i < (homeSteps - buoclui); i++) {
          digitalWrite(RULO_PIN, HIGH); 
          esc.writeMicroseconds(speedBLDC);
          digitalWrite(PUL, HIGH);
          delayMicroseconds(7000);  
          digitalWrite(PUL, LOW);
          delayMicroseconds(7000);
      }
      digitalWrite(DIR, LOW);
      for (int i = 0; i < (homeSteps - buoclui); i++) {
          digitalWrite(RULO_PIN, HIGH);
          esc.writeMicroseconds(speedBLDC); 
          digitalWrite(PUL, HIGH);
          delayMicroseconds(7000);  
          digitalWrite(PUL, LOW);
          delayMicroseconds(7000);
      }
      
  }
}
void controlDC2(int lake2){
  if(lake2 == 0){
    digitalWrite(MF,HIGH);
    digitalWrite(RULO_PIN, LOW);
  }
  if (lake2 == 2) {
    digitalWrite(MF,LOW);
      digitalWrite(DIR, LOW);
      for (int i = 0; i < (homeSteps - buoclui); i++) {
          digitalWrite(RULO_PIN, HIGH);
          esc.writeMicroseconds(speedBLDC); 
          digitalWrite(PUL, HIGH);
          delayMicroseconds(7000);  
          digitalWrite(PUL, LOW);
          delayMicroseconds(7000);
      }
      digitalWrite(DIR, HIGH);
      for (int i = 0; i < (homeSteps - buoclui); i++) {
          digitalWrite(RULO_PIN, HIGH);
          esc.writeMicroseconds(speedBLDC); 
          digitalWrite(PUL, HIGH);
          delayMicroseconds(7000);  
          digitalWrite(PUL, LOW);
          delayMicroseconds(7000);
      }
  }
}
void readTime() {
  DateTime now = rtc.now();
  thoiGian.hour = now.hour();
  thoiGian.minute = now.minute();
  thoiGian.second = now.second();
  Serial.print("Time: ");
  Serial.print(thoiGian.hour);
  Serial.print(":");
  Serial.print(thoiGian.minute);
  Serial.print(":");
  Serial.println(thoiGian.second);

}

void caculateOFF2() {
  int currentHour = thoiGian.hour;
  int currentMinute = thoiGian.minute;
  if (lake1 == 1 && currentHour == hourOn1 && currentMinute >= minuteOn1 && currentMinute < minuteOff1 && emergency == 1 && stateLake1 == 1) {
    //Serial.println("Lake1 hoạt động");
    controlDC1(lake1);
    stateLoadCell = false;

  } else {
    //Serial.println("Lake1 tắt");
    controlDC1(0);
    esc.writeMicroseconds(1000);
    stateLoadCell = true;

  }
  if (lake2 == 2 && currentHour == hourOn2 && currentMinute >= minuteOn2 && currentMinute < minuteOff2 && emergency == 1 && stateLake2 == 1) {
    //Serial.println("Lake2 hoạt động");
    controlDC2(lake2);
    stateLoadCell = false;

  } else {
    //Serial.println("Lake2 tắt");
    controlDC2(0);
    esc.writeMicroseconds(1000);
    stateLoadCell = true;

  }
}
void setup()
{
  Serial.begin(115200);
  setupReceive();
  setupDC();
  setupBLDC();
  Serial.println("Setup xong BLDC");

  setupds1307();
  Serial.println("Setup xong ds1307");

  pinMode(RULO_PIN, OUTPUT);
  setupLCD();
  setupLoadCell();
  setHomeDC();
  Serial.println("Setup done");
}

void loop()
{
  readTime();
  receiveSerialData();
  caculateOFF2();
  readLoadCell();
  displayLCD();
}