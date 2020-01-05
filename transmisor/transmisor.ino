#include <ESP8266WiFi.h>
#include <U8g2lib.h>

//U8g2 Constructor List - https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#introduction
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4);

const char *ssid = "poopssid";
const char *password = "pingu4prez";

const int analogInPin = 0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the potentiometer
int outputValue = 0;        // value sent to server

long currentMillis = 0;
long previousMillis = 0;

int interval = 1000;
float calibrationFactor = 4.5;
float flowRate;

volatile byte pulseCount;
byte pulse1Sec = 0;

unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

const int sensorFlujo = D5; // variable for D2 pin

void IRAM_ATTR pulseCounter(){
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(sensorFlujo, INPUT_PULLUP);
  delay(10);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(sensorFlujo), pulseCounter, FALLING);

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  u8g2.begin();
  u8g2.setFont(u8g2_font_logisoso62_tn);
  u8g2.setFontMode(0);    // enable transparent mode, which is faster
}

void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = millis();

    // read the analog in value:
    sensorValue = analogRead(A0);
    // map to range. The pot goes from about 3 to 1023. This makes the sent value be between 0 and 999 to fit on the OLED
    outputValue = map(sensorValue, 3, 1023, 0, 999);

    char intToPrint[5];
    itoa(outputValue, intToPrint, 10); //integer to string conversion for OLED library
    u8g2.firstPage();
    u8g2.drawUTF8(0, 64, intToPrint);
    u8g2.nextPage();

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const char * host = "192.168.4.1";
    const int httpPort = 80;

    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // We now create a URI for the request. Something like /data/?sensor_reading=123
    String url = "/data/";
    url += "?sensor_reading=";
    url += intToPrint;

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  }

  // delay(1000);
}
