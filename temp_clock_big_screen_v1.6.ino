/*************************************************** 
  This is an example for the SHTC3 Humidity & Temp Sensor

  Designed specifically to work with the SHTC3 sensor from Adafruit
  ----> https://www.adafruit.com/products/4636

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_SHTC3.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "RTClib.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    16
#define OLED_CS     5
#define OLED_RESET 17
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

RTC_DS3231 rtc;

DateTime now;

float tempf;
int posx = 2;

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int daySpace(){
  //decide spacing to center bottom row based on day of week
  uint8_t dow = now.dayOfTheWeek();
  if(dow == 3) return 4;
  if(dow == 4 || dow == 6) return 7;
  if(dow == 7) return 10;
  return 13;
}

void setup() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  // For adjusting time, uncomment following 3 lines. format for timespan is (days, hours, mins, secs)
  //now = rtc.now();
  //now = now + TimeSpan(0, 0, 0, 0);
  //rtc.adjust(now);
  
  Serial.begin(115200);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHTC3 test");
  if (! shtc3.begin()) {
    Serial.println("Couldn't find SHTC3");
    while (1) delay(1);
  }
  Serial.println("Found SHTC3 sensor");

 if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.display();
  delay(200);

  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);

  display.display();
  delay(25);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Display Setup Complete");
  display.display();
  delay(75);
  
}


void loop() {
  sensors_event_t humidity, temp;
  
  shtc3.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  tempf = (temp.temperature * 1.8) + 32;
  
//  Serial.print("Temperature: "); Serial.print(tempf); Serial.println(" degrees F");
//  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  
  now = rtc.now();
  display.setTextSize(4);
  display.clearDisplay();
  display.setCursor(7,0);
  display.print(tempf,1);
  display.write("F");
  
//  display.setTextSize(2);
//  display.setCursor(20,32);
  display.setTextSize(3);
  display.setCursor(posx,32);
  if(now.hour() < 10) display.write("0");
  display.print(now.hour(), DEC);
  display.setCursor(posx + 30,32);
  display.print(":");
  display.setCursor(posx + 44,32);
  if(now.minute() < 10) display.write("0");
  display.print(now.minute(), DEC);
  display.setCursor(posx + 76,32);
  display.print(":");
  display.setCursor(posx + 90,32);
  if(now.second() < 10) display.write("0");
  display.print(now.second(), DEC);

  display.setTextSize(1);
  display.setCursor(daySpace(),56);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  display.print(" ");
  if(now.month() < 10) display.write("0");
  display.print(now.month());
  display.print("/");
  if(now.day() < 10) display.write("0");
  display.print(now.day());
  display.print("/");
  display.print(now.year());
  
  display.display();

  delay(100);
}

