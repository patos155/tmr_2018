#include <AFMotor.h>
AF_DCMotor Md(4);   //motor derecho
AF_DCMotor Mi(3);   //motor izquierdo


int izq_1=22;
int izq_2=24;
int centro_3=26;
int der_4=28;
int der_5=30;

int l1=1;
int l2=1;
int l3=1;
int l4=1;
int l5=1;

//colores
int neg=0;
int bco=1;

void setup() {
  Serial.begin(9600);
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(centro_3,INPUT);
  pinMode(der_4,INPUT);
  pinMode(der_5,INPUT);
}

void loop() {
  l1=digitalRead(izq_1);
  l2=digitalRead(izq_2);
  l3=digitalRead(centro_3);
  l4=digitalRead(der_4);
  l5=digitalRead(der_5);

  /*Serial.print("Izq ext:");Serial.println(l5);
  Serial.print("Izq int:");Serial.println(l4);
  Serial.print("Centro:");Serial.println(l3);
  Serial.print("Der int:");Serial.println(l2);
  Serial.print("Der ext:");Serial.println(l1);
  delay(500);*/


  if(l1==1 && l2==1 && l4==1 && l5==1){
    Avanzar();
  }
  
  if(l1==0 || l1==0 && l2==0){
    GirarI2();
  }

  if(l5==0 || l5==0 && l4==0){
    GirarD2();
  }

  if(l2==0){
    GirarI1();
  }
  if(l4==0){
    GirarD1();
  }
  
  
}


void GirarD2(){
  Md.setSpeed(60);
  Md.run(BACKWARD);
  Mi.setSpeed(100);
  Mi.run(FORWARD);
}

void GirarI2(){
  Mi.setSpeed(60);
  Mi.run(BACKWARD);
  Md.setSpeed(100);
  Md.run(FORWARD);
}

void GirarD1(){
  Md.setSpeed(20);
  Md.run(BACKWARD);
  Mi.setSpeed(100);
  Mi.run(FORWARD);
}

void GirarI1(){
  Mi.setSpeed(20);
  Mi.run(BACKWARD);
  Md.setSpeed(100);
  Md.run(FORWARD);
}

void Avanzar(){
  Md.setSpeed(100);
  Md.run(FORWARD);
  Mi.setSpeed(100);
  Mi.run(FORWARD);
}
