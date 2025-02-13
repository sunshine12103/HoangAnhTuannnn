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