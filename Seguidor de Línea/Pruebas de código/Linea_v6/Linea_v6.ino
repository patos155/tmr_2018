
#include <Wire.h>           //Libreria para usar la comuniación I2C
#include <SFE_ISL29125.h>   //Libreria para usar el sensor RGB
#include <SoftwareWire.h>   //Libreria para usar I2C en otros pines digitales
#include <ArduinoMotorShieldR3.h>   //Libreria para usar el nuevo motor shield

ArduinoMotorShieldR3 motores;   //Declaración de los motores

SFE_ISL29125 RGB_sensor;    //Creación del 1° Sensor RGB
SoftwareWire myWire( 44, 45);   //Configurar pines 44(SDA) y 4(SCL) con I2C (Puede ser cualquier pin digital)

//SENSOR INFRARROJO
//Pines del sensor infrarrojo
int izq_1=A8;
int izq_2=A9;
int izq_3=A10;
int centro_i=A11;
int centro_d=A12;
int der_6=A13;
int der_7=A14;
int der_8=A15;

//Variables donde se guardan las lecuturas del sensor infrarrojo 
int l1=0;
int l2=0;
int l3=0;
int l4=0;
int l5=0;
int l6=0;
int l7=0;
int l8=0;

//valores de blanco y negro
int bco=710;    //660
int neg=711;    //661

bool vDer=false;
bool vIzq=false;

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

//SENSOR ULTRASÓNICO
//Pines
int Trig=52;
int Echo=53;

//Valores
int dist=0;

void setup(){
  //Se inicia la comunicación Serial
  Serial.begin(9600);

  //Iniciar motores
  motores.init();

  //Configurando pines del sensor infrarrojo como entradas
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(izq_3,INPUT);
  pinMode(centro_i,INPUT);
  pinMode(centro_d,INPUT);
  pinMode(der_6,INPUT);
  pinMode(der_7,INPUT);
  pinMode(der_8,INPUT);

  /*/Iniciar ambos sensores RGB
  if (RGB_sensor.init())    //Iniciar 1° sensor RGB
  {
    Serial.println("Sensor 1 RGB iniciado");
  }

  if(init2()){              //Iniciar 2° sensor RGB
    Serial.println("Sensor 2 RGB iniciado");
  }*/

  //Configuración del sensor ultrasónico
  pinMode(Echo,INPUT);
  pinMode(Trig,OUTPUT);
}

void loop(){

//Se guardan datos del sensor infrarrojo
  l1=analogRead(izq_1);
  l2=analogRead(izq_2);
  l3=analogRead(izq_3);
  l4=analogRead(centro_i);
  l5=analogRead(centro_d);
  l6=analogRead(der_6);
  l7=analogRead(der_7);
  l8=analogRead(der_8);

//Lectura del sensor ultrasónico
  dist=ping(Trig,Echo);

  //leerRGB();
  //imprimirDatosInf();
  //imprimirDatosRGB();
  //delay(700);

/*/Encontrar objeto ultrasonico
  if(dist<=7 && dist>0){
    Quieto();
    delay(1000);
  }
*/

//Linea chido v2

//Linea cortada
if(l1<=bco && l2<=bco && l3<=bco && l4<=bco && l5<=bco && l6<=bco && l7<=bco && l8<=bco){
  Avanzar();
}

//Centrado
 if(l4>=neg && l5>=neg){
  Avanzar();
}

//Potencia mayor
 if(l1>=neg && l2>=neg || l1>=neg){
  GirarI3();
}

//Mayor potencia
 if(l7>=neg && l8>=neg || l8>=neg){
  GirarD3();
}

//Mayor Potencia
 if(l1>=neg && l2>=neg && l3>=neg){ 
  GirarI3();
}
//Mayor Potencia
 if(l6>=neg && l7>=neg && l8>=neg){
  GirarD3();
}

//Muy desviado a la izquierda
 if (l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg || l1>=neg && l2>=neg && l3>=neg && l4>=neg){  
  GirarI3();
}

//Muy desviado a la derecha
 if(l4>=neg && l5>=neg && l6>=neg && l7>=neg && l8>=neg || l5>=neg && l6>=neg && l7>=neg && l8>=neg){
  GirarD3();
}

 if(l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg && l6>=neg){
  GirarI3();
}

 if(l3>=neg && l4>=neg && l5>=neg && l6>=neg && l7>=neg && l8>=neg){
  GirarD3();
}

 if(l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg && l6>=neg && l7>=neg){
  GirarI3();
}

 if(l2>=neg && l3>=neg && l4>=neg && l5>=neg && l6>=neg && l7>=neg && l8>=neg){
  GirarD3();
}

//Menor potencia
 if(l2>=neg && l3>=neg && l4>=neg || l2>=neg && l3>=neg && l4>=neg && l5>=neg){
  GirarI2();
}

//Menor potencia
 if(l5>=neg && l6>=neg && l7>=neg || l4>=neg && l5>=neg && l6>=neg && l7>=neg){
  GirarD2();
}

//Potencia normal
 if(l2>=neg && l3>=neg){
  GirarI2();
}

//Potencia normal
 if(l6>=neg && l7>=neg){  
  GirarD2();
}

//Menor potencia
 if(l5>=neg && l4>=neg){
  GirarI1();
}

 if(l5>=neg && l6>=neg){  
  GirarD1();
}

/*/Intersección 1
  if(l3>=neg && l4>=neg && l5>=neg && l6>=neg){
  Quieto();
}*/

//Quieto y leer RGB
 if(l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg && l6>=neg && l7>=neg && l8>=neg){
  if(vIzq || vDer){
    if(vDer){
      GirarD3();
    }
    if(vIzq){
      GirarI3();
    }
  }else{
    Quieto();
  }
}

}

//Velocidad máxima de motores: 400
//Función para girar a la derecha con velocidad normal
void GirarD3(){
  vDer=true;
  //Serial.println("GirarD3");
  motores.setM1Speed(-400); //-150
  motores.setM2Speed(400);  //190
}

//Función para girar a la izquierda con velocidad normal
void GirarI3(){
  vIzq=true;
  //Serial.println("GirarI3");
  motores.setM2Speed(-400); //-150
  motores.setM1Speed(400);  //190
}

//Función para girar a la derecha con mayor velocidad
void GirarD2(){
  //Serial.println("GirarD2");
  motores.setM1Speed(-150); //-80
  motores.setM2Speed(250);  //150
}

//Función para girar a la izquierda con mayor velocidad
void GirarI2(){
  //Serial.println("GirarI2");
  motores.setM2Speed(-150);  //-80
  motores.setM1Speed(250);  //150
}

//Función para girar a la derecha con velocidad normal
void GirarD1(){
  vDer=false;
  //Serial.println("GirarD1");
  motores.setM1Speed(-25); //0
  motores.setM2Speed(120);  //50
}

//Función para girar a la izquierda con velocidad normal
void GirarI1(){
  vIzq=false;
  //Serial.println("GirarI1");
  motores.setM2Speed(-25);  //0
  motores.setM1Speed(120);  //50
}

//Función para avanzar 
void Avanzar(){
  vIzq=false;
  vDer=false;
  //Serial.println("Avanza");
  motores.setM1Speed(110);  //80
  motores.setM2Speed(110);  //80
}

//Funcióon para girar sobre su eje
void Retorno(){
  vIzq=false;
  vDer=false;
  //Serial.println("Retorno");
  motores.setM2Speed(100);
  motores.setM1Speed(-100);
}

//Función para detenerse
void Quieto(){
  vIzq=false;
  vDer=false;
  //Serial.println("Quieto");
  motores.setM1Speed(0);
  motores.setM2Speed(0);
}

//Funcion para leer los colores de ambos sensores RGB
void leerRGB(){
  //Valores del 1° sensor RGB
  r1 = RGB_sensor.readRed();
  v1 = RGB_sensor.readGreen();
  a1 = RGB_sensor.readBlue();

  //Valores del 2° sensor RGB
  r2 = read16(0x0B);
  v2 = read16(0x09);
  a2 = read16(0x0D);
}

//Función para imprimir valores del sensor
void imprimirDatosInf(){
  Serial.print("Izq 1:");Serial.println(l1);
  Serial.print("Izq 2:");Serial.println(l2);
  Serial.print("Izq 3:");Serial.println(l3);
  Serial.print("Centro i:");Serial.println(l4);
  Serial.print("Centro d:");Serial.println(l5);
  Serial.print("Der 6:");Serial.println(l6);
  Serial.print("Der 7:");Serial.println(l7);
  Serial.print("Der 8:");Serial.println(l8);
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

//Conversión para el sensor ultrasónico
int ping(int TriggerPin, int EchoPin){
  long duration, distanceCm;

  digitalWrite(TriggerPin,LOW);
  delayMicroseconds(4);
  digitalWrite(TriggerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin,LOW);

  duration = pulseIn(EchoPin,HIGH);

  distanceCm = duration * 10/292/2;
  return distanceCm;
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

