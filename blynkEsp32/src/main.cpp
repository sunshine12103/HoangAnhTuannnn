#define BLYNK_PRINT Serial
#include <WiFi.h>              
#include <BlynkSimpleEsp32.h>
#include <ArduinoJson.h>


char ssid[] = "DACN";       
char pass[] = "12345678";

char auth[] = "m9o91Y5IwOOSzdWggKzfsORP3O06nzVe";
char server[] = "blynk.dke.vn";
int port = 8888;

int lake1;
int lake2;
int ao;
int hourOn1, minuteOn1, getTimeOn1;
int hourOff1, minuteOff1, getTimeOff1;
int hourOn2, minuteOn2, getTimeOn2;
int hourOff2, minuteOff2, getTimeOff2;
int stateLake1;
int stateLake2;
float kg, temp;
int enter;
int emergency;

void sendData() {
  StaticJsonDocument<2048> doc;
  doc["lake1"] = lake1;
  doc["lake2"] = lake2;
  doc["hourOn1"] = hourOn1;
  doc["minuteOn1"] = minuteOn1;
  doc["hourOff1"] = hourOff1;
  doc["minuteOff1"] = minuteOff1;
  doc["hourOn2"] = hourOn2;
  doc["minuteOn2"] = minuteOn2;
  doc["hourOff2"] = hourOff2;
  doc["minuteOff2"] = minuteOff2;
  doc["kg"] = kg;
  doc["temp"] = temp;
  doc["emergency"] = emergency;
  doc["stateLake1"] = stateLake1;
  doc["stateLake2"] = stateLake2;

  String jsonString;
  serializeJson(doc, jsonString);
  Serial1.print(jsonString);
  Serial1.println();
}

BLYNK_WRITE(V2) {
  ao = param.asInt();
  if (ao == 1)
  {
    lake1 = 1;
  }
  if(ao == 2)
  {
    lake2 = 2;
  }
}
BLYNK_WRITE(V3) {
  getTimeOn1 = param.asInt(); 
  hourOn1 = getTimeOn1 / 3600;
  minuteOn1 = (getTimeOn1 % 3600) / 60;
  Serial.println(hourOn1);
  Serial.println(minuteOn1);
}

BLYNK_WRITE(V4) {
  getTimeOff1 = param.asInt(); 
  hourOff1 = getTimeOff1 / 3600;
  minuteOff1 = (getTimeOff1 % 3600) / 60;
  Serial.println(hourOff1);
  Serial.println(minuteOff1);
}

BLYNK_WRITE(V5) {
  kg = param.asFloat(); 
  Serial.println(kg);
}

BLYNK_WRITE(V6) {
  temp = param.asFloat();
  Serial.println(temp);
}
BLYNK_WRITE(V7)
{
  enter = param.asInt();
  Serial.println(enter);
  if (enter == 1)
  {
    emergency = 1; 
    sendData();
    Serial.println("Send data to ESP32");
    Serial.println("Lake: " + String(lake1));
    Serial.println("Lake: " + String(lake2));
    Serial.println("Time On: " + String(hourOn1) + ":" + String(minuteOn1));
    Serial.println("Time Off: " + String(hourOff1) + ":" + String(minuteOff1));
    Serial.println("Time On: " + String(hourOn2) + ":" + String(minuteOn2));
    Serial.println("Time Off: " + String(hourOff2) + ":" + String(minuteOff2));
    Serial.println("Kg: " + String(kg));
    Serial.println("Temp: " + String(temp));
    Serial.println("Emergency: " + String(emergency));
    Serial.println("State Lake 1: " + String(stateLake1));
    Serial.println("State Lake 2: " + String(stateLake2));
    
  }
  if(enter == 0)
  {
    emergency = 0;
    StaticJsonDocument<200> doc;
    doc["emergency"] = emergency;
    String jsonString;
    serializeJson(doc, jsonString);
    Serial1.print(jsonString);
    Serial1.println();
  }
}   
BLYNK_WRITE(V8)
{
  getTimeOn2 = param.asInt(); 
  hourOn2 = getTimeOn2 / 3600;
  minuteOn2 = (getTimeOn2 % 3600) / 60;
  Serial.println(hourOn2);
  Serial.println(minuteOn2);
}
BLYNK_WRITE(V9)
{
  getTimeOff2 = param.asInt(); 
  hourOff2 = getTimeOff2 / 3600;
  minuteOff2 = (getTimeOff2 % 3600) / 60;
  Serial.println(hourOff2);
  Serial.println(minuteOff2);
}
BLYNK_WRITE(V10)
{
  stateLake1 = param.asInt();
}
BLYNK_WRITE(V11)
{
  stateLake2 = param.asInt();
}

void setup() {
  Serial.begin(115200); 
  Serial1.begin(115200, SERIAL_8N1, 16, 17);

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, pass, "blynk.dke.vn", port);
}

void loop() {
  Blynk.run();
}
