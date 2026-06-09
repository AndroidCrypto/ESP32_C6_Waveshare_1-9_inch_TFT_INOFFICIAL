/*
  Setup file for a ST7789 1.3-inches 240 x 240 pixels display with CS pin.
  This is mounted on a BLACK PCB with these pin connectors, seen from display's side:
  Pin order is from LEFT to RIGHT
  01 GND
  02 VCC 3.3
  03 SCK (SCLK, CLK)
  04 SDA (MOSI)
  05 RESET
  06 DC (Data / Command switch)
  07 CS (Chip Select)
  08 BLK (LED Backlight)

  Japanese -> Englisch translations using Google Translator by AndroidCrypto
*/

// Example of settings when using LovyanGFX with your own settings on ESP32
// ESP32でLovyanGFXを独自設定で利用する場合の設定例

// Create a class that performs your own settings by deriving it from LGFX_Device. 独自の設定を行うクラスを、LGFX_Deviceから派生して作成します。
class LGFX : public lgfx::LGFX_Device {
  /*
  You can change the class name from "LGFX" to something else.
  When used in conjunction with AUTODETECT, "LGFX" is used, so please change the name to something other than LGFX.
  Also, if you are using multiple panels at the same time, please give each one a different name.
  If you change the class name, you must also change the constructor name to the same name.

 クラス名は"LGFX"から別の名前に変更しても構いません。
 AUTODETECTと併用する場合は"LGFX"は使用されているため、LGFX以外の名前に変更してください。
 また、複数枚のパネルを同時使用する場合もそれぞれに異なる名前を付けてください。
 ※ クラス名を変更する場合はコンストラクタの名前も併せて同じ名前に変更が必要です。

  You can name them however you like, but in case the number of settings increases,
  For example, if you are configuring an SPI-connected ILI9341 on the ESP32 DevKit-C, you can name them like
  LGFX_DevKitC_SPI_ILI9341
  By matching the file name and class name, you will avoid confusion when using them.

 名前の付け方は自由に決めて構いませんが、設定が増えた場合を想定し、
 例えばESP32 DevKit-CでSPI接続のILI9341の設定を行った場合、
  LGFX_DevKitC_SPI_ILI9341
 のような名前にし、ファイル名とクラス名を一致させておくことで、利用時に迷いにくくなります。
/*/


  // Prepare an instance that matches the type of panel you want to connect. 接続するパネルの型にあったインスタンスを用意します。
  //lgfx::Panel_GC9A01      _panel_instance;
  //lgfx::Panel_GDEW0154M09 _panel_instance;
  //lgfx::Panel_HX8357B     _panel_instance;
  //lgfx::Panel_HX8357D     _panel_instance;
  //lgfx::Panel_ILI9163     _panel_instance;
  //lgfx::Panel_ILI9341     _panel_instance;
  //lgfx::Panel_ILI9342     _panel_instance;
  //lgfx::Panel_ILI9481     _panel_instance;
  //lgfx::Panel_ILI9486     _panel_instance;
  //lgfx::Panel_ILI9488     _panel_instance;
  //lgfx::Panel_IT8951      _panel_instance;
  //lgfx::Panel_RA8875      _panel_instance;
  //lgfx::Panel_SH110x      _panel_instance; // SH1106, SH1107
  //lgfx::Panel_SSD1306     _panel_instance;
  //lgfx::Panel_SSD1327     _panel_instance;
  //lgfx::Panel_SSD1331     _panel_instance;
  //lgfx::Panel_SSD1351     _panel_instance; // SSD1351, SSD1357
  //lgfx::Panel_SSD1963     _panel_instance;
  //lgfx::Panel_ST7735      _panel_instance;
  //lgfx::Panel_ST7735S     _panel_instance;
  lgfx::Panel_ST7789 _panel_instance;
  //lgfx::Panel_ST7796      _panel_instance;

  // Prepare an instance that matches the type of bus to which you want to connect the panel.
  // パネルを接続するバスの種類にあったインスタンスを用意します。
  lgfx::Bus_SPI _bus_instance;  // SPI bus instance SPIバスのインスタンス
                                //lgfx::Bus_I2C        _bus_instance;   // I2C bus instance I2Cバスのインスタンス
                                //lgfx::Bus_Parallel8  _bus_instance;   // 8-bit parallel bus instance 8ビットパラレルバスのインスタンス

  // If backlight control is possible, prepare an instance. (If not necessary, delete it.)
  // バックライト制御が可能な場合はインスタンスを用意します。(必要なければ削除)
  lgfx::Light_PWM _light_instance;

  // Prepare an instance that matches the touchscreen type. (Delete if not needed)
  // タッチスクリーンの型にあったインスタンスを用意します。(必要なければ削除)
  //lgfx::Touch_CST816S          _touch_instance;
  //lgfx::Touch_FT5x06           _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
  //lgfx::Touch_GSL1680E_800x480 _touch_instance; // GSL_1680E, 1688E, 2681B, 2682B
  //lgfx::Touch_GSL1680F_800x480 _touch_instance;
  //lgfx::Touch_GSL1680F_480x272 _touch_instance;
  //lgfx::Touch_GSLx680_320x320  _touch_instance;
  //lgfx::Touch_GT911            _touch_instance;
  //lgfx::Touch_STMPE610         _touch_instance;
  //lgfx::Touch_TT21xxx          _touch_instance; // TT21100
  //lgfx::Touch_XPT2046          _touch_instance;

public:

  // Create a constructor and set various settings here.
  // If you change the class name, please specify the same name for the constructor.
  // コンストラクタを作成し、ここで各種設定を行います。
  // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
  LGFX(void) {
    {  // Configure the bus control settings.
      // バス制御の設定を行います。
      auto cfg = _bus_instance.config();  // Get the bus configuration structure. バス設定用の構造体を取得します。

      // Configuring the SPI bus SPIバスの設定
      // Select the SPI to use ESP32-S2,C3: SPI2_HOST or SPI3_HOST / ESP32: VSPI_HOST or HSPI_HOST
      cfg.spi_host = SPI2_HOST;
      //cfg.spi_host = VSPI_HOST;     // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      //cfg.spi_host = HSPI_HOST;     // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // * Due to the ESP-IDF version upgrade, the VSPI_HOST and HSPI_HOST descriptions are deprecated, so if an error occurs, please use SPI2_HOST and SPI3_HOST instead.
      // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
      cfg.spi_mode = 3;  // Sets the SPI communication mode (0 to 3) SPI通信モードを設定 (0 ~ 3)
      // SPI clock during transmission (Maximum 80MHz, rounded to an integer value of 80MHz)
      //cfg.freq_write = 40000000;  // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      cfg.freq_write = 80000000;  // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      //cfg.freq_write = 55000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      cfg.freq_read = 16000000;  // SPI clock when receiving 受信時のSPIクロック
      cfg.spi_3wire = true;      // Set to true if receiving is done on the MOSI pin 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock = false;      // Set to true if you want to use transaction locking. トランザクションロックを使用する場合はtrueを設定
      // Set the DMA channel to use (0 = no DMA / 1 = 1ch / 2 = ch / SPI_DMA_CH_AUTO = automatic setting)
      cfg.dma_channel = SPI_DMA_CH_AUTO;  // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
      // * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is now recommended for the DMA channel. Specifying 1ch or 2ch is no longer recommended.
      // ※ ESP-IDFバージョンアップに伴い、DMAチャンネルはSPI_DMA_CH_AUTO(自動設定)が推奨になりました。1ch,2chの指定は非推奨になります。
      cfg.pin_sclk = 5;   // set SPI SCLK pin number SCK
      cfg.pin_mosi = 4;   // set MOSI pin number of SPI SDI
      cfg.pin_miso = -1;  // set SPI's MISO pin number (-1 = disable) SDO
      cfg.pin_dc = 6;     // set SPI D/C pin number (-1 = disable) RS
      // If you use the same SPI bus as the SD card, be sure to set MISO without omitting it.
      // SDカードと共通のSPIバスを使う場合、MISOは省略せず必ず設定してください。

      _bus_instance.config(cfg);               // The setting value is reflected in the bus. 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance);  // Place the bus on the panel. バスをパネルにセットします。
    }

    {  // Configure the display panel control settings.
      // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config();  // Gets the structure for display panel settings. 表示パネル設定用の構造体を取得します。

      cfg.pin_cs = 7;     // pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = 14;   // pin number to which RST is connected (-1 = disable)
      cfg.pin_busy = -1;  // pin number to which BUSY is connected (-1 = disable)

      // * The following settings are set to general default values ​​for each panel, so please try commenting out any items you are unsure about.
      // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。
      cfg.panel_width = 170;      // actual displayable width
      cfg.panel_height = 320;     // actual displayable height
      cfg.offset_x = 35;          // Panel offset in the X direction パネルのX方向オフセット量
      cfg.offset_y = 0;           // Panel offset in the Y direction パネルのY方向オフセット量
      cfg.offset_rotation = 0;    // Rotation direction offset 0~7 (4~7 are upside down) 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel = 16;  // Number of dummy read bits before pixel readout ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits = 1;    // Number of dummy read bits before reading non-pixel data ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable = true;        // Set to true if data can be read データ読出しが可能な場合 trueに設定
      cfg.invert = true;          // Set to true if the panel's brightness is reversed パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order = false;      // Set to true if the red and blue of the panel are swapped. パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit = false;     // Set to true for panels that transmit data in 16-bit units via 16-bit parallel or SPI. 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
      // If the bus is shared with the SD card, set it to true (bus control is performed using drawJpgFile, etc.)
      cfg.bus_shared = false;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

      // The following settings should only be used if the display is misaligned when using a driver with a variable pixel count, such as ST7735 or ILI9163.
      // 以下はST7735やILI9163のようにピクセル数が可変のドライバで表示がずれる場合にのみ設定してください。
      //  cfg.memory_width     =   135;  // Maximum width supported by the driver IC ドライバICがサポートしている最大の幅
      //  cfg.memory_height    =   240;  // Maximum height supported by the driver IC ドライバICがサポートしている最大の高さ

      _panel_instance.config(cfg);
    }

    {  // Set the backlight control (delete if not required).
      // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config();  // Gets the backlight settings structure. バックライト設定用の構造体を取得します。

      cfg.pin_bl = 15;      // The pin number to which the backlight is connected バックライトが接続されているピン番号
      cfg.invert = false;   // true if the backlight brightness is inverted バックライトの輝度を反転させる場合 true
      cfg.freq = 44100;     // Backlight PWM frequency バックライトのPWM周波数
      cfg.pwm_channel = 7;  // PWM channel number to use 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // Place the backlight on the panel. バックライトをパネルにセットします。
    }

    setPanel(&_panel_instance);  // Set the panel to be used. 使用するパネルをセットします。
  }
};
