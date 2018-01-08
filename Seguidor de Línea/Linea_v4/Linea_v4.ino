#include <AFMotor.h>
AF_DCMotor Md(2);   //motor derecho
AF_DCMotor Mi(3);   //motor izquierdo

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

//Pines del ultrasónico
int Trig=24;
int Echo=25;

float dist;

void setup() {
  //Se inicia la comunicación Serial
  Serial.begin(9600);

  //Configurando pines del sensor infrarrojo como entradas
  pinMode(izq_1,INPUT);
  pinMode(izq_2,INPUT);
  pinMode(centro_3,INPUT);
  pinMode(der_4,INPUT);
  pinMode(der_5,INPUT);

//Configuración de sensor ultrasónico
  pinMode(Echo,INPUT);
  pinMode(Trig,OUTPUT);
}

void loop() {
  //Lectura de sensor infrarrojo
  l1=digitalRead(izq_1);
  l2=digitalRead(izq_2);
  l3=digitalRead(centro_3);
  l4=digitalRead(der_4);
  l5=digitalRead(der_5);

  //Lectura de sensor ultrasónico
  dist=ping(Trig,Echo);

//Subida (puede perder la línea)
  if(l1==0 && l2==0 && l3==0 && l4==0 && l5==0){
    Avanzar();
  }

//Centrado
  if (l3==0){
    Avanzar();
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
  if(l1==1 && l2==1 && l4==1 && l5==1){
    Avanzar();            //Avanza hacia adelante
  }
  
//Desviado un poco a la izquierda
  if(l2==0 && l5==1 && l1==1 || l2==0 || l2==0 && l3==0 || l1==0 && l2==0){
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
    Avanzar();            //Avanza hacia adelante (por el momento)
  }


//Objeto cerca
  if(dist<=5 && dist!=0){
     //SE DETIENE 
       Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       //SE INCLINA A LA DERECHA 
       Md.setSpeed(250);
       Md.run(BACKWARD);
       Mi.setSpeed(250);
       Mi.run(FORWARD);
       delay(650);
       //se detiene 
       Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       //AVANZA ////////////////////////////////////////////////////////
        Md.setSpeed(250);
       Md.run(FORWARD);
       Mi.setSpeed(250);
       Mi.run(FORWARD);
       delay(1500);
       //SE DETIENE 
        Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       //GIRA A LA IZQUIERDA 
       Md.setSpeed(250);
       Md.run(FORWARD);
       Mi.setSpeed(250);
       Mi.run(BACKWARD);
       delay(650);
       //SE DETIENE 
       Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       // AVANZA 
       Md.setSpeed(250);
       Md.run(FORWARD);
       Mi.setSpeed(250);
       Mi.run(FORWARD);
       delay(2200);
       // SE DETIENE 
       Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       // GIRA A LA IZQUIERDA 
        Md.setSpeed(250);
       Md.run(FORWARD);
       Mi.setSpeed(100);
       Mi.run(BACKWARD);
       delay(1000);
       // SE DETIENE 
       Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       //AVANZA 
        Md.setSpeed(250);
       Md.run(FORWARD);
       Mi.setSpeed(250);
       Mi.run(FORWARD);
       delay(700);
       //SE DETIENE 
       Md.setSpeed(0);
       Md.run(RELEASE);
       Mi.setSpeed(0);
       Mi.run(RELEASE);
       delay(100);
       // INCORPORA LA LINIA 
       Md.setSpeed(250);
       Md.run(BACKWARD);
       Mi.setSpeed(250);
       Mi.run(FORWARD);
       delay(650);
  }

}

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

/*//Función para ir de reversa
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
}
*/
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
  Serial.println("-----------------------------------");
}

////////////Sensore ultrasónicos
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

