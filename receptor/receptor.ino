#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// librerias para display oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

const char *ssid = "poopssid";
const char *password = "pingu4prez";

ESP8266WebServer server(80);
StaticJsonDocument<200> doc;

void handleSentVar() {
  if (server.hasArg("sensor_reading")){
   int readingInt = server.arg("sensor_reading").toInt();
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(0, 2);   display.println("Valor Obtenido...");
   display.setCursor(0, 10);  display.println(readingInt);
   display.display();
   server.send(200, "text/html", "Data received");

   doc["sensor"] = "flujo";
   doc["totalml"] = readingInt;

   //serializeJson(doc, Serial);
   Serial.println();
   serializeJsonPretty(doc, Serial);
   delay(100);
  }
  else{
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(0, 2);   display.println("no sensor_reading");
   display.setCursor(0, 10);  display.println("error...");
   display.display();
   delay(100);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  //Serial.println("OLED init...");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println("Iniciando...");
  display.display();
  delay(1000);
  //Serial.println("OLED begun");

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  display.setCursor(0,10);
  display.println("Acces Point UP!");
  display.display();

  // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.on("/data/", HTTP_GET, handleSentVar);
  server.begin();
}

void loop() {
  server.handleClient();
}