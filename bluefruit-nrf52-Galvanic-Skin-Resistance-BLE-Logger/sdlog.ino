
// Set the pins used
#define cardSelect 11  // 11 for the Adalogger FeatherWing

File logfile;
char filename[15];

// blink out an error code
void error(uint8_t errno) {
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}

// This line is not needed if you have Adafruit SAMD board package 1.6.2+
//   #define Serial SerialUSB

void setupSDLog(void) {
  Serial.println("\r\nAnalog logger test");
  pinMode(LED_BUILTIN, OUTPUT);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }

  strcpy(filename, "/ANALG000.CSV");
  for (uint16_t i = 0; i < 512; i++) {
    filename[6] = '0' + i / 100;
    filename[7] = '0' + i % 100 / 10;
    filename[8] = '0' + i % 100  % 10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if ( ! logfile ) {
    Serial.print("Couldnt create ");
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to ");
  Serial.println(filename);


  // Open the file to write the .csv HEADER.
  logfile = SD.open(filename, FILE_WRITE);
  logfile.println("date/time, analog GSR value, liPo battery voltage, battery % charge");
  logfile.close();

  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(GREEN_LED, OUTPUT);
  Serial.println("Ready!");
}



void analogLog() {
  // fetch the time
  DateTime now = rtc.now();
  //  int gsr_measured = getGSR();
  //  float battVolts = battCheck();
  // set up buffer for date and time
//  char dateBuffer[14];
  char logBuffer[80];

  if ( xSemaphoreTakeRecursive( xSDSemaphoreMR, ( TickType_t ) 5 ) == pdTRUE ) {  // Reserve the SD card for this one

  // Open the file to write the .csv legends.
  logfile = SD.open(filename, FILE_WRITE);
  if (logfile) {
    //sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u, ", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    sprintf(logBuffer, "%04u-%02u-%02u %02u:%02u:%02u, %u, %.2f, %d%%\n", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
      gsr_measured, battVolts, battPct );
    
    logfile.print(logBuffer);
    
    /*logfile.print(dateBuffer);

    logfile.print(gsr_measured);
    logfile.print(", ");
    logfile.print(battVolts);
    logfile.print(", ");
    logfile.print(battPct);
    logfile.println("%");

    */
    logfile.close();
      xSemaphoreGiveRecursive( xSDSemaphoreMR ); // Now free or "Give" or decrement the count for the Serial Port for others recursively.
  }
  Serial.print(logBuffer);

/*
    Serial.print(dateBuffer);
    Serial.print(gsr_measured);
    Serial.print(", ");
    Serial.print(battVolts);
    Serial.print(", ");
    Serial.print(battPct);
    Serial.println("%");
    */


  }
  else {
    Serial.println("Error opening data file.");
  }

}

int getGSR() {
  long sum = 0;
  for (int i = 0; i < 500; i++)    //Average the 500 measurements to remove the glitch
  {
    int sensorValue = analogRead(GSR_Pin);
    sum += sensorValue;
    delay(5);
  }
  int gsr_average = sum / 500;
  return gsr_average;
}
