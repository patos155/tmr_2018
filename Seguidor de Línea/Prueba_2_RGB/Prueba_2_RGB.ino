#include <Wire.h>           //Libreria para usar la comuniación I2C
#include <SFE_ISL29125.h>   //Libreria para usar el sensor RGB
#include <SoftwareWire.h>   //Libreria para usar I2C en otros pines digitales

SFE_ISL29125 RGB_sensor;    //Creación del 1° Sensor RGB

SoftwareWire myWire( 4, 5);   //Configurar pines 4(SDA) y 5(SCL) con I2C

//Direcciones del 2° sensor
#define _addr 0x44
#define DEVICE_ID 0x00
#define CFG1_MODE_RGB 0x05
#define CFG1_10KLUX 0x08
#define CFG2_IR_ADJUST_HIGH 0x3F
#define CFG_DEFAULT 0x00
#define CONFIG_1 0x01
#define CONFIG_2 0x02
#define CONFIG_3 0x03
#define STATUS 0x08 

//Variables de ambos sensores RGB
//1° sensor RGB
uint16_t r1=0;
uint16_t v1=0;
uint16_t a1=0;

//2° sensor RGB
uint16_t r2=0;
uint16_t v2=0;
uint16_t a2=0;

void setup() {
  Serial.begin(9600);       //Iniciar comunicación Serial

  if (RGB_sensor.init())    //Iniciar 1° sensor RGB
  {
    Serial.println("Sensor 1 iniciado");
  }

  if(init2()){              //Iniciar 2° sensor RGB
    Serial.println("Sensor 2 iniciado");
  }
  
}

void loop() {

  //Valores del 1° sensor RGB
  r1 = RGB_sensor.readRed();
  v1 = RGB_sensor.readGreen();
  a1 = RGB_sensor.readBlue();

  //Valores del 2° sensor RGB
  r2 = read16(0x0B);
  v2 = read16(0x09);
  a2 = read16(0x0D);
  
  //Imprimir valores del 1° RGB (en decimal)
  Serial.print("Rojo 1: "); Serial.println(r1,DEC);
  Serial.print("Verde 1: "); Serial.println(v1,DEC);
  Serial.print("Azul 1: "); Serial.println(a1,DEC);
  Serial.println();

  //Imprimir valores del 2° RGB
  Serial.print("Rojo 2: ");Serial.println(r2);
  Serial.print("Verde 2: ");Serial.println(v2);
  Serial.print("Azul 2: ");Serial.println(a2);
  Serial.println("-----------------------------");
  
  delay(1000);
  
}

//Funciones para configurar y leer el 2° RGB///////////////////////
uint16_t read16(uint8_t reg)
{
  uint16_t data = 0x0000;

  myWire.beginTransmission(_addr);
  myWire.write(reg);
  myWire.endTransmission();
  
  myWire.beginTransmission(_addr);
  myWire.requestFrom(_addr, (uint8_t)2); // request 2 bytes of data
  data = myWire.read();
  data |= (myWire.read() << 8);
  myWire.endTransmission();

  return data;
}

bool init2()
{
  bool ret = true;
  uint8_t data = 0x00;
  
  // Start I2C
  myWire.begin();
  
  // Check device ID
  data = read8(DEVICE_ID);
  if (data != 0x7D)
  {
    ret &= false;
  }
  
  // Reset registers
  ret &= reset();
  
  // Set to RGB mode, 10k lux, and high IR compensation
  ret &= config2(CFG1_MODE_RGB | CFG1_10KLUX, CFG2_IR_ADJUST_HIGH, CFG_DEFAULT);
  
  return ret;
}

bool config2(uint8_t config1, uint8_t config2, uint8_t config3)
{
  bool ret = true;
  uint8_t data = 0x00;
  
  // Set 1st configuration register
  write8(CONFIG_1, config1);
  // Set 2nd configuration register
  write8(CONFIG_2, config2);
  // Set 3rd configuration register
  write8(CONFIG_3, config3);
  
  // Check if configurations were set correctly
  data = read8(CONFIG_1);
  if (data != config1)
  {
    ret &= false;
  }
  data = read8(CONFIG_2);
  if (data != config2)
  {
    ret &= false;
  }
  data = read8(CONFIG_3);
  if (data != config3)
  {
    ret &= false;
  }
  return ret;
}

uint8_t read8(uint8_t reg)
{
  myWire.beginTransmission(_addr);
  myWire.write(reg);
  myWire.endTransmission();
  myWire.beginTransmission(_addr);
  myWire.requestFrom(_addr,(uint8_t)1);
  uint8_t data = myWire.read();
  myWire.endTransmission();
  
  return data;
}

bool reset()
{
  uint8_t data = 0x00;
  // Reset registers
  write8(DEVICE_ID, 0x46);
  // Check reset
  data = read8(CONFIG_1);
  data |= read8(CONFIG_2);
  data |= read8(CONFIG_3);
  data |= read8(STATUS);
  if (data != 0x00)
  {
    return false;
  }
  return true;
}

void write8(uint8_t reg, uint8_t data)
{
  myWire.beginTransmission(_addr);
  myWire.write(reg);
  myWire.write(data);
  myWire.endTransmission();
  
  return;
}

