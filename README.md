# ESP32-C6 Waveshare 1,9 inch TFT Display -= INOFFICIAL =-

This is an **unofficial repository** for the **Waveshare ESP32-C6 development board with a 1.9-inch TFT display and a resolution of 170 x 320 pixels**. It contains example programs for all the board's components for the **Arduino IDE**. The board is available in two versions: **without a touch interface** (but with an RGB LED) or **with a touch interface** and an external antenna connector. My examples refer entirely to the board with the touch surface.

You can find the **manufacturer's official repository** via this link: https://github.com/waveshareteam/ESP32-C6-LCD-1.9

This repository accompanies the article "**A detailed look at the Waveshare ESP32-C6 1.9-inch ST7789 TFT display with touch interface Development Board**" published here: <soon>

In the **[Documentation](./Documentation)**" folder, you will find files and descriptions taken from the [original repository](https://github.com/waveshareteam/ESP32-C6-LCD-1.9) or the [manufacturer's shop page](https://www.waveshare.com/esp32-c6-lcd-1.9.htm) and provided here as copies. **Naturally, all rights remain with the manufacturer.**

![Image 2](./images/esp32_c6_waveshare_lcd_1_9_pinout_700w.png)

## Complete Pin Assignments of the board

````plaintext
TFT Display (SPI):
TFT-Backlight: GPIO 15 (LOW = ON)
TFT-CS       : GPIO  7
TFT-MOSI     : GPIO  4 // = SDA
TFT-SCLK     : GPIO  5   
TFT-MISO     : GPIO 19 // Not connected
TFT-DC       : GPIO  6
TFT-RST      : GPIO 14

Touch surface (I2C):
I2C-SDA      : GPIO 18
I2C-SCL      : GPIO  8   
I2C-ADDR     : 0x15

Micro SD-Card Reader (SPI):
SD_CS        : GPIO 20
Sd-MOSI      : GPIO  4 // = SDA
SD-SCLK      : GPIO  5   
SD-MISO      : GPIO 19

QMI8658 IMU Sensor (I2C):
I2C-SDA      : GPIO 18
I2C-SCL      : GPIO  8
I2C-ADDR     : 0x6B
IMU-INT1     : GPIO  1
IMU-INT2     : GPIO  2

TCA9554 GPIO Extender (I2C):
I2C-SDA      : GPIO 18
I2C-SCL      : GPIO  8
I2C-ADDR     : 0x20

Buttons:
Boot Button  : GPIO  9

Battery Voltage measurement:
ADC          : GPIO  0

Miscellaneous:
UART 0 TX    : GPIO 16
UART 0 RX    : GPIO 17
USB D+       : GPIO 13
USB D-       : GPIO 12
````

## Sketches

To drive the display, I use three graphics libraries: **Adafruit ST7735|ST7789** ("Ada"), **LovyanGFX** ("Lov"), and **TFT_eSPI** ("Tft", modified version). I have written examples for all three libraries, identifiable by these abbreviations.

### Display Information
Esp32_C6_WS_1_9_ST7789_xxx_DisplayInfo_v01: Ada | Lov | Tft folder

### Touch Surface
Esp32_C6_WS_1_9_ST7789_Tft_Touch_v01: Tft (only) folder

### Micro SD-Card Reader
Esp32_C6_WS_1_9_ST7789_xxx_SD_BMP_JPG_Touch_v01: Ada | Lov | Tft folder

### QMI8656 IMU
xxx

### TCA9554 GPIO Extender
xxx

### Battery Voltage Measurement
Esp32_C6_WS_1_9_Ada_BatteryMeasurement_v01: Ada (only) folder

## Download Links und Versions of the software and libraries

- Arduino IDE 2.3.8: https://www.arduino.cc/en/software
- esp32 Boards 3.3.8 (based on ESP-IDF v5.5.4): https://github.com/espressif/arduino-esp32
- Adafruit ST7735|ST7789 1.11.0: https://github.com/adafruit/Adafruit-ST7735-Library
- Adafruit GFX 1.12.6: https://github.com/adafruit/Adafruit-GFX-Library
- TFT_eSPI 2.5.43: https://github.com/Bodmer/TFT_eSPI (gepatchte Version)
- TFT_eSPI (modified version) based on 2.5.4: https://github.com/AndroidCrypto/TFT_eSPI
- LovyanGFX 1.2.21: https://github.com/lovyan03/LovyanGFX
- CST816 Bibliothek (ohne Version): selbst erstellt
- SD SD-Card Reader 1.3.0: https://github.com/arduino-libraries/SD (ist in esp32 Boards enthalten)
- JPEGDEC JPG Bibliothek 1.8.4: https://github.com/bitbank2/JPEGDEC
- QMI8658 Arduino Library 1.0.1: https://github.com/lahavg/QMI8658-Arduino-Library
- Adafruit xCA9554 1.0.0: https://github.com/adafruit/Adafruit_XCA9554
- Waveshare Manufacturer Shopseite: https://www.waveshare.com/esp32-c6-lcd-1.9.htm
- Waveshare Hersteller GitHub Repository: https://github.com/waveshareteam/ESP32-C6-LCD-1.9

## Development Environment (Arduino)
````plaintext
Arduino IDE Version 2.3.8 (Windows)
arduino-esp32 boards Version 3.3.8 (https://github.com/espressif/arduino-esp32) that is based on Espressif ESP32 Version 5.5.1
````
