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

Adafruit_MLX90614_Patos155::Adafruit_MLX90614_Patos155(uint8_t i2caddr) {
  _addr = i2caddr;
  Adafruit_MLX90614_Patos155::init();
}


boolean Adafruit_MLX90614_Patos155::begin(void) {
  Adafruit_MLX90614_Patos155::myWire.begin();

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
int Adafruit_MLX90614_Patos155::setSda_Scl(int x,int y){
  Adafruit_MLX90614_Patos155::sda=x;
  Adafruit_MLX90614_Patos155::scl=y;
}

boolean Adafruit_MLX90614_Patos155::init(void){
  Adafruit_MLX90614_Patos155::myWire(Adafruit_MLX90614_Patos155::sda,Adafruit_MLX90614_Patos155::scl);
  return true;
}
/*********************************************************************/

uint16_t Adafruit_MLX90614_Patos155::read16(uint8_t a) {
  uint16_t ret;

  Adafruit_MLX90614_Patos155::myWire.beginTransmission(_addr); // start transmission to device
  Adafruit_MLX90614_Patos155::myWire.write(a); // sends register address to read from
  Adafruit_MLX90614_Patos155::myWire.endTransmission(false); // end transmission

  Adafruit_MLX90614_Patos155::myWire.requestFrom(_addr, (uint8_t)3);// send data n-bytes read
  ret = Adafruit_MLX90614_Patos155::myWire.read(); // receive DATA
  ret |= Adafruit_MLX90614_Patos155::myWire.read() << 8; // receive DATA

  uint8_t pec = Adafruit_MLX90614_Patos155::Wire.read();

  return ret;
}
