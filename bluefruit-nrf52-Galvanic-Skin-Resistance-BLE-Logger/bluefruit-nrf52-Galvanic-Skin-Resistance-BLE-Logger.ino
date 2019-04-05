

#include <bluefruit.h>
#include <Wire.h>
#include "RTClib.h"

// BLE Client Current Time Service
BLEClientCts  bleCTime;

// Realtime Clock 
RTC_PCF8523 rtc;

void setup()
{
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  setupTime();
  setRTCLoop();
}


void loop()
{
//setRTCLoop(); 
delay(100); 
}
