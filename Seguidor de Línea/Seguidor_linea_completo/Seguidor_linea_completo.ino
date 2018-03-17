#include <ArduinoMotorShieldR3.h>   //Libreria para usar el motor shield de arduino
#include <Wire.h>           //Libreria para usar la comuniación I2C
#include <SFE_ISL29125.h>   //Libreria para usar el sensor RGB
#include <SoftwareWire.h>   //Libreria para usar I2C en otros pines digitales

ArduinoMotorShieldR3 motores;   //Declaración de los motores
//M1 = motor derecho
//M2 = motor izquierdo

SFE_ISL29125 RGB_sensor;    //Creación del 1° Sensor RGB
SoftwareWire myWire( 50,51 );   //Configurar pines 4(SDA) y 5(SCL) con I2C (Puede ser cualquier pin digital)

//Direcciones del 2° sensor
#define _addr 0x44                  //Dirección del sensor
#define DEVICE_ID 0x00              //ID del sensor, verifica si el sensor funciona correctamente
#define CFG1_MODE_RGB 0x05          //Configuración del sensor para que lea rojo, verde y azul
#define CFG1_10KLUX 0x08            //Configuracion del rango de luz de lectura en 10,000
#define CFG2_IR_ADJUST_HIGH 0x3F    //Configura el filtro de infrarrojo en high
#define CFG_DEFAULT 0x00            //Configuración estándar de lectura

//Para configurar el sensor se usa config2 y necesita 3 parámetros
#define CONFIG_1 0x01               //En él se guardan el primer parámetro de configuración
#define CONFIG_2 0x02               //En él se guardan el segundo parámetro de configuración
#define CONFIG_3 0x03               //En él se guardan el tercer parámetro de configuración

#define STATUS 0x08                 //En él guarda el estado del sensor

//Variables de la pista
#define blanco 1
#define negro 0
//Velocidades de los motores
int velI=145;    //160
int velD=145;   //163

//Variables de ambos sensores RGB
//1° sensor RGB
uint16_t r1=0;
uint16_t v1=0;
uint16_t a1=0;

//2° sensor RGB
uint16_t r2=0;
uint16_t v2=0;
uint16_t a2=0;

//SENSOR INFRARROJO
//Pines del sensor infrarrojo
int izq_1=A8;
int izq_2=A9;
int izq_3=A5;
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

int dl1=0;
int dl2=0;
int dl3=0;
int dl4=0;
int dl5=0;
int dl6=0;
int dl7=0;
int dl8=0;

//valores de blanco y negro
int bco=590;  //620
int neg=720;  //720

//ultrasónico
const int EchoPin = 53;
const int TriggerPin = 52;
int cm=20,tiempo=0;

boolean izq=false;
boolean der=false;

void setup(){
  //Se inicia la comunicación Serial
  Serial.begin(9600);

  //Iniciar motores
  motores.init();

  //Iniciar sensores RGB
  if (RGB_sensor.init())    //Iniciar 1° sensor RGB
  {
    Serial.println("Sensor 1 iniciado");
  }

  if(init2()){              //Iniciar 2° sensor RGB
    Serial.println("Sensor 2 iniciado");
  }

  //Configurando pines del sensor infrarrojo como entradas
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(izq_3,INPUT);
  pinMode(centro_i,INPUT);
  pinMode(centro_d,INPUT);
  pinMode(der_6,INPUT);
  pinMode(der_7,INPUT);
  pinMode(der_8,INPUT);

  //Ultrasónico
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}

void loop() {
  //Se guardan datos del sensor infrarrojo
  l1=analogRead(izq_1);
  l2=analogRead(izq_2);
  l3=analogRead(izq_3);
  l4=analogRead(centro_i);
  l5=analogRead(centro_d);
  l6=analogRead(der_6);
  l7=analogRead(der_7);
  l8=analogRead(der_8);
  //Conversion de datos a 0 1  1=blanco 0=negro
  if (l1<=bco){dl1=1;} else {dl1=0;}
  if (l2<=bco){dl2=1;} else {dl2=0;}
  if (l3<=bco){dl3=1;} else {dl3=0;}
  if (l4<=bco){dl4=1;} else {dl4=0;}
  if (l5<=bco-10){dl5=1;} else {dl5=0;}
  if (l6<=bco){dl6=1;} else {dl6=0;}
  if (l7<=bco){dl7=1;} else {dl7=0;}
  if (l8<=bco){dl8=1;} else {dl8=0;}
  
  //Moverse en linea recta
  Moverse(velD,velI);

  if(dl3==negro && dl6==negro){
    Moverse(velD,velI);
  }

  //Muy desviado a la izquierda
  if(dl8==negro && dl7==negro && dl3==negro){
    Moverse(-250,200);
  }

  //Muy desviado a la derecha
  if(dl1==negro && dl2==negro && dl6==negro){
    Moverse(200,-250);
  }

  //Centrado
  if(dl8==blanco && dl7==blanco && dl2==blanco && dl1==blanco){
    Moverse(velD,velI);
      ultra();
  if (cm<10 && cm>1){
    //RETROCEDER
    Moverse(-90,-90);
    delay (1000);//1000
    //Quieto
    Moverse(0,0);
    delay (500);//500
    //GIRAR A LA DERECHA
    Moverse(-150,190);
    delay (800);//800
     //AVANZAR
    Moverse(100,100);
    delay (3100);//3100
    //GIRAR A LA IZQUIERDA
    Moverse(180,-160);
    delay (900);//900
    //AVANZAR
    Moverse(100,100);
    delay (4700);//4700
    //GIRAR A LA IZQUIERDA
    Moverse(150,-190);
    delay (700);//700
    //AVANZAR
    Moverse(100,100);
    delay (1000);//1000
    //Quieto
    Moverse(0,0);
    delay (500);//500
  }
  }

  //Ligeramente desviado a la izquierda
  if(dl2==negro && dl8==blanco && dl1==blanco){
    Moverse(200,-120);
  }

  //Ligeramente desviado a la derecha
  if(dl7==negro && dl1==blanco && dl5==blanco){
    Moverse(-120,200);
  }

  //Curvas de 90°
  if(dl8==negro && dl7==negro && dl6==negro || dl8==negro && dl7==negro && dl6==negro && dl5==negro){
    Moverse(-130,200);
  }

  if(dl1==negro && dl2==negro && dl3==negro || dl1==negro && dl2==negro && dl3==negro && dl4==negro){
    Moverse(200,-130);
  }

  if(dl2==negro || dl2==negro && dl1==negro){
    Moverse(200,-250);
  }
  if(dl7==negro || dl7==negro && dl8==negro){
    Moverse(-250,200);  
  }
/*

  //Intersección
  if(dl1==negro && dl8==negro && dl4==negro && dl5==negro){
    Moverse(0,0);
    delay(500);

    for(int i=1;i<=7;i++){
      leerRGB();
      if(r2>840 && v2>1840 && a2>200 && r2<1900 && v2<3000 && a2<3000){
        izq=true;
      }else{
        izq=false;
      }

      if(r1>700 && v1>1000 && a1>1200 && r1<2500 && v1<2500 && a1<2500){
        der=true;
      }else{
        der=false;
      } 
      delay(200);
    }

    if(der && izq){ //Si se encuentran 2 verdes 
      Moverse(-200,200);  //Vuleta de 180°
      delay(2000);
    }else{  
      if(der && izq==false){    //Si hay un verde en derecha
        Moverse(-120,200);
        delay(800);
      }
      if(izq && der==false){    //Si hay un verde en izquierda
        Moverse(200,-120);
        delay(800);
      }
    }

    if(der==false && izq==false){   //Si no hay ningun verde
      Moverse(100,100);
      delay(200);
      //Leemos la linea nuevamente
      l1=analogRead(izq_1);
      l2=analogRead(izq_2);
      l3=analogRead(izq_3);
      l6=analogRead(der_6);
      l7=analogRead(der_7);
      l8=analogRead(der_8);
      //Conversion de datos a 0 1  1=blanco 0=negro
      if (l1<=bco){dl1=1;} else {dl1=0;}
      if (l2<=bco){dl2=1;} else {dl2=0;}
      if (l3<=bco){dl3=1;} else {dl3=0;}
      if (l6<=bco){dl6=1;} else {dl6=0;}
      if (l7<=bco){dl7=1;} else {dl7=0;}
      if (l8<=bco){dl8=1;} else {dl8=0;}

      if(dl1==negro && dl8==negro){
        Moverse(100,100);
        delay(400);
      }else{
        if(dl3==negro && dl2==negro && dl1==negro && dl8==blanco){
          Moverse(-100,-100);
          delay(400);
          Moverse(100,-100);
          delay(400);
          Moverse(100,100);
          delay(500);
        }
        if(dl8==negro && dl7==negro && dl6==negro && dl1==blanco){
          Moverse(-100,-100);
          delay(400);
          Moverse(-100,100);
          delay(400);
          Moverse(100,100);
          delay(500);
        }
      }
      
    }
  }*/

  
  //leerRGB();
  //imprimirRGB();
  
  //imprimirDatosInf();
  //delay(800);
  

}
//Movimiento de los motores
int Moverse(int velD_, int velI_){
  motores.setM1Speed(velD_);
  motores.setM2Speed(velI_);
}

void ultra(){
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  
  // medimos el pulso de respuesta
  tiempo = (pulseIn(EchoPin, HIGH)/2); // dividido por 2 por que es el 
                                       // tiempo que el sonido tarda
                                       // en ir y en volver
  // ahora calcularemos la distancia en cm
  // sabiendo que el espacio es igual a la velocidad por el tiempo
  // y que la velocidad del sonido es de 343m/s y que el tiempo lo 
  // tenemos en millonesimas de segundo
  cm = float(tiempo * 0.0343);
}

//Función para imprimir valores del sensor
void imprimirDatosInf(){
  Serial.print("Izq 1: ");Serial.print(l1);Serial.print("\t / D Izq 1: ");Serial.println(dl1);
  Serial.print("Izq 2: ");Serial.print(l2);Serial.print("\t / D Izq 1: ");Serial.println(dl2);
  Serial.print("Izq 3: ");Serial.print(l3);Serial.print("\t / D Izq 1: ");Serial.println(dl3);
  Serial.print("Cen i: ");Serial.print(l4);Serial.print("\t / D Izq 1: ");Serial.println(dl4);
  Serial.print("Cen d: ");Serial.print(l5);Serial.print("\t / D Izq 1: ");Serial.println(dl5);
  Serial.print("Der 6: ");Serial.print(l6);Serial.print("\t / D Izq 1: ");Serial.println(dl6);
  Serial.print("Der 7: ");Serial.print(l7);Serial.print("\t / D Izq 1: ");Serial.println(dl7);
  Serial.print("Der 8: ");Serial.print(l8);Serial.print("\t / D Izq 1: ");Serial.println(dl8);
  Serial.println("-----------------------------------");
}

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

void verde(){
  if(r2>840 && v2>1840 && a2>200 && r2<1900 && v2<3000 && a2<3000){
      izq=true;
    }else{
      izq=false;
    }

    if(r1>700 && v1>1000 && a1>1200 && r1<2500 && v1<2500 && a1<2500){
      der=true;
    }else{
      der=false;
    }
}

void imprimirRGB(){
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
