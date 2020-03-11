/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display

#include "Animation.h"
#include <Bounce2.h>

const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

//Your IP address or domain name with URL path
const char* serverNameLitters = "http://192.168.4.1/liquid";
const char* serverNameAlarma = "http://192.168.4.1/alarma";
//const char* serverNamePres = "http://192.168.4.1/pressure";

String liters;
String alarma;

unsigned long previousMillis = 0;
unsigned long previousMillis_data = 0;

const long interval = 3000; 
const long intervalIcons = 1000;

int httpCode = -1;

long precioporm3 = 463;
long cargoFijo = 663;

void upPrice(){
  precioporm3 = 463 * 1000;
}

void downPrice(){
  precioporm3 = 463;
}

const int BUTTON_PIN1 = 13;
const int BUTTON_PIN2 = 12;
const int SONIDO_PIN = 10;

Bounce debouncer1 = Bounce(); // Instantiate a Bounce object
Bounce debouncer2 = Bounce(); // Instantiate a Bounce object

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  debouncer1.attach(BUTTON_PIN1,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer1.interval(25); // Use a debounce interval of 25 milliseconds

  debouncer2.attach(BUTTON_PIN2,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer2.interval(25); // Use a debounce interval of 25 milliseconds

  // Para output de sonido
  pinMode(SONIDO_PIN, OUTPUT);
  digitalWrite(SONIDO_PIN, LOW);    // turn the LED off by making the voltage LOW

  //begin display
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.noBlink();
  
  createAllChars();
  
  lcd.setCursor(0,0);
  //lcd.print("Iniciando...");
  Serial.println();
  Serial.print("Iniciando");
 
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid);//, password);
  while((WiFiMulti.run() == WL_CONNECTED)) { 
    delay(500);
    Serial.print(".");
    handleIconWifi();
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  stableWifi_icon();
  cleanNoMenu();
  lcd.setCursor(0,0);  lcd.print("Precio:0"); 
  lcd.setCursor(0,1);  lcd.print("Agua:0");  
}


void loop() {
  
  unsigned long currentMillis = millis();
  unsigned long currentMillis_data = currentMillis;

  debouncer1.update(); // Update the Bounce instance
  debouncer2.update(); // Update the Bounce instance

  // Call code if button transitions from HIGH to LOW
  if (debouncer1.fell()) {    
    Serial.print("baja precio ");
    downPrice();
  }

  if (debouncer2.fell()) {    
    Serial.print("aumento de precio ");
    upPrice();
  }
  

   if(currentMillis - previousMillis >= intervalIcons) {
     handleIconDucha();
     previousMillis = currentMillis;    
   }

  if (currentMillis_data - previousMillis_data >= interval)
  {
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      stableWifi_icon();
      liters = httpGETRequest(serverNameLitters, httpCode);
      if(httpCode > 0){
        Serial.println("Liters: " + liters);
        lcd.setCursor(0,1);
        lcd.print("Agua:"); lcd.print(liters);
        lcd.setCursor(12,1);lcd.print("[L]");
        float litersFloat = liters.toFloat();

        long precio = cargoFijo + (litersFloat /1000.0) * precioporm3;
        cleanNoMenu();
        lcd.setCursor(0,0);
        lcd.print("Precio:"); lcd.print(precio);
        //lcd.setCursor(12,1);lcd.print("[L]");
      }

      alarma = httpGETRequest(serverNameAlarma, httpCode);
      if(httpCode > 0){
         Serial.println("Alarma: " + alarma);
         if (alarma == "enciende"){
          stableAlarm_icon();
          digitalWrite(SONIDO_PIN, HIGH);
         }
          
         if (alarma == "apaga"){
          clear_icon(position_alarma_icon);
          digitalWrite(SONIDO_PIN, LOW);
         }
      }
            
    }
    else {
      Serial.println("WiFi Disconnected");
      handleIconWifi();
      delay(500);
    }
    previousMillis_data = currentMillis_data;   
  }
}

String httpGETRequest(const char* serverName, int &httpResponseCode) {
  WiFiClient client;
  HTTPClient http;
  
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);    
  }
  // Free resources
  http.end();

  return payload;
}
