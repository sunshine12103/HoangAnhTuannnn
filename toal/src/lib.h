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
extern HX711 scale;

namespace main{
    typedef struct  {
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    }time;
    namespace setup{
        void setup();
    }
    namespace controlDC{
        void setupDC();
        void setHomeDC();
        void setupBLDC();
        void controlDC(int lake);
        extern Servo esc;
        extern int homeSteps;
        extern int buoclui;
    }
    namespace ds1307{
        void setup();
        void readTime();
        void caculateON();
        void caculateOFF();
        bool caculatetime(int currentHour, int currentMinute, int startHour, int startMinute, int endHour, int endMinute);
        extern RTC_DS1307 rtc;
        extern time thoiGian;
    }
    namespace displayLCD{
        extern LiquidCrystal_I2C lcd;
        void setupLCD();
        void displayLCD();
    }
    namespace loadcell{
        extern float calibration_factor ;  
        extern float tare_weight;  
        extern unsigned long buttonPressStart;  
        extern bool buttonPressed ;
        void setupLoadCell();
        float readEEPROMFloat(int address);
        void readLoadCell();
        void writeEEPROMFloat(int address, float value);
    }
    namespace receiveData {
        
        void setupReceive();
        void receiveSerialData();
    }
}