#include "lib.h"
namespace main{
    namespace controlDC{
        Servo esc;
        int homeSteps = 52;
        int buoclui = 5;

        void setupDC(){
            pinMode(PUL, OUTPUT);
            pinMode(DIR, OUTPUT);
            pinMode(MF, OUTPUT);
            pinMode(CTHT2, INPUT_PULLUP);  
            pinMode(CTHT1, INPUT_PULLUP);
            digitalWrite(MF, LOW);
        }
        void setupBLDC(){
            esc.attach(ESC_PIN);
            esc.writeMicroseconds(1000);
            delay(2000);
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
        // điều khiển step
        void controlDC(int lake){
            if(lake == 0){
                digitalWrite(MF,HIGH);
                
            }
            else if (lake == 1) {
                digitalWrite(MF,LOW);

                digitalWrite(DIR, HIGH);
                for (int i = 0; i < (homeSteps - buoclui); i++) {
                    esc.writeMicroseconds(speedBLDC); 

                    digitalWrite(PUL, HIGH);
                    delayMicroseconds(7000);  
                    digitalWrite(PUL, LOW);
                    delayMicroseconds(7000);
                }
                digitalWrite(DIR, LOW);
                for (int i = 0; i < (homeSteps - buoclui); i++) {
                    esc.writeMicroseconds(speedBLDC); 

                    digitalWrite(PUL, HIGH);
                    delayMicroseconds(7000);  
                    digitalWrite(PUL, LOW);
                    delayMicroseconds(7000);
                }
                
            }
            else if (lake == 2) {
                digitalWrite(DIR, LOW);
                for (int i = 0; i < (homeSteps - buoclui); i++) {
                    esc.writeMicroseconds(speedBLDC); 
                    digitalWrite(PUL, HIGH);
                    delayMicroseconds(7000);  
                    digitalWrite(PUL, LOW);
                    delayMicroseconds(7000);
                }
                digitalWrite(DIR, HIGH);
                for (int i = 0; i < (homeSteps - buoclui); i++) {
                    esc.writeMicroseconds(speedBLDC); 
                    digitalWrite(PUL, HIGH);
                    delayMicroseconds(7000);  
                    digitalWrite(PUL, LOW);
                    delayMicroseconds(7000);
                }
               
            }
        }
    }
}