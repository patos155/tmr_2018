
SoftwareWire myWire(48,45);

boolean begin2() {
  myWire.begin();

  /*
  for (uint8_t i=0; i<0x20; i++) {
    Serial.print(i); Serial.print(" = ");
    Serial.println(read16(i), HEX);
  }
  */
  return true;
}

uint16_t read162(uint8_t a) {
  uint16_t ret;

  myWire.beginTransmission(_addr); // start transmission to device 
  myWire.write(a); // sends register address to read from
  myWire.endTransmission(); // end transmission

  myWire.beginTransmission(_addr); // start transmission to device 
  myWire.requestFrom(_addr, (uint8_t)3);// send data n-bytes read
  ret = myWire.read(); // receive DATA
  ret |= myWire.read() << 8; // receive DATA
  myWire.endTransmission(); // end transmission

  uint8_t pec = myWire.read();

  return ret;
}

float readTemp2(uint8_t reg) {
  float temp;
  
  temp = read162(reg);
  temp *= .02;
  temp  -= 273.15;
  return temp;
}

double readObjectTempF2() {
  return (readTemp2(MLX90614_TOBJ1) * 9 / 5) + 32;
}

double readAmbientTempF2() {
  return (readTemp2(MLX90614_TA) * 9 / 5) + 32;
}

double readObjectTempC2() {
  return readTemp2(MLX90614_TOBJ1);
}

double readAmbientTempC2() {
  return readTemp2(MLX90614_TA);
}


