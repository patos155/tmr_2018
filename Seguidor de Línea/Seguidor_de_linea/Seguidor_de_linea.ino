#include <ArduinoMotorShieldR3.h>   //Libreria para usar el nuevo motor shield

ArduinoMotorShieldR3 motores;   //Declaración de los motores

//SENSOR INFRARROJO
//Pines del sensor infrarrojo
int izq_1=A5;
int izq_2=A8;
int izq_3=A9;
int centro_i=A10;
int centro_d=A11;
int der_6=A12;
int der_7=A13;
int der_8=A14;

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
int bco=680;
int neg=720;

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
  //delay(1000);

//Encontrar objeto
  if(dist<=7 && dist>0){
    Quieto();
    delay(1000);
  }

//Línea cortada
  if(l1<=bco && l2<=bco && l3<=bco && l4<=bco && l5<=bco && l6<=bco && l7<=bco && l8<=bco || l4>=neg && l5>=neg){
    Avanzar();
  }

  else if(l4>=neg && l5>=neg && (l3>=neg || l6>=neg)){
    Avanzar();
  }

  //Muy desviado a la izquierda
  else if(l1>=neg || l1>=neg && l2>=neg || l1>=neg && l2>=neg && l3>=neg || l1>=neg && l2>=neg && l3>=neg && l4>=neg || l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg || l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg && l6>=neg || l1>=neg && l2>=neg && l3>=neg && l4>=neg && l5>=neg && l6>=neg && l7>=neg){    //Curvas cerradas
    GirarI2();            //Gira a la izquierda con mayor velocidad normal
  }
//Muy desviado a la derecha
  else if(l8>=neg || l8>=neg && l7>=neg || l8>=neg && l7>=neg && l6>=neg || l8>=neg && l7>=neg && l6>=neg && l5>=neg || l8>=neg && l7>=neg && l6>=neg && l5>=neg && l4>=neg || l8>=neg && l7>=neg && l6>=neg && l5>=neg && l4>=neg && l3>=neg || l8>=neg && l7>=neg && l6>=neg && l5>=neg && l4>=neg && l3>=neg && l2>=neg){    //Curvas cerradas
    GirarD2();            //Gira a la derecha con mayor velocidad normal 
  }

/*/Desviado un poco a la izquierda
  else if(l4>=neg && l5<=bco || l3>=neg && l5<=bco || l2>=neg && l5<=bco){
    GirarI1();            //Gira a la izquierda con velocidad normal
  }
//Desviado un poco a la derecha
  else if(l5>=neg && l4<=bco || l6>=neg && l4<=bco || l7>=neg && l4<=bco){
    GirarD1();            //Gira a la derecha con velocidad normal 
  }*/

  //Desviado un poco a la izquierda
  else if(l2>=neg && l3>=neg){
    GirarI1();            //Gira a la izquierda con velocidad normal
  }
//Desviado un poco a la derecha
  else if(l6>=neg && l7>=neg){
    GirarD1();            //Gira a la derecha con velocidad normal 
  }

  /*else if(l3>=neg && l4>=neg && l5<=bco || l2>=neg && l3>=neg){
    GirarI3();
  }

  else if(l6>=neg && l5>=neg && l4<=bco || l7>=neg && l6>=neg){
    GirarD3();
  }*/

/*90° Izquierda
  if(l1==0 && 12==0 || l1==0 && l2==0 && l3==0 || l1==0 && l2==0 && l3==0 && l4==0){
    GirarI2();            //Gira a la izquierda con mayor velocidad
  }
//90° Derecha
  if(l5==0 && l4==0 || l5==0 && l4==0 && l3==0 || l5==0 && l4==0 && l3==0 && l2==0){
    GirarD2();            //Gira a la derecha con mayor velocidad
  }
*/

//Interseccion
  else if(l1>=neg && l8>=neg && l2>=neg && l7>=neg){     //Si se encuentra una intersección
    Quieto();
    /*delay(1000);
    leerRGB();                                  //Lee los colores que los sensores detectan
    //imprimirRGB();
    //delay(800);
    if(v1<480 && v1>450 && v1<r1 && v1<a1 && v2<700 && v2>544 && v2<r2 && v2<a2){     //Si ambos sensores detectan verde
      Retorno();                                            //Da media vuleta y vuelve a seguir la línea
      delay(1000);
    }else{
      if(v1<480 && v1>450 && v1<r1 && v1<a1 && v2>544){                        //Si verde es mayor a 1000 y menor a rojo y azul
      GirarI2();
      delay(250);                                          //Da vuelta hacia la izquierda
      }else{
        if(v2<700 && v2>544 && v2<r2 && v2<a2 && v1>480){                      //Si verde es mayor a 1000 y menor a rojo y azul
          GirarD2();                                          //Da vuelta hacia la derecha
          delay(250);
        }else{                                              //Si no detecta ningun verde
          Avanzar();
        }
      }
    }
    Quieto();
    delay(1000);*/
  }
  
}

//Velocidad máxima de motores: 400
//Función para girar a la derecha con velocidad normal
void GirarD3(){
  //Serial.println("GirarD1");
  motores.setM1Speed(-20); //-20
  motores.setM2Speed(80);  //80
}

//Función para girar a la izquierda con velocidad normal
void GirarI3(){
  //Serial.println("GirarI1");
  motores.setM2Speed(-20);  //-20
  motores.setM1Speed(80);  //80
}

//Función para girar a la derecha con mayor velocidad
void GirarD2(){
  //Serial.println("GirarD2");
  motores.setM1Speed(-255); //170
  motores.setM2Speed(200);  //200
}

//Función para girar a la izquierda con mayor velocidad
void GirarI2(){
  //Serial.println("GirarI2");
  motores.setM2Speed(-255); //-170
  motores.setM1Speed(200);  //200
}

//Función para girar a la derecha con velocidad normal
void GirarD1(){
  //Serial.println("GirarD1");
  motores.setM1Speed(-120); //-130
  motores.setM2Speed(200);  //180
}

//Función para girar a la izquierda con velocidad normal
void GirarI1(){
  //Serial.println("GirarI1");
  motores.setM2Speed(-20);  //-130
  motores.setM1Speed(200);  //180
}

//Función para avanzar 
void Avanzar(){
  //Serial.println("Avanza");
  motores.setM1Speed(125);  //125
  motores.setM2Speed(100);  //100
}

//Funcióon para girar sobre su eje
void Retorno(){
  //Serial.println("Retorno");
  motores.setM2Speed(100);
  motores.setM1Speed(-100);
}

//Función para detenerse
void Quieto(){
  //Serial.println("Quieto");
  motores.setM1Speed(0);
  motores.setM2Speed(0);
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

