#include "lib.h"
#include <Servo.h>

Servo esc; 

void setup() {
  esc.attach(7); 
  esc.writeMicroseconds(1000);
  delay(2000);
  
}

void loop() {

    esc.writeMicroseconds(1500); 
  
}