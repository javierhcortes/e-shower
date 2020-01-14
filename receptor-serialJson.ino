#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>

//U8g2 Constructor List - https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#introduction
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4);

const char *ssid = "poopssid";
const char *password = "pingu4prez";

ESP8266WebServer server(80);
StaticJsonDocument<200> doc;

void handleSentVar() {
  if (server.hasArg("sensor_reading")) { // this is the variable sent from the client
    int readingInt = server.arg("sensor_reading").toInt();
    char readingToPrint[5];
    itoa(readingInt, readingToPrint, 10); //integer to string conversion for OLED library
    u8g2.firstPage();
    u8g2.drawUTF8(0, 64, readingToPrint);
    u8g2.nextPage();
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  delay(1000);

  u8g2.begin();
  u8g2.setFont(u8g2_font_logisoso62_tn);
  u8g2.setFontMode(0);    // enable transparent mode, which is faster
  u8g2.firstPage();
  u8g2.drawUTF8(0, 64, "hELLO wORLD");
  //u8g2.nextPage();

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();

  Serial.begin(9600);
  while (!Serial) continue;
  
}

void loop() {
  server.handleClient();
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);
  //serializeJson(doc, Serial);
  Serial.println();
  serializeJsonPretty(doc, Serial);
    delay(100);

}
