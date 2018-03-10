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
 
#include <SoftwareWire.h>
#include <Adafruit_MLX90614.h>

//Direcciones del sensor
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

//Pines 20 y 21 con comunicación i2c
SoftwareWire myWire(20,21);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test");  
  mlx.begin();  

  if(begin2()){
    Serial.println("Sensor 2 Iniciado");
  }
}

void loop() {
  Serial.println("Sensor 1");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.println("\nSensor 2");
  Serial.print("Ambient = "); Serial.print(readAmbientTempC2()); 
  Serial.print("*C\tObject = "); Serial.print(readObjectTempC2()); Serial.println("*C");
  Serial.println("-----------------------------------------------------------");
  delay(1000);
}


//////Funciones para leer el segundo sensor
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

uint16_t read16(uint8_t a) {
  uint16_t ret;

  myWire.beginTransmission(_addr); // Comienza la transmisión del dispositivo 
  myWire.write(a); // envía la dirección de registro para leer
  myWire.endTransmission(); // Termina la transmisión

  myWire.beginTransmission(_addr); // Comienza la transmisión del dispositivo 
  myWire.requestFrom(_addr, (uint8_t)3);// send data n-bytes read
  ret = myWire.read(); // Recibe datos
  ret |= myWire.read() << 8; // Recibe datos
  myWire.endTransmission(); // Termina la transmisión
  uint8_t pec = myWire.read();

  return ret;
}

float readTemp(uint8_t reg) {
  float temp;
  
  temp = read16(reg);
  temp *= .02;
  temp  -= 273.15;
  return temp;
}

double readObjectTempF2() {
  return (readTemp(MLX90614_TOBJ1) * 9 / 5) + 32;
}

double readAmbientTempF2() {
  return (readTemp(MLX90614_TA) * 9 / 5) + 32;
}

double readObjectTempC2() {
  return readTemp(MLX90614_TOBJ1);
}

double readAmbientTempC2() {
  return readTemp(MLX90614_TA);
}


