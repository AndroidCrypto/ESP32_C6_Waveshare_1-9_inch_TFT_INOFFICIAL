/*
  Hardware: Waveshare ESP32-C6 1,9-inch TFT Display 170x320 pixel with Touch

  This sketch is showing 6 buttons on the display. After touching the button
  its color changes from RED to GREEN and set the GPIO Extender output on
  HIGH. After a second press, the color changes back to RED and the GPIO
  Extender output is LOW.
  
  This is using the
  Adafruit ST7735 | ST7789 Graphic Library

*/

/*
Version Information
08.06.2026 V01 Initial programming
*/


// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare 1.90-inches ST7789 170x320 XCA9554 GPIO Expander Touch Adafruit V01";
const char *PROGRAM_VERSION_SHORT = "Waveshare XCA9554 GPIO Expander V01";

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
uint8_t ledBrightnessPercent = 75;

// --------------------------------------------------------------
// XCA9554 GPIO Expander

#include <Wire.h>
#include <Adafruit_XCA9554.h>  // https://github.com/adafruit/Adafruit_xCA9554 version 1.0

#define I2C_SDA 18
#define I2C_SCL 8
#define I2C_TCA9554_ADDRESS 0x20

// Create an instance of the Adafruit_XCA9554 class
Adafruit_XCA9554 expander;

const uint8_t MIN_EXIO = 0;
const uint8_t MAX_EXIO = 7;

// --------------------------------------------------------------
// Touch

#include "WAVESHARE_TOUCH.h"
// Note: don't use 'initializeTouch()' a second time, is done for the GPIO Expander

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
// Button control

Adafruit_GFX_Button buttons[8];
bool buttonStates[8] = { false };
const char *labels[8] = { "0", "1", "2", "3", "4", "5", "6", "7" };

// Layout Parameter
const int btnW = 70;
const int btnH = 70;
const int gapX = 8;
const int gapY = 10;

// --------------------------------------------------------------
// Display control

const int PWM_FREQUENCY_BL = 5000;
const int PWM_RESOLUTION_BL = 8;

void setDisplayBrightnessAdafruit(uint8_t brightnessPercent) {
  if (brightnessPercent > 100) brightnessPercent = 100;
  if (brightnessPercent < 0) brightnessPercent = 0;
  ledcWrite(LED_BACKLIGHT_PIN, 255 - (255 * ledBrightnessPercent / 100));
}

const uint8_t DISPLAY_ORIENTATION_LANDSCAPE = 1;
const uint16_t DISPLAY_BACKGROUND_COLOR = D_BLACK;
const uint16_t DISPLAY_FOREGROUND_COLOR = D_WHITE;
const uint8_t DISPLAY_FONT_TYPE = 4;
const uint8_t DISPLAY_FONT_TYPE_SMALL = 2;
const uint8_t TEXT_SIZE = 2;
const uint8_t LINE_DISTANCE = 25;
const uint8_t DISPLAY_START_X = 5;
const uint8_t DISPLAY_START_Y = 9;
const uint8_t DISPLAY_DISTANCE = 26;

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
  tft.setRotation(DISPLAY_ORIENTATION_LANDSCAPE);

  tft.fillScreen(DISPLAY_BACKGROUND_COLOR);
  tft.setTextColor(DISPLAY_FOREGROUND_COLOR, DISPLAY_BACKGROUND_COLOR);
  Serial.println("Initialization of the display done");
}

void drawTheButtons() {
  for (int i = 0; i < 8; i++) {
    int col = i % 4;
    int row = i / 4;

    // center of the buttons (x, y)
    int x = gapX + btnW / 2 + (col * (btnW + gapX));
    int y = gapY + btnH / 2 + (row * (btnH + gapY));

    buttons[i].initButton(&tft,
                          x, y,        // center
                          btnW, btnH,  // dimensions
                          D_WHITE,     // frame color
                          D_RED,       // color off
                          D_WHITE,     // txt color
                          (char *)labels[i], 2);
    buttons[i].drawButton();
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

  Wire.begin(I2C_SDA, I2C_SCL);
  // slow down the frequency for better user experience with the touch display
  Wire.setClock(100000);
  Serial.println("PCA/TCA9554 Simple Test Sketch");

  // begin communication with the expander
  if (!expander.begin(I2C_TCA9554_ADDRESS)) {  // Replace with actual I2C address if different
    Serial.println("Failed to find XCA9554 chip");
    while (1)
      ;
  } else {
    Serial.println("XCA9554 initialized");
  }

  // configure pins as an output and off
  for (uint8_t i = MIN_EXIO; i <= MAX_EXIO; i++) {
    expander.pinMode(i, OUTPUT);
    expander.digitalWrite(i, false);  // LOW/OFF
  }

  // initializeTouch(); // I2C initializations already done

  // TFT backlight brightness control
  ledcAttach(LED_BACKLIGHT_PIN, PWM_FREQUENCY_BL, PWM_RESOLUTION_BL);

  initializeDisplay();
  tft.setTextSize(1);
  drawCentreString(PROGRAM_VERSION_SHORT, TFT_WIDTH_2, DISPLAY_START_Y + 0 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE_SMALL);
  drawCentreString("Display init done", TFT_WIDTH_2, DISPLAY_START_Y + 1 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE_SMALL);
  tft.setTextSize(TEXT_SIZE);
  drawCentreString("Using", TFT_WIDTH_2, DISPLAY_START_Y + 2 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  drawCentreString(DISPLAY_LIBRARY, TFT_WIDTH_2, DISPLAY_START_Y + 3 * DISPLAY_DISTANCE, DISPLAY_FONT_TYPE);
  delay(1000);
  tft.fillScreen(DISPLAY_BACKGROUND_COLOR);
  drawTheButtons();

}

void loop() {
  touchLoop();
  if (isTouch) {
    isTouch = false;

    for (int i = 0; i < 8; i++) {
      // as the display is in landscape mode, the coordinates are switched and mirrored
      if (buttons[i].contains(320 - touchMappedY, touchMappedX)) {
        buttonStates[i] = !buttonStates[i];

        uint16_t newColor = buttonStates[i] ? D_GREEN : D_RED;

        int x = gapX + btnW / 2 + ((i % 4) * (btnW + gapX));
        int y = gapY + btnH / 2 + ((i / 4) * (btnH + gapY));

        buttons[i].initButton(&tft, x, y, btnW, btnH,
                              D_WHITE, newColor, D_WHITE,
                              (char *)labels[i], 2);

        buttons[i].drawButton();

        expander.digitalWrite(i, buttonStates[i]);

        delay(300);
        break;
      }
    }
  }
}
