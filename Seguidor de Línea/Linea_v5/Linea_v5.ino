#include <Wire.h>           //Libreria para usar la comuniación I2C
#include <SFE_ISL29125.h>   //Libreria para usar el sensor RGB
#include <SoftwareWire.h>   //Libreria para usar I2C en otros pines digitales
#include <AFMotor.h>        //Libreria para usar los motores DC

AF_DCMotor Md(2);   //motor derecho (B)
AF_DCMotor Mi(1);   //motor izquierdo (A)

SFE_ISL29125 RGB_sensor;    //Creación del 1° Sensor RGB
SoftwareWire myWire( 51, 50);   //Configurar pines 51(SDA) y 50(SCL) con I2C (Puede ser cualquier pin digital)

//SENSOR INFRARROJO
//Pines del sensor infrarrojo
int izq_1=38;
int izq_2=40;
int centro_3=42;
int der_4=39;
int der_5=41;

//Variables para guardar datos del sensor infrarrojo
int l1=0;
int l2=0;
int l3=0;
int l4=0;
int l5=0;

//SENSORES RGB
//Direcciones del 2° sensor
#define _addr 0x44                  //Dirección del sensor
#define DEVICE_ID 0x00              //ID del sensor, verifica si el sensor funciona correctamente
#define CFG1_MODE_RGB 0x05          //Configuración del sensor para que lea rojo, verde y azul
#define CFG1_10KLUX 0x08            //Configuracion del rango de luz de lectura en 10,000
#define CFG2_IR_ADJUST_HIGH 0x3F    //Configura el filtro de infrarrojo en high
#define CFG_DEFAULT 0x00            //Configuración estándar de lectura

//Para configurar el sensor se usa la función config2() y necesita 3 parámetros
#define CONFIG_1 0x01               //En él se guardan el primer parámetro de configuración
#define CONFIG_2 0x02               //En él se guardan el segundo parámetro de configuración
#define CONFIG_3 0x03               //En él se guardan el tercer parámetro de configuración

#define STATUS 0x08                 //En él se guarda el estado del sensor

//Variables de ambos sensores RGB
//1° sensor RGB
uint16_t r1=0;
uint16_t v1=0;
uint16_t a1=0;

//2° sensor RGB
uint16_t r2=0;
uint16_t v2=0;
uint16_t a2=0;

// negro=0 blanco=1

void setup() {
  //Se inicia la comunicación Serial
  Serial.begin(9600);

  //Configurando pines del sensor infrarrojo como entradas
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(centro_3,INPUT);
  pinMode(der_4,INPUT);
  pinMode(der_5,INPUT);

  //Iniciar ambos sensores RGB
  if (RGB_sensor.init())    //Iniciar 1° sensor RGB
  {
    Serial.println("Sensor 1 iniciado");
  }

  if(init2()){              //Iniciar 2° sensor RGB
    Serial.println("Sensor 2 iniciado");
  }
}

void loop() {
  //Se guardan datos del sensor infrarrojo
  l1=digitalRead(izq_1);
  l2=digitalRead(izq_2);
  l3=digitalRead(centro_3);
  l4=digitalRead(der_4);
  l5=digitalRead(der_5);

  //Valores del 1° sensor RGB
  r1 = RGB_sensor.readRed();
  v1 = RGB_sensor.readGreen();
  a1 = RGB_sensor.readBlue();

  //Valores del 2° sensor RGB
  r2 = read16(0x0B);
  v2 = read16(0x09);
  a2 = read16(0x0D);
  
  //imprimirDatosInf();
  //imprimirDatosRGB();
  //delay(1000);

//Subida (puede perder la línea)
  if(l1==0 && l2==0 && l3==0 && l4==0 && l5==0){
    Quieto();
  }

//Línea cortada
  if(l1==1 && l2==1 && l3==1 && l4==1 && l5==1){
    Avanzar();
  }

//Muy desviado a la izquierda
  if(l1==0 || l1==0 && l2==0 || l1==0 && l2==0 && l4==0){    //Curvas cerradas
    GirarI2();            //Gira a la izquierda con mayor velocidad normal
  }
//Muy desviado a la derecha
  if(l5==0 || l5==0 && l4==0 || l5==0 && l4==0 && l2==0){    //Curvas cerradas
    GirarD2();            //Gira a la derecha con mayor velocidad normal 
  }

//CENTRADO  
  if(l1==1 && l2==1 && l4==1 && l5==1 || l3==0){
    Avanzar();
  }
  
//Desviado un poco a la izquierda
  if(l2==0 && l5==1 && l1==1 || l2==0 || l2==0 && l3==0 || l2==0 && l1==0){
    GirarI1();            //Gira a la izquierda con velocidad normal
  }
//Desviado un poco a la derecha
  if(l4==0 && l1==1 && l5==1 || l4==0 || l4==0 && l3==0 || l4==0 && l5==0){
    GirarD1();            //Gira a la derecha con velocidad normal 
  }

//90° Izquierda
  if(l1==0 && 12==0 || l1==0 && l2==0 && l3==0 || l1==0 && l2==0 && l3==0 && l4==0){
    GirarI2();            //Gira a la izquierda con mayor velocidad
  }

//90° Derecha
  if(l5==0 && l4==0 || l5==0 && l4==0 && l3==0 || l5==0 && l4==0 && l3==0 && l2==0){
    GirarD2();            //Gira a la derecha con mayor velocidad
  }

//Interseccion
  if(l2==0 && l4==0 || l1==0 && l2==0 && l4==0 && l5==0 ){     //Si se encuentra una intersección
    Quieto();
  }
  if(l3==0 && l2==0 && l4==0){
    Quieto(); 
  }
  
}

//velocidad máxima 255
//Función para girar a la derecha con mayor velocidad
void GirarD2(){
  Md.setSpeed(200);
  Md.run(BACKWARD);
  Mi.setSpeed(255);
  Mi.run(FORWARD);
}

//Función para girar a la izquierda con mayor velocidad
void GirarI2(){
  Mi.setSpeed(200);
  Mi.run(BACKWARD);
  Md.setSpeed(255);
  Md.run(FORWARD);
}

//Función para girar a la derecha con velocidad normal
void GirarD1(){
  Md.setSpeed(120);
  Md.run(BACKWARD);
  Mi.setSpeed(200);
  Mi.run(FORWARD);
}

//Función para girar a la izquierda con velocidad normal
void GirarI1(){
  Mi.setSpeed(120);
  Mi.run(BACKWARD);
  Md.setSpeed(200);
  Md.run(FORWARD);
}

//Función para avanzar 
void Avanzar(){
  Md.setSpeed(85);
  Md.run(FORWARD);
  Mi.setSpeed(85);
  Mi.run(FORWARD);
}

/*
//Función para ir de reversa
void Retroceder(){
  Md.setSpeed(85);
  Md.run(BACKWARD);
  Mi.setSpeed(85);
  Mi.run(BACKWARD);
}

//Función para desviarse un poco para luego recuperar la línea
void Recuperar(){
  Md.setSpeed(200);
  Md.run(BACKWARD);
  Mi.setSpeed(0);
  Mi.run(RELEASE);
}*/

//Función para detenerse
void Quieto(){
  Md.setSpeed(0);
  Md.run(RELEASE);
  Mi.setSpeed(0);
  Mi.run(RELEASE);
}

//Función para imprimir valores del sensor
void imprimirDatosInf(){
  Serial.print("Izq ext:");Serial.println(l5);
  Serial.print("Izq int:");Serial.println(l4);
  Serial.print("Centro:");Serial.println(l3);
  Serial.print("Der int:");Serial.println(l2);
  Serial.print("Der ext:");Serial.println(l1);
  Serial.println("-----------------------------------");
}

//Función para imrpimir valores de ambos sensores RGB
void imprimirDatosRGB(){
  //Imprimir valores del 1° RGB (en decimal)
  Serial.print("Rojo 1: "); Serial.println(r1,DEC);
  Serial.print("Verde 1: "); Serial.println(v1,DEC);
  Serial.print("Azul 1: "); Serial.println(a1,DEC);
  Serial.println();

  //Imprimir valores del 2° RGB
  Serial.print("Rojo 2: ");Serial.println(r2);
  Serial.print("Verde 2: ");Serial.println(v2);
  Serial.print("Azul 2: ");Serial.println(a2);
  Serial.println("-----------------------------------");
}

//Funciones para configurar y leer el 2° RGB///////////////////////
//NO MODIFICAR
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

