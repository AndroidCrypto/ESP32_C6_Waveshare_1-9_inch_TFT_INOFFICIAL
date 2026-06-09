/*
  Hardware: Waveshare ESP32-C6 1,9-inch TFT Display 170x320 pixel with Touch

  This sketch is showing some device information on the TFT display using the
  modified TFT_eSPI Graphic Library

*/

/*
Version Information
05.06.2026 V01 Initial programming
               Library Setup File: LovyanGFX_C6_Waveshare_ST7789_1_9_170x320_Settings.h
*/

// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare 1.90-inches ST7789 170x320 Display Information LovyanGFX V01";
const char *PROGRAM_VERSION_SHORT = "Waveshare DispInfo V01";

// --------------------------------------------------------------
// LovyanGFX library

#include <LovyanGFX.hpp> // https://github.com/lovyan03/LovyanGFX version 1.2.21
#include "LovyanGFX_C6_Waveshare_ST7789_1_9_170x320_Settings.h"

// Note: On ESP32-C6 the SPI mode is '3' and not '0' as on ESP32
LGFX tft;

const char *DISPLAY_LIBRARY = "LovyanGFX";
uint8_t ledBrightnessPercent = 70;

// --------------------------------------------------------------
// Display data
// filled in initializeDisplay()
uint16_t TFT_WIDTH;
uint32_t TFT_HEIGHT;
uint16_t TFT_WIDTH_2;

const char *DISPLAY_LINE01 = "Waveshare";
const char *DISPLAY_LINE02 = "ESP32-C6";
const char *DISPLAY_LINE03 = "1.9-inches";
const char *DISPLAY_LINE04 = "170 x 320 px";
const char *DISPLAY_LINE05 = "Type";
const char *DISPLAY_LINE06 = "ST7789";
const char *DISPLAY_LINE07 = "Touch";
const char *DISPLAY_LINE08 = "CST816";
const char *DISPLAY_LINE09 = "Android";
const char *DISPLAY_LINE10 = "Crypto";

// colors
const uint16_t D_BLACK = TFT_BLACK;
const uint16_t D_WHITE = TFT_WHITE;
const uint16_t D_RED = TFT_RED;
const uint16_t D_GREEN = TFT_GREEN;
const uint16_t D_BLUE = TFT_BLUE;
const uint16_t D_YELLOW = TFT_YELLOW;
const uint16_t D_MAGENTA = TFT_MAGENTA;
const uint16_t D_ORANGE = TFT_ORANGE;
const uint16_t D_SKYBLUE = TFT_SKYBLUE;
const uint16_t D_CYAN = TFT_CYAN;

// --------------------------------------------------------------
// Display control

void setDisplayBrightnessLovyanGFX(uint8_t brightnessPercent) {
  if (brightnessPercent > 100) brightnessPercent = 100;
  if (brightnessPercent < 0) brightnessPercent = 0;
  tft.setBrightness(255 - (255 * ledBrightnessPercent / 100));
}

const uint8_t DISPLAY_ORIENTATION_PORTRAIT = 0; // USB connector @bottom
const uint16_t DISPLAY_BACKGROUND_COLOR = D_BLACK;
const uint16_t DISPLAY_FOREGROUND_COLOR = D_WHITE;
const uint8_t DISPLAY_FONT_TYPE = 4;
const uint8_t DISPLAY_FONT_TYPE_SMALL = 2;
const uint8_t TEXT_SIZE = 1;
const uint8_t LINE_DISTANCE = 25;
const uint8_t DISPLAY_START_Y = 10;
const uint8_t DISPLAY_DISTANCE = 30;

void initializeDisplay() {
  tft.init();
  // set the brightness of the display
  setDisplayBrightnessLovyanGFX(ledBrightnessPercent);
  delay(10);
  tft.setRotation(DISPLAY_ORIENTATION_PORTRAIT);

  tft.fillScreen(DISPLAY_BACKGROUND_COLOR);
  tft.setTextColor(DISPLAY_FOREGROUND_COLOR, DISPLAY_BACKGROUND_COLOR);
  TFT_WIDTH = tft.width(); // taken from User Setup
  TFT_HEIGHT = tft.height();
  TFT_WIDTH_2 = TFT_WIDTH / 2;
  Serial.println("Initialization of the display done");
}

void showInformation() {
  tft.fillScreen(DISPLAY_BACKGROUND_COLOR);

  tft.setTextSize(TEXT_SIZE);

  tft.setTextColor(D_RED, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE01, TFT_WIDTH_2, DISPLAY_START_Y + 0 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  tft.setTextColor(D_CYAN, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE02, TFT_WIDTH_2, DISPLAY_START_Y + 1 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_MAGENTA, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE03, TFT_WIDTH_2, DISPLAY_START_Y + 2 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_ORANGE, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE04, TFT_WIDTH_2, DISPLAY_START_Y + 3 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_GREEN, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE05, TFT_WIDTH_2, DISPLAY_START_Y + 4 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_SKYBLUE, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE06, TFT_WIDTH_2, DISPLAY_START_Y + 5 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  tft.setTextColor(D_YELLOW, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE07, TFT_WIDTH_2, DISPLAY_START_Y + 6 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_RED, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE08, TFT_WIDTH_2, DISPLAY_START_Y + 7 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_WHITE, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE09, TFT_WIDTH_2, DISPLAY_START_Y + 8 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.setTextColor(D_WHITE, D_BLACK);
  tft.drawCentreString(DISPLAY_LINE10, TFT_WIDTH_2, DISPLAY_START_Y + 9 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);

  tft.drawRoundRect(0, 0, TFT_WIDTH, TFT_HEIGHT, 25, D_GREEN);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

  initializeDisplay();
  tft.setTextSize(TEXT_SIZE);
  tft.drawCentreString(PROGRAM_VERSION_SHORT, TFT_WIDTH_2, DISPLAY_START_Y + 0 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE_SMALL);
  tft.drawCentreString("Display init done", TFT_WIDTH_2, DISPLAY_START_Y + 1 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE_SMALL);
  tft.drawCentreString("Using", TFT_WIDTH_2, DISPLAY_START_Y + 2 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  tft.drawCentreString(DISPLAY_LIBRARY, TFT_WIDTH_2, DISPLAY_START_Y + 3 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  delay(4000);
  showInformation();

  Serial.printf("Display width: %d height: %d\n", TFT_WIDTH, TFT_HEIGHT);
}

void loop() {
  delay(100);
}
