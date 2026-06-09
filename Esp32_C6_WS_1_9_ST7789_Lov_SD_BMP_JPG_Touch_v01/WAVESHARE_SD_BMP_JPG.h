
#include <SPI.h>

#include <SD.h>
// Pin for SD Card
#define SD_CS 20  // Chip Select for SD Card Reader

// LovyanGFX
#define SDCARD_SPI SPI
// as we are sharing the bus this is the same 
#define SD_MOSI   4 // = SDA = DIN
#define SD_SCLK   5
#define SD_MISO  19 // Sharing the SPI bus with SD Card
SPIClass sdSPI = SPIClass(SPI2_HOST);


#include <JPEGDEC.h> // https://github.com/bitbank2/JPEGDEC
JPEGDEC jpeg;
File jpgFile;

bool initSdCard() {
  if (!SD.begin(SD_CS)) {
    Serial.println("Error: SD-Initialization failed!");
    return false;
  } else {
    Serial.println("SD-Initialization success!");
    return true;
  }
}

bool initSdCardLovyanGFX() {
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  SD.end();
  //if (!SD.begin(SD_CS)) {
  if (!SD.begin(SD_CS, sdSPI, 25000000)) {
    Serial.println("Error: SD-Initialization failed!");
    return false;
  } else {
    Serial.println("SD-Initialization success!");
    return true;
  }
}

// BMP
void drawBmp(const char *filename, int16_t x, int16_t y) {
  File bmpFile = SD.open(filename);
  if (!bmpFile) return;

  uint32_t dataOffset;
  bmpFile.seek(10);
  bmpFile.read((uint8_t *)&dataOffset, 4);

  int32_t w, h;
  bmpFile.seek(18);
  bmpFile.read((uint8_t *)&w, 4);
  bmpFile.read((uint8_t *)&h, 4);

  int rowSize = (w * 3 + 3) & ~3;
  int padding = rowSize - (w * 3);

  bmpFile.seek(dataOffset);

  for (int row = h - 1; row >= 0; row--) {
    for (int col = 0; col < w; col++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();

      if (col < 170 && row < 320) {
        tft.drawPixel(x + col, y + row, tft.color565(r, g, b));
      }
    }
    if (padding > 0) bmpFile.seek(bmpFile.position() + padding);
  }
  bmpFile.close();
}

// JPG

// Callbacks for getting access to the SD Card file system
// --- Helper Funktionen für JPEGDEC, damit es die SD-Karte versteht ---
void *myOpen(const char *filename, int32_t *size) {
  jpgFile = SD.open(filename);
  *size = jpgFile.size();
  return &jpgFile;
}
void myClose(void *handle) {
  jpgFile.close();
}
int32_t myRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
  return jpgFile.read(buffer, length);
}
int32_t mySeek(JPEGFILE *handle, int32_t position) {
  return jpgFile.seek(position);
}

int JPEGDrawLov(JPEGDRAW *pDraw) {
  tft.setSwapBytes(true);
  tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);
  return 1;
}
