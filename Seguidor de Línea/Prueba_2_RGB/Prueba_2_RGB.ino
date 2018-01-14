#include <Wire.h>
#include <SFE_ISL29125.h>
#include <SoftwareWire.h>

SFE_ISL29125 RGB_sensor;

SoftwareWire myWire( 4, 5);

int rojo=0;
int verde=0;
int azul=0;
int x=0;

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

uint16_t r=0;
uint16_t v=0;
uint16_t a=0;

void setup() {
  Serial.begin(9600);

  if (RGB_sensor.init())
  {
    Serial.println("Sensor 1 iniciado");
  }

  if(init2()){
    Serial.println("Sensor 2 iniciado");
  }
  
}

void loop() {

  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();
  
  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("Red: "); Serial.println(red,DEC);
  Serial.print("Green: "); Serial.println(green,DEC);
  Serial.print("Blue: "); Serial.println(blue,DEC);
  Serial.println();

  r=read16(0x0B);
  v=read16(0x09);
  a=read16(0x0D);
  Serial.print("Rojo 2: ");Serial.println(r);
  Serial.print("Verde 2: ");Serial.println(v);
  Serial.print("Azul 2: ");Serial.println(a);
  Serial.println("-----------------------------");
  
  delay(1000);
  
}

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

