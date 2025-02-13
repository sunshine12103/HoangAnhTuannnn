#include "lib.h"
namespace main{
    namespace receiveData{
        int lake;
        void setupReceive(){
            Serial2.begin(115200);
        }
        void receiveSerialData() {
            if (Serial2.available()) {
                String data = Serial2.readStringUntil('\n');
                Serial.println(data);
                StaticJsonDocument <200> doc;
                DeserializationError error = deserializeJson(doc, data);
                if (error) {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.c_str());
                    return;
                }
                lake= doc["hourOn"];
                Serial.println(lake);
            }   
        }
    }
}