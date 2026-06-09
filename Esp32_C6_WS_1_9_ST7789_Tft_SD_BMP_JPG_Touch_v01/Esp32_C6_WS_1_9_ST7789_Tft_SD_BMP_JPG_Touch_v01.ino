/*
  Hardware: Waveshare ESP32-C6 1,9-inch TFT Display 170x320 pixel with Touch

  This sketch is showing BMP and JPG images in portrait mode on the TFT display.
  The next image is displayed after touching the display.
  
  This is using the
  modified TFT_eSPI Graphic Library

*/

/*
Version Information
06.06.2026 V01 Initial programming
               Library Setup File: Setup814_C6_WS_ST7789_1_9_170x320.h
*/

// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare 1.90-inches ST7789 170x320 SD read BMP & JPG Touch TFT_eSPI V01";
const char *PROGRAM_VERSION_SHORT = "Waveshare SD&Touch V01";

// --------------------------------------------------------------
// TFT_eSPI library

#include <SPI.h>
#include <TFT_eSPI.h> // https://github.com/Bodmer/TFT_espi

TFT_eSPI tft = TFT_eSPI();

const char *DISPLAY_LIBRARY = "TFT_eSPI";
const uint8_t LED_BACKLIGHT_PIN = TFT_BL; // taken from User Setup
uint8_t ledBrightnessPercent = 100;

// --------------------------------------------------------------
// SD Card Reader
#include "WAVESHARE_SD_BMP_JPG.h"

const uint8_t MAX_IMAGES = 2;
String fileNamesJpg[MAX_IMAGES] = { "/wave_170_320.jpg", "/gps_170_320.jpg" };
String fileNamesBmp[MAX_IMAGES] = { "/wave_170_320.bmp", "/gps_170_320.bmp" };
uint8_t imageIndex = 0;
bool isDisplayed = false;

// --------------------------------------------------------------
// Touch
#include "WAVESHARE_TOUCH.h"

// --------------------------------------------------------------
// Display data

//const uint16_t TFT_WIDTH = 170; // taken from User Setup
//const uint32_t TFT_HEIGHT = 320; // taken from User Setup
const uint16_t TFT_WIDTH_2 = TFT_WIDTH / 2;

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

void setDisplayBrightnessTft_eSPI(uint8_t brightnessPercent) {
  if (brightnessPercent > 100) brightnessPercent = 100;
  if (brightnessPercent < 0) brightnessPercent = 0;
  analogWrite(LED_BACKLIGHT_PIN, 255 - (255 * ledBrightnessPercent / 100));
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
  setDisplayBrightnessTft_eSPI(ledBrightnessPercent);
  delay(10);
  tft.setRotation(DISPLAY_ORIENTATION_PORTRAIT);

  tft.fillScreen(DISPLAY_BACKGROUND_COLOR);
  tft.setTextColor(DISPLAY_FOREGROUND_COLOR, DISPLAY_BACKGROUND_COLOR);
  Serial.println("Initialization of the display done");
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
  delay(2000);

  initializeTouch();

  if (initSdCard()) {
    Serial.println("SD Card ready");
    tft.drawCentreString("SD Card ready", TFT_WIDTH_2, DISPLAY_START_Y + 4 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  } else {
    Serial.println("SD Card FAILURE");
    tft.drawCentreString("SD Card FAILURE", TFT_WIDTH_2, DISPLAY_START_Y + 4 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  }

  delay(2000);

}

void displayImage(uint8_t ind) {
  Serial.printf("display image with index %d\n", ind);
  if (ind < MAX_IMAGES) {
    // changed for LovyanGFX
    //if (jpeg.open(fileNamesJpg[ind].c_str(), myOpen, myClose, myRead, mySeek, JPEGDraw)) {
    if (jpeg.open(fileNamesJpg[ind].c_str(), myOpen, myClose, myRead, mySeek, JPEGDrawLov)) {      
      Serial.println("Display JPG");

      jpeg.decode(0, 0, 0);
      jpeg.close();
    } else {
      tft.println("JPG could not get opened!");
    }
  } else {
    drawBmp(fileNamesBmp[ind - MAX_IMAGES].c_str(), 0, 0);
  }
  tft.setTextColor(D_RED, D_BLACK);
  tft.drawCentreString("Touch Image -> next", tft.width() / 2, 20, 2);
  tft.drawCentreString("Touch Image -> next", tft.width() / 2, 290, 2);
}

void loop() {
  if (!isDisplayed) {
    isDisplayed = true;
    displayImage(imageIndex);
    imageIndex++;
    if (imageIndex >= (2 * MAX_IMAGES)) {
      // roll over
      imageIndex = 0;
    }
  }

  touchLoop();
  if (isTouch) {
    isTouch = false;
    isDisplayed = false;
  }
}
