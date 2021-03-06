/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
StaticJsonDocument<200> doc;

byte shower[8] = {
  0b11100,
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b10101,
  0b10101,
  0b10101
};

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  // set up the LCD's number of columns and rows:
  
  lcd.init();
  lcd.backlight();
  //lcd.createChar(0, shower);
  lcd.print("Conectando...");
  Serial.print("Conectando...");
}

void loop() {
  DeserializationError error = deserializeJson(doc, Serial);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  long totalml = doc["totalml"];
  const char* typeSensor = doc["sensor"];

  long totallitros = totalml/1000;
  long mtcubicos = totallitros/1000;
  long precioporm3 = mtcubicos*463;

  long precio = 663 + precioporm3;

  lcd.home();  
  lcd.print("Agua :"); lcd.print(totalml); lcd.print(" [ml]");
  lcd.setCursor(0,1);  
  lcd.print("Costo :"); lcd.print(precio); lcd.print(" $");
  // lcd.setCursor(0, 33);
  // lcd.print(millis() / 1000);
  delay(1000);
}
