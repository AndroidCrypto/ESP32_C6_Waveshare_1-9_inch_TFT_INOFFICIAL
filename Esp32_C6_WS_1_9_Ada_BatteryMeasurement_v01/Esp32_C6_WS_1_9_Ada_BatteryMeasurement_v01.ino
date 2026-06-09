/*
    Hardware: Waveshare ESP32-C6 1,9-inch TFT Display 170x320 pixel with Touch
    
    Measures the Battery Voltage on GPIO 0 (ADC) and displays the measured raw
    value and calculated voltage on the display. Im using a 20-samples average
    for the raw data.

    The conversion between raw values ​​and voltage is performed using a divisor 
    determined by the ratio of two resistors (a "voltage divider"). 
    Due to manufacturing tolerances, you should determine your specific value 
    yourself through a simple calibration; that is, measure the voltage at the 
    input of the battery connector on the board and adjust the voltage divider 
    so that the displayed voltage matches the measured voltage.
    -> #define BATTERY_VOLTAGE_DIVIDER 322.7

    https://ohmslawcalculator.com/voltage-divider-calculator
    As per schematic, R1 is 200K and R2 is 100K so it is dividing by 3. When
    feeding 4.5 Volts the output to the ADC input is 1.5 Volts. 
    
    Official example https://github.com/waveshareteam/ESP32-C6-LCD-1.9/tree/main/02_Example/Arduino/01_ADC_Test
    is using Attenuation ADC_ATTEN_DB_12 and Resolution ADC_BITWIDTH_12, but
    using Arduino it is analogSetAttenuation(ADC_11db);

    The Waveshare board has a green indicator LED beneath the USB connector:
    lighting in GREEN = loading the battery.

    This is using the
    Adafruit ST7735 | ST7789 Graphic Library

*/

/*
Version Information
06.06.2026 V01 Initial programming
*/

// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare 1.90-inches ST7789 170x320 ADC Battery Measurement Adafruit ST7789 V01";

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// --- PIN DEFINITIONS ---
#define BOOT_BUTTON 9
#define TFT_BL 15
#define TFT_CS 7
#define TFT_MOSI 4
#define TFT_SCLK 5
#define TFT_MISO 19
#define TFT_DC 6
#define TFT_RST 14

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// --------------------------------------------------------------
// Display control

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
const uint8_t DISPLAY_ORIENTATION_PORTRAIT = 1; // 1 = USB-Connector @left side

const int PWM_FREQUENCY_BL = 5000;  // 5 kHz für flimmerfreies Licht
const int PWM_RESOLUTION_BL = 8;    // 8 Bit Auflösung (Werte von 0 bis 255)

const uint8_t LED_BACKLIGHT_PIN = TFT_BL;  // taken from User Setup
uint8_t ledBrightnessPercent = 75;

void setDisplayBrightnessAdafruit(uint8_t brightnessPercent) {
  if (brightnessPercent > 100) brightnessPercent = 100;
  if (brightnessPercent < 0) brightnessPercent = 0;
  ledcWrite(LED_BACKLIGHT_PIN, 255 - (255 * ledBrightnessPercent / 100));
}

// -----------------------------------------------------------------------
// Battery Management

#define BATTERY_VOLTAGE_ADC_PIN 0

#define BATTERY_VOLTAGE_DIVIDER 322.7
const uint8_t READ_SAMPLES = 20;
uint16_t batteryVoltageRaw = 1;
float batteryVoltage = 1.23;
const long BATTERY_MEASUREMENT_INTERVAL = 1000;  // each second
long lastBatteryMeasurementMillis = 0;

static bool isButtonPressed = false;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(PROGRAM_VERSION);

  pinMode(BOOT_BUTTON, INPUT_PULLUP);

  // TFT backlight brightness control
  ledcAttach(LED_BACKLIGHT_PIN, PWM_FREQUENCY_BL, PWM_RESOLUTION_BL);
  // set the brightness of the display
  setDisplayBrightnessAdafruit(ledBrightnessPercent);

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.init(170, 320);
  tft.setRotation(DISPLAY_ORIENTATION_PORTRAIT);
  tft.fillScreen(D_BLACK);

  tft.setTextColor(D_WHITE, D_BLACK);
  tft.setTextSize(2);
}

void readBatteryVoltage() {
  pinMode(BATTERY_VOLTAGE_ADC_PIN, INPUT);
  analogReadResolution(12);        //Use 12 bits which would give range of 0 to 4095
  analogSetAttenuation(ADC_11db);  // this is the default value
  float measures = 0;
  for (uint8_t i = 0; i < READ_SAMPLES; i++) {
    measures += analogRead(BATTERY_VOLTAGE_ADC_PIN);
  }
  batteryVoltageRaw = measures / READ_SAMPLES;
  batteryVoltage = batteryVoltageRaw / BATTERY_VOLTAGE_DIVIDER;
  //uint32_t armv = analogReadMilliVolts(BATTERY_VOLTAGE_ADC_PIN);
  //Serial.printf("batteryVoltage: %f armv %d\n", batteryVoltage, armv);
}

void displayData() {
  tft.setTextSize(2);
  tft.setTextColor(D_WHITE, D_BLACK);
  tft.setCursor(10, 10);
  tft.print("Battery Measurement");
  tft.setCursor(10, 60);
  tft.printf("Raw value: %d  ", batteryVoltageRaw);
  tft.setCursor(10, 110);
  if (batteryVoltage > 4.3) {
    tft.printf("Voltage: %4.2f Volts (USB)", batteryVoltage);
  } else {
    tft.printf("Voltage: %4.2f Volts       ", batteryVoltage);
  }
}

void loop() {
  // --- Reserved for any LOGIC (Debounced) ---
  if (digitalRead(BOOT_BUTTON) == LOW) {
    if (!isButtonPressed) {
      isButtonPressed = true;
    }
  } else {
    isButtonPressed = false;
  }

  if (millis() - lastBatteryMeasurementMillis > BATTERY_MEASUREMENT_INTERVAL) {
    readBatteryVoltage();
    displayData();
    Serial.printf("Battery Voltage Raw: %d = %4.2f Volts\n", batteryVoltageRaw, batteryVoltage);
    lastBatteryMeasurementMillis = millis();
  }
}
