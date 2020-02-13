#include <Arduino.h>
#include "Display.h"
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin) // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

Adafruit_SSD1306 oledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned int nextGraphIndex = 0;
unsigned int graph[66];

void Display::initializeDisplay()
{
  if (!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x32, SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  Display::showSplashScreen();
}

void Display::update(double RPM)
{
  static const unsigned char PROGMEM thumbsUp[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x00, 0x30, 0x3e, 0x00, 0x78, 0x7c, 0x00, 0x3f, 0xf0, 0x00, 0x1f, 0xe0, 0x00, 0x07, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static const unsigned char PROGMEM thumbsDown[] = {0x00, 0x00, 0x00, 0x38, 0x03, 0x80, 0x7e, 0x0f, 0xc0, 0x3f, 0x1f, 0x80, 0x0f, 0xfe, 0x00, 0x07, 0xfc, 0x00, 0x03, 0xf8, 0x00, 0x07, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x3f, 0x1f, 0x80, 0x7c, 0x07, 0xc0, 0x38, 0x03, 0x80, 0x00, 0x00, 0x00};

  oledDisplay.clearDisplay();

  oledDisplay.setTextSize(2);
  oledDisplay.setCursor(2, 2);
  oledDisplay.setTextColor(WHITE);
  oledDisplay.println(RPM);

  oledDisplay.setTextSize(1);
  oledDisplay.setCursor(65, 2);
  oledDisplay.println("RPM");

  oledDisplay.drawLine(0, 18, 128, 18, WHITE);
  oledDisplay.drawLine(90, 2, 90, 15, WHITE);

  if ((RPM > 33 && RPM < 34) || (RPM > 44 && RPM < 46))
  {
    oledDisplay.drawBitmap(100, 3, thumbsUp, 19, 13, 1);
  }
  else
  {
    oledDisplay.drawBitmap(100, 3, thumbsDown, 19, 13, 1);
  }

  if (RPM <= 10)
  {
    graph[nextGraphIndex] = 1;
  }
  else if (RPM > 10 && RPM <= 20)
  {
    graph[nextGraphIndex] = 2;
  }
  else if (RPM > 20 && RPM <= 30)
  {
    graph[nextGraphIndex] = 4;
  }
  else if (RPM > 30 && RPM <= 40)
  {
    graph[nextGraphIndex] = 6;
  }
  else if (RPM > 40)
  {
    graph[nextGraphIndex] = 8;
  }

  nextGraphIndex++;
  if (nextGraphIndex > 67)
  {
    nextGraphIndex = 0;
  }

  for (int i = 0; i < 66; i++)
  {
    oledDisplay.drawLine((1 * i) * 2 + 1, 28, (1 * i * 2) + 1, 28 - graph[i], WHITE);
  }

  oledDisplay.display();
}

void Display::showSplashScreen()
{
  static const unsigned char PROGMEM splashScreen[] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x03, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x1d, 0xbd, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x3d, 0xfd, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x01, 0xfd, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x03, 0xff, 0xfb, 0xfc, 0x00, 0x3e, 0x1f, 0x18, 0x18, 0x87, 0xc6, 0x60, 0x70, 0x02, 0x00, 0x00,
      0x05, 0xfe, 0xfb, 0x7b, 0x00, 0x33, 0x11, 0x3c, 0x38, 0x8c, 0x66, 0x70, 0x70, 0x02, 0x00, 0x00,
      0x0e, 0xf6, 0xfb, 0xf7, 0xc0, 0x31, 0x11, 0xbc, 0x38, 0x98, 0x26, 0x70, 0xf0, 0xc7, 0x86, 0x04,
      0x1f, 0xfa, 0xfe, 0xef, 0x80, 0x31, 0x11, 0xbc, 0x28, 0x10, 0x30, 0x58, 0xb3, 0xe7, 0xdf, 0x3e,
      0x27, 0xdf, 0x25, 0xde, 0x60, 0x3f, 0x13, 0x36, 0x68, 0x10, 0x30, 0x58, 0xb2, 0x33, 0x31, 0x30,
      0x3d, 0xfd, 0x25, 0xf9, 0xe0, 0x3e, 0x1e, 0x36, 0x48, 0x10, 0x30, 0x49, 0xb3, 0xf2, 0x3f, 0xb0,
      0x7e, 0x7a, 0xfa, 0xe7, 0xf0, 0x33, 0x18, 0x32, 0xc8, 0x10, 0x20, 0x4d, 0x33, 0xf2, 0x3f, 0x30,
      0x1f, 0xcf, 0xff, 0x9f, 0x00, 0x31, 0x10, 0x33, 0xc8, 0x18, 0x60, 0x4f, 0x32, 0x02, 0x30, 0x30,
      0x60, 0xf7, 0xff, 0x70, 0xf0, 0x31, 0x90, 0x31, 0x88, 0x0c, 0xe0, 0x46, 0x33, 0x03, 0x10, 0x30,
      0x7f, 0x07, 0xff, 0x0f, 0xf0, 0x31, 0x90, 0x31, 0x88, 0x07, 0x80, 0x46, 0x31, 0xf3, 0x9f, 0x30,
      0x7f, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x07, 0x8f, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x7f, 0xf7, 0xff, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x7f, 0x0f, 0xff, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x70, 0xf7, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x0f, 0x99, 0xfe, 0xe7, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x3f, 0x7e, 0x65, 0x7b, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x3c, 0xed, 0x65, 0xde, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x08, 0x00, 0x80,
      0x03, 0xdf, 0x7a, 0xef, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0c, 0x20, 0x00,
      0x0f, 0xbb, 0xfb, 0x77, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x76, 0xff, 0x78, 0x9e,
      0x0f, 0x7e, 0xeb, 0x7b, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x96, 0x8d, 0xa0, 0xb2,
      0x01, 0xfe, 0xfd, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x86, 0x88, 0xa0, 0xb2,
      0x00, 0xdf, 0xfd, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xf6, 0xe8, 0xba, 0x9c,
      0x00, 0x3d, 0xfd, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x0d, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  oledDisplay.clearDisplay();
  oledDisplay.drawBitmap(0, 0, splashScreen, 128, 32, 1);
  oledDisplay.display();
  delay(5000);

  oledDisplay.clearDisplay();
  Display::update(0.0);
}
