#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// librerias para display oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

const char *ssid = "poopssid";
const char *password = "pingu4prez";

long currentMillis = 0;
long previousMillis = 0;
long currentMillis_wifi = 0;
long previousMillis_wifi = 0;

int interval = 300;
float calibrationFactor = 8.5;//4.5
float flowRate;

volatile byte pulseCount;
byte pulse1Sec = 0;

unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

const int sensorFlujo = D5;

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

WiFiClient client;
const char * host = "192.168.4.1";
const int httpPort = 80;

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  pinMode(sensorFlujo, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  previousMillis_wifi = 0;

  attachInterrupt(digitalPinToInterrupt(sensorFlujo), pulseCounter, FALLING);

  Serial.println("OLED init...");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // display.display();
  // delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 2);
  display.println("Inicio...");
  display.display(); // actually display all of the above

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);


  while (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0,2);    display.println("Conectando...");
    display.setCursor(0,10);   display.println("ssid: ");
    display.setCursor(35,10);  display.println(ssid);
    display.setCursor(0,20);   display.println("pass: ");
    display.setCursor(35,20);  display.println(password);
    display.display();

    delay(300);
  }
  display.clearDisplay();
  display.setCursor(0,2);    display.println("ONLINE!");
  display.display();

}

void loop() {
  currentMillis = millis();
  currentMillis_wifi = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    Serial.print("Flow rate: ");
    Serial.print(int(flowMilliLitres));  // Print the integer part of the variable
    Serial.print("mL/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.println("mL");
    // oled
    display.clearDisplay();
    display.setCursor(0,2);    display.println("ONLINE! [Total ml]");
    display.setCursor(0,10);   display.println(totalMilliLitres);
    display.display();
  }

  if (currentMillis_wifi - previousMillis_wifi > 10*interval) {
    previousMillis_wifi = millis();

    // Use WiFiClient class to create TCP connections
    // WiFiClient client;

    if (client.connect(host, httpPort)) {
      Serial.println("connected");
      // We now create a URI for the request. Something like /data/?sensor_reading=123

      String url = "/data/";
      url += "?sensor_reading=";
      url += totalMilliLitres;

      // This will send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n");
      // unsigned long timeout = millis();
      // while (client.available() == 0) {
      //   if (millis() - timeout > 5000) {
      //     Serial.println(">>> Client Timeout !");
      //     client.stop();
      //     return;
      //   }
      // }
    }
    else{
      Serial.println("connection failed");
      //delay(200);
    }
  }
}
