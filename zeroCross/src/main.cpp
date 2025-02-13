#include <Arduino.h>
#define TRIAC_GATE_PIN 8                 // Chân điều khiển Triac
#define ZERO_CROSS_PIN 19                // Chân nhận tín hiệu zero-cross
volatile bool zeroCrossDetected = false; // Biến cờ để theo dõi zero-cross
int alpha = 9500;                  // Thời gian trễ để điều chỉnh mức công suất (micro giây)
void zeroCrossISR()
{
    digitalWrite(TRIAC_GATE_PIN, LOW); // Tắt Triac
    delayMicroseconds(alpha); 
    digitalWrite(TRIAC_GATE_PIN, HIGH); // Tắt Triac
}
void setup()
{
    pinMode(TRIAC_GATE_PIN, OUTPUT);
    pinMode(ZERO_CROSS_PIN, INPUT);
    digitalWrite(TRIAC_GATE_PIN, LOW); // Tắt Triac
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(ZERO_CROSS_PIN), zeroCrossISR, HIGH);
}
void loop()
{
}