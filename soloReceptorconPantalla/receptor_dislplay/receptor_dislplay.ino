#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// librerias para display oled
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

//Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

#include <ArduinoJson.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

void handleSentVar();

const char *ssid = "poopssid";
const char *password = "pingu4prez";

 // set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F,16,2); 

ESP8266WebServer server(80);
StaticJsonDocument<200> doc;

long currentMillis = 0;
long previousMillis = 0;
int currentAnimation = 0;
long interval = 1000;

byte shower1[8] = {  0b11100,  0b00100,  0b01110,  0b11111,  0b00000,  0b10101,  0b10101,  0b10101};
byte shower2[8] = {  0b11100,  0b00100,  0b01110,  0b11111,  0b00000,  0b01010,  0b01010,  0b00000};
byte arrowDown1[8] = { 0b00100, 0b00100,  0b00100,  0b11111,  0b01110,  0b00100,  0b10001,  0b11111};
byte arrowDown2[8] = { 0b00000, 0b00100,  0b00100,  0b00100,  0b11111,  0b01110,  0b10101,  0b11111};

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.noBlink();
  lcd.createChar(0, shower1);  lcd.createChar(1, shower2);
  lcd.createChar(2, arrowDown1);  lcd.createChar(3, arrowDown2);
  lcd.setCursor(0,0);
  lcd.print("Iniciando..."); 
  Serial.print("Iniciando...");

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.on("/data/", HTTP_GET, handleSentVar);
  server.begin();
  delay(2000);
}

void loop() {
  
  if (currentAnimation > 1)
    currentAnimation = 0;
  
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    lcd.setCursor(12,0);
    lcd.write(currentAnimation);
    currentAnimation++;
    previousMillis = millis();
  }
  
  server.handleClient();
}

void handleSentVar() {
  if (server.hasArg("sensor_reading")){
    int readingInt = server.arg("sensor_reading").toInt();
    Serial.print("Valor Obtenido...");
    Serial.println(readingInt);
    
    //server.send(200, "text/html", "Data received");

    lcd.home();  
    lcd.setCursor(0,1);

    int totalml = readingInt;
    

    long totallitros = totalml/1000.0;
    long mtcubicos = totallitros/1000.0;
    long precioporm3 = mtcubicos*463;

    long precio = 663 + precioporm3;

    lcd.home();  
    lcd.print("Agua :"); lcd.print(totallitros); lcd.print(" [L]");
    lcd.setCursor(0,1);  
    lcd.print("Costo :"); lcd.print(precio); lcd.print(" $");
    // lcd.setCursor(0, 33);
    // lcd.print(millis() / 1000);
    //delay(1000);
    //lcd.print(readingInt);  lcd.print(" [L]");   
  }
  else{
    Serial.println("Error: no existe sensor_reading");
    //delay(100);
  }
}
