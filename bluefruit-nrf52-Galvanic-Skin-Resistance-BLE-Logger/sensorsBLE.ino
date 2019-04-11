


void setupAIO() {
  aios.begin();
  // Note: You must call .begin() on the BLEService before calling .begin() on
  // any characteristic(s) within that service definition.. Calling .begin() on
  // a BLECharacteristic will cause it to be added to the last BLEService that
  // was 'begin()'ed!

  aioc.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  aioc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  aioc.setFixedLen(2);
  aioc.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  aioc.begin();
  uint8_t aiodata[2] = { 0, 0 };  // Set the first data to zero
  aioc.write(aiodata, 2);


}


void updateAIO(uint16_t GSRValue){
    //Update the BLE Analog data 
    uint8_t aiodata[2] = {  lowByte(GSRValue), highByte(GSRValue) };
    aioc.notify(aiodata, 2);                    // Use .write for init data

  
}

void updateBatt(uint8_t battPercent){
    blebas.write(battPercent);
}
