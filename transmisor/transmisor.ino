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

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  Serial.println("OLED init...");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // display.display();
  // delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  Serial.println("IO test");

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 2);
  display.println("Inicio...");
  display.display(); // actually display all of the above
  

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

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

//  u8g2.begin();
//  u8g2.setFont(u8g2_font_logisoso62_tn);
//  u8g2.setFontMode(0);    // enable transparent mode, which is faster
}

void loop() {
//  // read the analog in value:
//  sensorValue = analogRead(A0);
//  // map to range. The pot goes from about 3 to 1023. This makes the sent value be between 0 and 999 to fit on the OLED
//  outputValue = map(sensorValue, 3, 1023, 0, 999);
//
//  char intToPrint[5];
//  itoa(outputValue, intToPrint, 10); //integer to string conversion for OLED library
//  u8g2.firstPage();
//  u8g2.drawUTF8(0, 64, intToPrint);
//  u8g2.nextPage();
//
//  // Use WiFiClient class to create TCP connections
//  WiFiClient client;
//  const char * host = "192.168.4.1";
//  const int httpPort = 80;
//
//  if (!client.connect(host, httpPort)) {
//    Serial.println("connection failed");
//    return;
//  }
//
//  // We now create a URI for the request. Something like /data/?sensor_reading=123
//  String url = "/data/";
//  url += "?sensor_reading=";
//  url += intToPrint;
//
//  // This will send the request to the server
//  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" +
//               "Connection: close\r\n\r\n");
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      return;
//    }
//  }
//
//  delay(200);
}
