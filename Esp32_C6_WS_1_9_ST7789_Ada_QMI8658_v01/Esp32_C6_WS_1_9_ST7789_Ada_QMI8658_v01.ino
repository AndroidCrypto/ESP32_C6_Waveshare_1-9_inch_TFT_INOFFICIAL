/*
  Hardware: Waveshare ESP32-C6 1,9-inch TFT Display 170x320 pixel with Touch

  This sketch is showing the data from the QMI8656 IME Sensor on the TFT display using the
  Adafruit ST7735 | ST7789 Graphic Library

*/

/*
Version Information
08.06.2026 V01 Initial programming
*/

// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32-C6 Waveshare 1.90-inches ST7789 170x320 QMI8658 IME Sensor Adafruit V01";
const char *PROGRAM_VERSION_SHORT = "Waveshare QMI8658 IME V01";

// --------------------------------------------------------------
// Adafruit ST7735 library

#include <SPI.h>
#include <Adafruit_GFX.h>     // Core graphics library, 
// https://github.com/adafruit/Adafruit-GFX-Library version 1.12.6
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789 
// https://github.com/adafruit/Adafruit-ST7735-Library version 1.11.0

#define TFT_WIDTH  170
#define TFT_HEIGHT 320

// Pin Definitions
#define TFT_BL   15  
#define TFT_CS   7
#define TFT_MOSI 4   
#define TFT_SCLK 5
#define TFT_MISO 19  
#define TFT_DC   6
#define TFT_RST  14
#define IMU_SDA  18
#define IMU_SCL  8
#define ST77XX_GREY 0x8410 

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const uint8_t DISPLAY_ORIENTATION_LANDSCAPE = 1; // USB connector @left side
const uint8_t TEXT_SIZE = 2;

// --------------------------------------------------------------
// QMI8658 Sensor

#include <QMI8658.h> // https://github.com/lahavg/QMI8658-Arduino-Library version 1.0.1
#include <Wire.h>

QMI8658 imu;

// Offsets & Filter Variables
float offsetAX = 0, offsetAY = 0, offsetAZ = 0;
float offsetGX = 0, offsetGY = 0, offsetGZ = 0;
float filterAX = 0, filterAY = 0, filterAZ = 0;
float filterGX = 0, filterGY = 0, filterGZ = 0;
float filterTemp = 0; 

const float alpha = 0.05; // Smoothing factor

void setup() {
    Serial.begin(115200);
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, LOW); 

    SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    tft.init(TFT_WIDTH, TFT_HEIGHT); 
    tft.setRotation(DISPLAY_ORIENTATION_LANDSCAPE); 
    tft.fillScreen(ST77XX_BLACK);
    
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(10, 10);
    tft.println("IMU Initialization...");

    if (!imu.begin(IMU_SDA, IMU_SCL)) {
        tft.println("IMU Error!");
        while (1);
    }
    
    imu.setAccelUnit_mg(true);
    imu.setGyroUnit_dps(true);
    imu.enableSensors(QMI8658_ENABLE_ACCEL | QMI8658_ENABLE_GYRO);

    QMI8658_Data startData;
    if(imu.readSensorData(startData)) filterTemp = startData.temperature;

    for (int i = 0; i < 100; i++) {
        QMI8658_Data samples;
        if (imu.readSensorData(samples)) {
            offsetAX += samples.accelX; offsetAY += samples.accelY; offsetAZ += samples.accelZ;
            offsetGX += samples.gyroX;  offsetGY += samples.gyroY;  offsetGZ += samples.gyroZ;
        }
        delay(5);
    }
    offsetAX /= 100; offsetAY /= 100; offsetAZ /= 100;
    offsetGX /= 100; offsetGY /= 100; offsetGZ /= 100;

    tft.fillScreen(ST77XX_BLACK);
    tft.drawFastHLine(0, 35, 320, ST77XX_GREY);
    tft.drawFastHLine(0, 145, 320, ST77XX_GREY);
    tft.setCursor(10, 10);
    tft.println("Waveshare QMI8658 Data");
}

void loop() {
    QMI8658_Data raw;
    if (imu.readSensorData(raw)) {
        // 1. Filtering
        filterAX = (alpha * (raw.accelX - offsetAX)) + ((1.0 - alpha) * filterAX);
        filterAY = (alpha * (raw.accelY - offsetAY)) + ((1.0 - alpha) * filterAY);
        filterAZ = (alpha * (raw.accelZ - offsetAZ)) + ((1.0 - alpha) * filterAZ);
        filterGX = (alpha * (raw.gyroX - offsetGX)) + ((1.0 - alpha) * filterGX);
        filterGY = (alpha * (raw.gyroY - offsetGY)) + ((1.0 - alpha) * filterGY);
        filterGZ = (alpha * (raw.gyroZ - offsetGZ)) + ((1.0 - alpha) * filterGZ);
        filterTemp = (0.05 * raw.temperature) + (0.95 * filterTemp);

        // 2. Display Output with units
        displayValue("AX:", filterAX, "mg", 10, 50, ST77XX_CYAN);
        displayValue("AY:", filterAY, "mg", 10, 80, ST77XX_CYAN);
        displayValue("AZ:", filterAZ, "mg", 10, 110, ST77XX_CYAN);

        // Shifted X to 160 to fit unit labels
        displayValue("GX:", filterGX, "dps", 160, 50, ST77XX_YELLOW);
        displayValue("GY:", filterGY, "dps", 160, 80, ST77XX_YELLOW);
        displayValue("GZ:", filterGZ, "dps", 160, 110, ST77XX_YELLOW);

        // Footer
        tft.setCursor(10, 153);
        tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
        tft.print("Temp: ");
        tft.printf("%.1f C", filterTemp);
    }
    delay(20);
}

void displayValue(const char* label, float value, const char* unit, int x, int y, uint16_t color) {
    tft.setCursor(x, y);
    tft.setTextColor(color, ST77XX_BLACK);
    if (abs(value) < 0.2) value = 0.0; 
    
    tft.print(label);
    tft.printf("%6.1f", value);
    tft.print(" ");
    tft.print(unit);
}
