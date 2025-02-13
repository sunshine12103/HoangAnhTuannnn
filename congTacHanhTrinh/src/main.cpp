#include <Arduino.h>
#define CTHT1 25
#define CTHT2 3
void setup() {
  Serial.begin(9600);
  pinMode(CTHT1, INPUT);
  pinMode(CTHT2, INPUT);
}
void loop()
{
Serial.print("CTHT1: ");
Serial.println(digitalRead(CTHT1));
Serial.print("CTHT2: ");
Serial.println(digitalRead(CTHT2));
delay(1000);
}