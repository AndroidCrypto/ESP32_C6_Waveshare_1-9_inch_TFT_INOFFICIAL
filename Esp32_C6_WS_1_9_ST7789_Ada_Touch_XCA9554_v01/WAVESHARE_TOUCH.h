/*
  This is a very basic library to work with the Touch controller of
  the Waveshare ESP32-C6 1.9 inch TFT Display ST7789 with a resolution
  of 170 x 320 pixel.

  As both, the INT/IRQ and RST are not connected between the CST816 and
  the ESP32-C6 chips, you can't use any of the libraries you can find
  for Arduino/ESP32.
*/

#include <Wire.h>

#define I2C_SDA 18
#define I2C_SCL 8
#define I2C_ADDR_TOUCH 0x15

// Mapping
#define X_MIN 40
#define Y_MIN 15
#define X_MAX 150
#define Y_MAX 310

// please define this manual
#define TOUCH_SCREEN_WIDTH 170
#define TOUCH_SCREEN_HEIGHT 320

bool isTouch = false;
bool isReleased = true;
int touchX, touchY;
int touchMappedX, touchMappedY;
uint8_t touchPress;  // pressure
int lastX = -1, lastY = -1;
unsigned long lastChangeTime = 0;

void initializeTouch() {
  Wire.begin(I2C_SDA, I2C_SCL);
  // lower the I2C clock as the CST816 may get problems with 400kHz without RST pin
  Wire.setClock(100000);
  Serial.println("initializeTouch done");
}

void touchLoop() {
  Wire.beginTransmission(I2C_ADDR_TOUCH);
  Wire.write(0x02);  // Start bei Touch-Point Register
  if (Wire.endTransmission() == 0) {
    Wire.requestFrom(I2C_ADDR_TOUCH, 5);
    if (Wire.available() >= 5) {
      touchPress = Wire.read();
      uint8_t xh = Wire.read();
      uint8_t xl = Wire.read();
      uint8_t yh = Wire.read();
      uint8_t yl = Wire.read();

      touchX = ((xh & 0x0F) << 8) | xl;
      touchY = ((yh & 0x0F) << 8) | yl;

      // Mapping
      touchMappedX = map(touchX, X_MIN, X_MAX, 0, TOUCH_SCREEN_WIDTH);
      touchMappedY = map(touchY, Y_MIN, Y_MAX, 0, TOUCH_SCREEN_HEIGHT);

      // Unbedingt begrenzen!
      touchMappedX = constrain(touchMappedX, 0, TOUCH_SCREEN_WIDTH);
      touchMappedY = constrain(touchMappedY, 0, TOUCH_SCREEN_HEIGHT);

      // Logik: Nur ausgeben, wenn sich die Werte ändern
      // ODER wenn seit der letzten Änderung weniger als 100ms vergangen sind
      if (touchPress > 0 && (touchX != lastX || touchY != lastY)) {
        Serial.printf("TOUCH -> X: %d | Y: %d\n", touchX, touchY);
        Serial.printf("TchMap-> X: %3d | Y: %3d\n", touchMappedX, touchMappedY);
        isTouch = true;
        isReleased = false;
        lastX = touchX;
        lastY = touchY;
        lastChangeTime = millis();
      }
      // Wenn 150ms keine Änderung kam, gehen wir davon aus, dass der Finger weg ist
      else if (millis() - lastChangeTime > 150) {
        if (lastX != -1) {
          Serial.println("RELEASE -> Finger lifted");
          isReleased = true;
          lastX = -1;
          lastY = -1;
        }
      }
    }
  }
}