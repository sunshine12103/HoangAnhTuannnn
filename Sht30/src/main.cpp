#include <Arduino.h>
#include "Wire.h"
#include "SHT31.h"


SHT31 sht;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  sht.begin(); 
}

void loop()
{
  sht.read();
  Serial.print("Temperature:");
  Serial.print(sht.getTemperature(), 1);
  Serial.print("\t");
  Serial.print("Humidity:");
  Serial.println(sht.getHumidity(), 1);
  delay(1000);
}
