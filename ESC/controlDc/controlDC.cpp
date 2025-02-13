#include "lib.h"
namespace main{
    namespace controlDC{
        Servo myServo;
        void setupBLDC(){
            for (int i = 1100; i < 2000; i+200){
                myServo.write(i);
                delay(2000);
            }
        }
    }
}