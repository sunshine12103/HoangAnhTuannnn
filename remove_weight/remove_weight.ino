#include "HX711.h"
#include <EEPROM.h>  

#define DT_PIN 65
#define SCK_PIN 64  
#define BUTTON_PIN 22

HX711 scale;

float calibration_factor = 38.3333333;  
float tare_weight = 0;  
unsigned long buttonPressStart = 0;  
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tare_weight = readEEPROMFloat(0); // Đọc tare từ EEPROM
  
  Serial.println("Đang đọc giá trị trọng lượng (kg)...");
}

void loop() {
  float weight_kg = scale.get_units(10) / 1000.0 * -1.0; 

  if (digitalRead(BUTTON_PIN) == LOW) { 
    if (!buttonPressed) {  
      buttonPressStart = millis();  
      buttonPressed = true; 
    } else {
      if (millis() - buttonPressStart >= 5000) {  
        tare_weight = weight_kg;  
        writeEEPROMFloat(0, tare_weight);  // Lưu tare vào EEPROM
        Serial.print("Đã tare! Giá trị tare: ");
        Serial.println(tare_weight); 
        buttonPressed = false; 
      }
    }
  } else { 
    buttonPressed = false; 
  }

  float new_weight_kg = weight_kg - tare_weight;  
  if(new_weight_kg < 0 )
  {
    new_weight_kg =0;
  }
  Serial.print("Trọng lượng (kg): ");
  Serial.println(new_weight_kg);

  delay(1000); 
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
