# bluefruit-nrf52-Galvanic-Skin-Resistance-BLE-Logger

Using:
* Seeedstudio GSR sensor with analog output Grove connector
* Adalogger Featherwing, utilizing both SD card and RTC
* nrf52832 BLE Feather

Logs CSV file to SD card, with first line header, every X seconds.
    #define interval_Logging_Seconds 10
    
Currently does not offer sensor readings via bluetooth
