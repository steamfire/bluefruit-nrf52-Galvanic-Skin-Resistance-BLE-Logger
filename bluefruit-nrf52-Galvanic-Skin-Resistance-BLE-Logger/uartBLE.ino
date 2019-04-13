


void uartBLELoop() {
  // Forward from BLEUART to HW Serial
  while ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    Serial.write(ch);
  }
}


void testUartSD() {

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("ANALOG01.CSV");

  // if the file is available, read from it:
  if (dataFile) {
    while (dataFile.available()) {
      bleuart.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening ANALOG01.CSV");
  }
}



void testUartSD_2() {

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("ANALOG61.CSV");

  // if the file is available, read from it:
  if (dataFile) {
    while (dataFile.available()) {
      uint8_t buf[64];
      int count = dataFile.readBytes(buf, sizeof(buf));
      bleuart.write( buf, count );
      //delay(2);
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening ANALOG61.CSV");
  }

}

void testUartSD_3() {

//  Print the contents of the currently open log file

  if ( xSemaphoreTakeRecursive( xSDSemaphoreMR, ( TickType_t ) 5 ) == pdTRUE ) {  // Reserve the SD card for this one

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(filename);

    // if the file is available, read from it:
    if (dataFile) {
        bleuart.println(filename);  // Print the filename
      while (dataFile.available()) {
        uint8_t buf[64];
        int count = dataFile.readBytes(buf, sizeof(buf));
        bleuart.write( buf, count );
        //delay(2);
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.print("error opening ");
      Serial.println(filename);
    }
    xSemaphoreGiveRecursive( xSDSemaphoreMR ); // Now free or "Give" or decrement the count for the Serial Port for others recursively.
  }

}

void testUart_4(){

  // This works to tell if the UART is connected to something
  
  if ( bleuart.notifyEnabled() )
  {
    // Forward data from our peripheral to Mobile
    bleuart.println( "Hello Bob" );
  }else
  {
    // response with no prph message
    Serial.println(" no bob connected");
  }  
}

void prph_bleuart_rx_callback(uint16_t conn_handle)
{
  (void) conn_handle;

  // Forward data from Mobile to our peripheral
  char str[20 + 1] = { 0 };
  bleuart.read(str, 20);

  Serial.print("[Prph] RX: ");
  Serial.println(str);

  testUartSD_3();

}
