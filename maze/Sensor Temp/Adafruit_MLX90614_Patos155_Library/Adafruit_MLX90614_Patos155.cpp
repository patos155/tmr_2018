/***************************************************
  This is a library for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  ----Edited by Patos 155----
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_MLX90614_Patos155.h"
#include "SoftwareWire.h"

SoftwareWire myWire(uint8_t sda=48,uint8_t scl=45);


Adafruit_MLX90614_Patos155::Adafruit_MLX90614_Patos155(uint8_t i2caddr) {
  _addr = i2caddr;
}


boolean Adafruit_MLX90614_Patos155::begin(void) {
  myWire.begin();

  /*
  for (uint8_t i=0; i<0x20; i++) {
    Serial.print(i); Serial.print(" = ");
    Serial.println(read16(i), HEX);
  }
  */
  return true;
}

//////////////////////////////////////////////////////


double Adafruit_MLX90614_Patos155::readObjectTempF(void) {
  return (readTemp(MLX90614_TOBJ1) * 9 / 5) + 32;
}


double Adafruit_MLX90614_Patos155::readAmbientTempF(void) {
  return (readTemp(MLX90614_TA) * 9 / 5) + 32;
}

double Adafruit_MLX90614_Patos155::readObjectTempC(void) {
  return readTemp(MLX90614_TOBJ1);
}


double Adafruit_MLX90614_Patos155::readAmbientTempC(void) {
  return readTemp(MLX90614_TA);
}

float Adafruit_MLX90614_Patos155::readTemp(uint8_t reg) {
  float temp;

  temp = read16(reg);
  temp *= .02;
  temp  -= 273.15;
  return temp;
}
/*********************************************************************/

/*********************************************************************/

uint16_t Adafruit_MLX90614_Patos155::read16(uint8_t a) {
  uint16_t ret;

  myWire.beginTransmission(_addr); // start transmission to device
  myWire.write(a); // sends register address to read from
  myWire.endTransmission(false); // end transmission

  myWire.requestFrom(_addr, (uint8_t)3);// send data n-bytes read
  ret = myWire.read(); // receive DATA
  ret |= myWire.read() << 8; // receive DATA

  uint8_t pec = myWire.read();

  return ret;
}
