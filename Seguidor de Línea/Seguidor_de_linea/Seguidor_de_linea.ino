#include <ArduinoMotorShieldR3.h>   //Libreria para usar el motor shield de arduino

ArduinoMotorShieldR3 motores;   //Declaración de los motores
//M1 = motor derecho
//M2 = motor izquierdo

#define blanco 1
#define negro 0
#define velI 170
#define velD 173

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

int dl1=0;
int dl2=0;
int dl3=0;
int dl4=0;
int dl5=0;
int dl6=0;
int dl7=0;
int dl8=0;

//valores de blanco y negro
int bco=620;  //600
int neg=720;

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
  if (l5<=bco){dl5=1;} else {dl5=0;}
  if (l6<=bco){dl6=1;} else {dl6=0;}
  if (l7<=bco){dl7=1;} else {dl7=0;}
  if (l8<=bco){dl8=1;} else {dl8=0;}
  //Moverse en linea recta
  Moverse(velD,velI);
  
  /*
  //l3 detecta negro
  if(dl3==negro){
    Moverse(velD+20,velI-50);
  }
  //l5 detecta negro
  if(dl6==negro){
    Moverse(velD-50,velI+20);
  }
  if(dl3==negro && dl2==negro && dl1==negro || dl4==negro && dl3==negro && dl2==negro && dl1==negro){
    Moverse(velD+100,velI-130);
  }
  if(dl6==negro && dl7==negro && dl8==negro || dl5==negro && dl6==negro && dl7==negro && dl8==negro){
    Moverse(velD-130,velI+100);
  }
  if(dl5==negro && dl4==negro && dl3==negro && dl2==negro && dl1==negro){
    Moverse(velD+100,velI-130);
  }
  if(dl4==negro && dl5==negro && dl6==negro && dl7==negro && dl8==negro){
    Moverse(velD-130,velI+100);
  }
  if(dl6==negro && dl5==negro && dl4==negro && dl3==negro && dl2==negro && dl1==negro){
    Moverse(velD+120,velI-150);
  }
  if(dl3==negro && dl4==negro && dl5==negro && dl6==negro && dl7==negro && dl8==negro){
    Moverse(velD-150,velI+120);
  }*/


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
    //Moverse(-250,-250);
  }

  if(dl1==negro && dl2==negro && dl3==negro || dl1==negro && dl2==negro && dl3==negro && dl4==negro){
    Moverse(200,-130);
    //Moverse(-250,-250);
  }

  //Intersección
  if(dl1==negro && dl8==negro){
    //Moverse(0,0);
    //delay(1000);
  }

  //imprimirDatosInf();
  //delay(800);
  

}
//Movimiento de los motores
int Moverse(int velD_, int velI_){
  motores.setM1Speed(velD_);
  motores.setM2Speed(velI_);
}

//Función para imprimir valores del sensor
void imprimirDatosInf(){
  Serial.print("Izq 1: ");Serial.print(l1);Serial.print("\ D Izq 1: ");Serial.println(dl1);
  Serial.print("Izq 2: ");Serial.print(l2);Serial.print("\ D Izq 1: ");Serial.println(dl2);
  Serial.print("Izq 3: ");Serial.print(l3);Serial.print("\ D Izq 1: ");Serial.println(dl3);
  Serial.print("Cen i: ");Serial.print(l4);Serial.print("\ D Izq 1: ");Serial.println(dl4);
  Serial.print("Cen d: ");Serial.print(l5);Serial.print("\ D Izq 1: ");Serial.println(dl5);
  Serial.print("Der 6: ");Serial.print(l6);Serial.print("\ D Izq 1: ");Serial.println(dl6);
  Serial.print("Der 7: ");Serial.print(l7);Serial.print("\ D Izq 1: ");Serial.println(dl7);
  Serial.print("Der 8: ");Serial.print(l8);Serial.print("\ D Izq 1: ");Serial.println(dl8);
  Serial.println("-----------------------------------");
}
