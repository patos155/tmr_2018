/*************************************************** 
  This is a library example for the MLX90614 Temp Sensor

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
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <SoftwareWire.h>
//#include <Adafruit_MLX90614.h>

/////////////////////////////
#define MLX90614_I2CADDR 0x5A

// RAM
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08
// EEPROM
#define MLX90614_TOMAX 0x20
#define MLX90614_TOMIN 0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_EMISS 0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x0E
#define MLX90614_ID1 0x3C
#define MLX90614_ID2 0x3D
#define MLX90614_ID3 0x3E
#define MLX90614_ID4 0x3F
#define _addr 0x5A
//////////////////////////////

//Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);

  if(begin1()){
    Serial.println("Sensor 1 Iniciado");
  }

  if(begin2()){
    Serial.println("Sensor 2 Iniciado");
  }
}

void loop() {
  Serial.println("Sensor 1");
  Serial.print("Ambient = "); Serial.print(readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(readObjectTempF()); Serial.println("*F\n");
  Serial.println("Sensor 2\n");
  Serial.print("Ambient = "); Serial.print(readAmbientTempC2()); 
  Serial.print("*C\tObject = "); Serial.print(readObjectTempC2()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(readAmbientTempF2()); 
  Serial.print("*F\tObject = "); Serial.print(readObjectTempF2()); Serial.println("*F");
  Serial.println("-----------------------------------------------------------");
  delay(1000);
}
