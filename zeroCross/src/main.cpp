#include <Arduino.h>
#define TRIAC_GATE_PIN 8    // Chân điều khiển Triac
#define ZERO_CROSS_PIN 19   // Chân nhận tín hiệu zero-cross
volatile bool zeroCrossDetected = false;  // Biến cờ để theo dõi zero-cross
int alpha = 5000;  // Thời gian trễ để điều chỉnh mức công suất (micro giây)
void zeroCrossISR() {
    zeroCrossDetected = true;
}
void setup() {
    pinMode(TRIAC_GATE_PIN, OUTPUT);  
    pinMode(ZERO_CROSS_PIN, INPUT);   
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(ZERO_CROSS_PIN), zeroCrossISR, RISING);
}
void loop() {
    // Kiểm tra tín hiệu zero-cross
    int value = digitalRead(ZERO_CROSS_PIN);
    Serial.print("Zero Cross Pin State: ");
    Serial.println(value);
    delay(10);  
    if (zeroCrossDetected) {
        zeroCrossDetected = false;  // Reset cờ
        delayMicroseconds(alpha);  // Trì hoãn kích Triac để điều chỉnh điện áp tải
        
        // Kích Triac
        digitalWrite(TRIAC_GATE_PIN, HIGH);
        delayMicroseconds(1000);  // Giữ Triac bật trong 1ms
        digitalWrite(TRIAC_GATE_PIN, LOW);  // Tắt Triac
        Serial.print("Triac fired at alpha: ");
        Serial.println(alpha);
    }
    // Điều chỉnh góc pha bằng Serial Monitor
    if (Serial.available() > 0) {
        int newAlpha = Serial.parseInt();
        if (newAlpha >= 0 && newAlpha <= 8333) {  // Giới hạn alpha trong 0 - 8333 µs (50Hz)
            alpha = newAlpha;
            Serial.print("Updated alpha to: ");
            Serial.println(alpha);
        }
    }
}