#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2c.h>
#include <Servo.h>
// #include <DS1307.h>
#define ESCpin 6
namespace main{
    namespace controlDC{
        void setupDC();
        void controlDC(int lake );
        void setupBLDC();
        void controlBLDC(int speed);
        extern Servo myservo;
    }
    namespace ds1307{
        void setupDS1307();
        void readDS1307();
        void caculateON();
        void caculateOFF();
    }
    namespace displayLCD{
        extern LiquidCrystal_I2C lcd;
        void setupLCD();
        void displayLCD();
    }
    namespace loadcell{
        void setupLoadCell();
        void readLoadCell();
    }
    namespace action{
       void actionSetup();
        void actionLoop();
    }
}