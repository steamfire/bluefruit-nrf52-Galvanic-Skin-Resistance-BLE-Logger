
#include <ArduinoLog.h>
#include <bluefruit.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

#define GSR_Pin A0
#define interval_Logging_Seconds 10

// BLE Client Current Time Service
BLEClientCts  bleCTime;

// Realtime Clock
RTC_PCF8523 rtc;

int gsr_measured = 0;
float battVolts = 0;
uint8_t battPct = 0;


void setup()
{
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
  setRTCLoop();

  // Get a single ADC sample and throw it away
  readVBAT();

  // Create loop2() using Scheduler to run in 'parallel' with loop()
  Scheduler.startLoop(loop2);
}


void loop()
{
  gsr_measured = getGSR();
  battVolts = battCheck();
  //delay(1000);
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
