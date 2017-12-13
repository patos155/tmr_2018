#include <AFMotor.h>
AF_DCMotor Md(2);   //motor derecho
AF_DCMotor Mi(3);   //motor izquierdo

//Pines del sensor infrarrojo
int izq_1=52;
int izq_2=48;
int centro_3=51;
int der_4=50;
int der_5=53;

//Variables para guardar datos del sensor infrarrojo
int l1=0;
int l2=0;
int l3=0;
int l4=0;
int l5=0;


void setup() {
  //Se inicia la comunicación Serial
  Serial.begin(9600);

  //Configurando pines del sensor infrarrojo como entradas
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(centro_3,INPUT);
  pinMode(der_4,INPUT);
  pinMode(der_5,INPUT);

}

void loop() {
  //Se guardan datso del sensor infrarrojo
  l1=digitalRead(izq_1);
  l2=digitalRead(izq_2);
  l3=digitalRead(centro_3);
  l4=digitalRead(der_4);
  l5=digitalRead(der_5);

  
//Desacomodarse para recuperar línea
  if(l1==0 && l2==0 && l3==1 && l4==0 && l5==0 || l1==0 && l2==0 && l4==0 && l5==0){
    Quieto();             //Se detiene
    delay(500);
    Retroceder();         //Va hacia atrás
    delay(1000);
    Recuperar();          //Se desvia un poco para después encontrar nuevamente la línea
    delay(500);
    //imprimirDatos();
  }
//Muy desviado a la izquierda
  if(l1==0 || l1==0 && l2==0){    //Curvas cerradas
    GirarI2();            //Gira a la izquierda con mayor velocidad normal
  }
//Muy desviado a la derecha
  if(l5==0 || l5==0 && l4==0){    //Curvas cerradas
    GirarD2();            //Gira a la derecha con mayor velocidad normal 
  }

//CENTRADO  
  if(l1==1 && l2==1 && l4==1 && l5==1){
    Avanzar();            //Avanza hacia adelante
  }
  
//Desviado un poco a la izquierda
  if(l2==0 && l5==1 && l1==1 || l2==0){
    GirarI1();            //Gira a la izquierda con velocidad normal
  }
//Desviado un poco a la derecha
  if(l4==0 && l1==1 && l5==1 || l4==0){
    GirarD1();
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
  if(l2==0 && l4==0){     //Si se encuentra una intersección
    Avanzar();            //Avanza hacia adelante (por el momento)
  }

}

//Función para girar a la derecha con mayor velocidad
void GirarD2(){
  Md.setSpeed(225);
  Md.run(BACKWARD);
  Mi.setSpeed(250);
  Mi.run(FORWARD);
}

//Función para girar a la izquierda con mayor velocidad
void GirarI2(){
  Mi.setSpeed(225);
  Mi.run(BACKWARD);
  Md.setSpeed(250);
  Md.run(FORWARD);
}

//Función para girar a la derecha con velocidad normal
void GirarD1(){
  Md.setSpeed(150);
  Md.run(BACKWARD);
  Mi.setSpeed(200);
  Mi.run(FORWARD);
}

//Función para girar a la izquierda con velocidad normal
void GirarI1(){
  Mi.setSpeed(150);
  Mi.run(BACKWARD);
  Md.setSpeed(200);
  Md.run(FORWARD);
}

//Función para avanzar 
void Avanzar(){
  Md.setSpeed(115);
  Md.run(FORWARD);
  Mi.setSpeed(115);
  Mi.run(FORWARD);
}

//Función para ir de reversa
void Retroceder(){
  Md.setSpeed(115);
  Md.run(BACKWARD);
  Mi.setSpeed(115);
  Mi.run(BACKWARD);
}

//Función para desviarse un poco para luego recuperar la línea
void Recuperar(){
  Md.setSpeed(200);
  Md.run(BACKWARD);
  Mi.setSpeed(0);
  Mi.run(RELEASE);
}

//Función para detenerse
void Quieto(){
  Md.setSpeed(0);
  Md.run(RELEASE);
  Mi.setSpeed(0);
  Mi.run(RELEASE);
}

//Función para imprimir valores del sensor
void imprimirDatos(){
  Serial.print("Izq ext:");Serial.println(l5);
  Serial.print("Izq int:");Serial.println(l4);
  Serial.print("Centro:");Serial.println(l3);
  Serial.print("Der int:");Serial.println(l2);
  Serial.print("Der ext:");Serial.println(l1);
}

