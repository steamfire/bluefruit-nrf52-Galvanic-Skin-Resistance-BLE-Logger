
#include <ArduinoLog.h>
#include <bluefruit.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

#define GSR_Pin A0
#define interval_Logging_Seconds 10


// Declare a mutex Semaphore Handle which we will use to manage shared hardware resources.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSDSemaphoreMR;

/* Automation IO Service Definitions
   Automation IO Service:  0x1815
   Analog Read Characteristic: 0x2A58
*/
BLEService        aios = BLEService(0x1815);
BLECharacteristic aioc = BLECharacteristic(0x2A58);

// BLE Client Current Time Service
BLEClientCts  bleCTime;
BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance
BLEUart bleuart; // uart over ble


// Realtime Clock
RTC_PCF8523 rtc;

uint16_t gsr_measured = 0;
uint16_t gsr_old = 0;
float battVolts = 0;
uint8_t battPct = 0;
uint16_t conn_handleCTS = 0;
uint8_t afterStartup = 0;

// Advanced function prototypes
void startAdv(void);
void setupAIO(void);
void cccd_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);


void setup()
{
  setupRTOS();
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println(F("\n\n\n\n\tCompiled:" __DATE__ " " __TIME__ "\n\t" __FILE__ )) ;
  // Pass log level, whether to show log level, and print interface.
  // Available levels are:
  // LOG_LEVEL_SILENT, LOG_LEVEL_FATAL, LOG_LEVEL_ERROR, LOG_LEVEL_WARNING, LOG_LEVEL_NOTICE, LOG_LEVEL_TRACE, LOG_LEVEL_VERBOSE
  // Note: if you want to fully remove all logging code, uncomment #define DISABLE_LOGGING in Logging.h
  //       this will significantly reduce your project size
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.setPrefix(printTimestamp); // Uncomment to get timestamps as prefix
  //Log.setSuffix(printNewline); // Uncomment to get newline as suffix
  Log.notice(F("Hello.\n") );
  setupSDLog();
  setupTime();
  startBluetooth();
  setRTCLoop();

  // Get a single ADC sample and throw it away
  readVBAT();
  afterStartup = 1;

  // Create loop2() using Scheduler to run in 'parallel' with loop()
  Scheduler.startLoop(loop2);
}


void loop()
{
  gsr_measured = getGSR();
  if (gsr_old == gsr_measured) {
    ; // do nothing special
  } else {
    updateAIO(gsr_measured);  // Update the bluetooth low energy available value
    gsr_old = gsr_measured;
  }
  battVolts = battCheck();
  updateBatt(battPct);
}

void loop2() {
  analogLog();  // save stored values to SD card
  long logDelay = interval_Logging_Seconds * 1000;
  delay(logDelay);

}


void printTimestamp(Print* _logOutput) {
  char c[12];
  int m = sprintf(c, "%10lu ", millis());
  _logOutput->print(c);
}
