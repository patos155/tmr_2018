#include <AFMotor.h>
#include <SFE_ISL29125.h>

//MOTORES
AF_DCMotor Md(4);   //motor derecho
AF_DCMotor Mi(3);   //motor izquierdo

SFE_ISL29125 rgb_d;      //pines SDA - A4, SCL - A5
//SFE_ISL29125 rgb_i;      //pines SDA - A4, SCL - A5

//PINES SENSOR INFRARROJO
int izq_1=22;
int izq_2=24;
int centro_3=26;
int der_4=28;
int der_5=30;

//VARIABLES SENSOR INFRARROJO
int l1=1;
int l2=1;
int l3=1;
int l4=1;
int l5=1;

int neg=0;
int bco=1;

//VARIABLES RGB
int ri=0;
int gi=0;
int bi=0;

int rd=0;
int gd=0;
int bd=0;

int verde_d=0;
int verde_i=0;

void setup() {
  Serial.begin(9600);

  if(rgb_d.init()){                  //Si el sensor RGB funciona se manda mensaje al monitor serial
    Serial.println("Sensor RGB funcionanando..");
  }

  //SE DECLARAN LOS PINES DEL SENSOR INFRARROJO COMO ENTRADA
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(centro_3,INPUT);
  pinMode(der_4,INPUT);
  pinMode(der_5,INPUT);
}

void loop() {

  //LECTURA SENSOR INFRAROJO
  l1=digitalRead(izq_1);
  l2=digitalRead(izq_2);
  l3=digitalRead(centro_3);
  l4=digitalRead(der_4);
  l5=digitalRead(der_5);

  //LECTURA SENSOR RGB DERECHO
  rd = rgb_d.readRed();
  gd = rgb_d.readGreen();
  bd = rgb_d.readBlue();

  /*Serial.print("Izq ext:");Serial.println(l5);
  Serial.print("Izq int:");Serial.println(l4);
  Serial.print("Centro:");Serial.println(l3);
  Serial.print("Der int:");Serial.println(l2);
  Serial.print("Der ext:");Serial.println(l1);
  delay(500);*/

//CENTRADO  
  if(l1==1 && l2==1 && l4==1 && l5==1){
    Avanzar();
  }
//Muy desviado a la izquierda
  if(l1==0 || l1==0 && l2==0 || l1==0 && l2==0 && l4==0){
    GirarI2();
  }
//Muy desviado a la derecha
  if(l5==0 || l5==0 && l4==0 || l2==0 && l4==0 && l5==0){
    GirarD2();
  }
//Desviado a la izquierda
  if(l2==0){
    GirarI1();
  }
//Desviado a la derecha
  if(l4==0){
    GirarD1();
  }  
//Posibles intersecciones
  if(l2==0 && l3==0 && l4==0){
    Color_d();

    if(verde_d==1 && verde_i==0){      //Si el sensor rgb derecho encuentra verde
      Avanzar();
      delay(1000);
      GirarD1();
      delay(1000);
    }else{
      Avanzar();
    }
    
  }
  if (l1==0 && l2==0 && l3==0 && l4==0 && l5==0){
    Color_d();
    if(verde_d==1 && verde_i==0){      //Si el sensor rgb derecho encuentra verde
      Avanzar();
      delay(1000);
      GirarD1();
      delay(1000);
    }else{
      Avanzar();
    }
    
  }
  if(l2==0 && l4==0){
    Color_d();
    if(verde_d==1 && verde_i==0){      //Si el sensor rgb derecho encuentra verde
      Avanzar();
      delay(1000);
      GirarD1();
      delay(1000);
    }else{
      Avanzar();
    }
    
  }
  
}


void GirarD2(){
  Md.setSpeed(200);
  Md.run(BACKWARD);
  Mi.setSpeed(225);
  Mi.run(FORWARD);
}

void GirarI2(){
  Mi.setSpeed(200);
  Mi.run(BACKWARD);
  Md.setSpeed(225);
  Md.run(FORWARD);
}

void GirarD1(){
  Md.setSpeed(100);
  Md.run(BACKWARD);
  Mi.setSpeed(225);
  Mi.run(FORWARD);
}

void GirarI1(){
  Mi.setSpeed(100);
  Mi.run(BACKWARD);
  Md.setSpeed(225);
  Md.run(FORWARD);
}

void Avanzar(){
  Md.setSpeed(200);
  Md.run(FORWARD);
  Mi.setSpeed(200);
  Mi.run(FORWARD);
}
void Retroceder(){
  Md.setSpeed(200);
  Md.run(FORWARD);
  Mi.setSpeed(200);
  Mi.run(FORWARD);
}

int Color_d(){    //BUSCAR VERDE

  if(rd>=250 && gd>=405 && bd>=215 && rd<335 && gd<450 && bd<255){
    Serial.println("Verde !!");
    verde_d=1;
  }
  if(rd>=335 && gd>=450 && bd>=255 && rd<1000 && gd<1110 && bd<545){
    Serial.println("Negro !!");
    verde_d=0;
  }
  if(rd>=1000 && gd>=1110 && bd>=545){
    Serial.println("Blanco !!");
    verde_d=0;
  }

}
/*
int Color_i(){    //BUSCAR VERDE

  if(ri>=250 && gi>=405 && bi>=215 && ri<335 && gi<450 && bi<255){
    Serial.println("Verde !!");
    verde_i=1;
  }
  if(ri>=335 && gi>=450 && bi>=255 && ri<1000 && gi<1110 && bi<545){
    Serial.println("Negro !!");
    verde_i=0;
  }
  if(ri>=1000 && gi>=1110 && bi>=545){
    Serial.println("Blanco !!");
    verde_i=0;
  }
  return verde_i;
}*/

