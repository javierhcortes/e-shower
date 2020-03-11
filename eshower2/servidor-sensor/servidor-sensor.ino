/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <Ticker.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

float calibrationFactor = 8.5;
float flowRate = 0.0;

volatile byte pulseCount = 0;
byte pulse1Sec = 0;

unsigned int flow = 0;
double total = 0;

double storedTotal = 0;
int secElapsedShower = 0;

const int sensorFlujo = D5;

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

Ticker timerSensor;
Ticker timerAlarma;

void calculateLitters()
{  
  pulse1Sec = pulseCount;
  pulseCount = 0;

  // Because this loop may not complete in exactly 1 second intervals we calculate
  // the number of milliseconds that have passed since the last execution and use
  // that to scale the output. We also apply the calibrationFactor to scale the output
  // based on the number of pulses per second per units of measure (litres/minute in
  // this case) coming from the sensor.
  //flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;

  //PERO nosotros usamos ticker de 1 seg.
  flowRate = (pulse1Sec) / calibrationFactor;

  // Divide the flow rate in litres/minute by 60 to determine how many litres have
  // passed through the sensor in this 1 second interval, then multiply by 1000 to
  // convert to millilitres.
  
  flow = (flowRate / 60) * 1000.0;
  total += flow /1000.0F;
  //totalLiters = totalMilliLitres / 1000.0F;

  Serial.print("Flow rate: ");
  Serial.print(flow);
  Serial.println("mL/min");

  // Print the cumulative total of litres flowed since starting
  Serial.print("Output Liquid Quantity: ");
  Serial.print(total,2);
  Serial.println("[L]");
}

int is_equal_3decplaces(double a, double b) {
    long long ai = a;
    long long bi = b ;
    return ai == bi;
}

void isShower(){
  if (total > storedTotal){
    storedTotal = total;
    secElapsedShower++;
    Serial.println("secElapsed : ");//, secElapsedShower);
    Serial.println(secElapsedShower);
  }
  else{
    secElapsedShower = 0;
    Serial.println("secElapsed : ");//, secElapsedShower);
    Serial.println(secElapsedShower);

  }
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

String readLiquid() {
  return String(total, 2);
}

String readAlarma() {
  if (secElapsedShower > 30)    return "enciende";
  if (secElapsedShower <= 30)    return "apaga";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  while (!Serial) continue;

  pinMode(sensorFlujo, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorFlujo), pulseCounter, FALLING);

  //Initialize Ticker every 1s
  timerSensor.attach(1, calculateLitters); //Use <strong>attach_ms</strong> if you need time in ms

  timerAlarma.attach(10, isShower);

  Serial.println("attachInterrupt.");
  Serial.print("Setting AP (Access Point)…");
  
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid);//, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, world");
  });

  server.on("/liquid", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("working");
    request->send(200, "text/plain", readLiquid().c_str());
  });

  server.on("/alarma", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("working");
    request->send(200, "text/plain", readAlarma().c_str());
  });

  server.onNotFound(notFound);
  
  Serial.println("enable on commands");
  // Start server
  server.begin();
}
 
void loop(){
}
