/*
  Hardware: Waveshare ESP32-C6 1,9-inch TFT Display 170x320 pixel with Touch

  This sketch is showing BMP and JPG images in portrait mode on the TFT display.
  The next image is displayed after touching the display.
  
  This is using the
  Adafruit ST7735 | ST7789 Graphic Library

*/

/*
Version Information
06.06.2026 V01 Initial programming
*/

// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare 1.90-inches ST7789 170x320 SD read BMP & JPG Touch Adafruit V01";
const char *PROGRAM_VERSION_SHORT = "Waveshare SD|Touch V01";

// --------------------------------------------------------------
// Adafruit ST7735 library

#include <SPI.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789

#define TFT_WIDTH 170
#define TFT_HEIGHT 320

#define TFT_BL 15  // LED back-light
#define TFT_CS 7
#define TFT_MOSI 4  // = SDA = DIN
#define TFT_SCLK 5
#define TFT_MISO 19  // Sharing the SPI bus with SD Card
#define TFT_DC 6
#define TFT_RST 14  // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char *DISPLAY_LIBRARY = "Adafruit";
const uint8_t LED_BACKLIGHT_PIN = TFT_BL;  // taken from User Setup
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

const uint16_t TFT_WIDTH_2 = TFT_WIDTH / 2;

// colors
const uint16_t D_BLACK = ST77XX_BLACK;
const uint16_t D_WHITE = ST77XX_WHITE;
const uint16_t D_RED = ST77XX_RED;
const uint16_t D_GREEN = ST77XX_GREEN;
const uint16_t D_BLUE = ST77XX_BLUE;
const uint16_t D_YELLOW = ST77XX_YELLOW;
const uint16_t D_MAGENTA = ST77XX_MAGENTA;
const uint16_t D_ORANGE = ST77XX_ORANGE;
const uint16_t D_SKYBLUE = 0x867D;
const uint16_t D_CYAN = ST77XX_CYAN;

// --------------------------------------------------------------
// Display control

void setDisplayBrightnessAdafruit(uint8_t brightnessPercent) {
  if (brightnessPercent > 100) brightnessPercent = 100;
  if (brightnessPercent < 0) brightnessPercent = 0;
  analogWrite(LED_BACKLIGHT_PIN, 255 - (255 * ledBrightnessPercent / 100));
}

const uint8_t DISPLAY_ORIENTATION_PORTRAIT = 2;  // USB connector @bottom
const uint16_t DISPLAY_BACKGROUND_COLOR = D_BLACK;
const uint16_t DISPLAY_FOREGROUND_COLOR = D_WHITE;
const uint8_t DISPLAY_FONT_TYPE = 4;
const uint8_t DISPLAY_FONT_TYPE_SMALL = 2;
const uint8_t TEXT_SIZE = 2;
const uint8_t LINE_DISTANCE = 25;
const uint8_t DISPLAY_START_Y = 10;
const uint8_t DISPLAY_DISTANCE = 30;

// The fontType is a dummy to keep the parameters same to TFT_eSPI
void drawCentreString(const char *buf, int16_t x, int16_t y, uint8_t fontType) {
  int16_t x1, y1;
  uint16_t w, h;

  // calculate the width and height of a text
  tft.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);

  // set the cursor
  tft.setCursor(x - w / 2, y);
  tft.print(buf);
}

void initializeDisplay() {
  // Changing the Hardware SPI pins
  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.init(TFT_WIDTH, TFT_HEIGHT);
  // set the brightness of the display
  setDisplayBrightnessAdafruit(ledBrightnessPercent);
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

  initializeTouch();

  initializeDisplay();
  tft.setTextSize(1);
  drawCentreString(PROGRAM_VERSION_SHORT, TFT_WIDTH_2, DISPLAY_START_Y + 0 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE_SMALL);
  drawCentreString("Display init done", TFT_WIDTH_2, DISPLAY_START_Y + 1 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE_SMALL);
  tft.setTextSize(TEXT_SIZE);
  drawCentreString("Using", TFT_WIDTH_2, DISPLAY_START_Y + 2 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  drawCentreString(DISPLAY_LIBRARY, TFT_WIDTH_2, DISPLAY_START_Y + 3 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  delay(2000);

  if (initSdCard()) {
    drawCentreString("SD Card ready", TFT_WIDTH_2, DISPLAY_START_Y + 4 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  } else {
    drawCentreString("SD Card FAILURE", TFT_WIDTH_2, DISPLAY_START_Y + 4 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  }
  delay(2000);
}

void displayImage(uint8_t ind) {
  Serial.printf("display image with index %d\n", ind);
  if (ind < MAX_IMAGES) {
    //if (jpeg.open("/gps_170_320.jpg", myOpen, myClose, myRead, mySeek, JPEGDraw)) {
    if (jpeg.open(fileNamesJpg[ind].c_str(), myOpen, myClose, myRead, mySeek, JPEGDraw)) {
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
  tft.setTextSize(2);
  drawCentreString("Touch Image", tft.width() / 2, 20, 2);
  drawCentreString("-> next", tft.width() / 2, 36, 2);
  drawCentreString("Touch Image", tft.width() / 2, 280, 2);
  drawCentreString("-> next", tft.width() / 2, 296, 2);
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
