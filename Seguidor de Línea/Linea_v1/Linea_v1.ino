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
    Avanzar();
  }
  if (l1==0 && l2==0 && l3==0 && l4==0 && l5==0){
    Avanzar();
    
  }

  if(l2==0 && l4==0){
    Avanzar();
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

