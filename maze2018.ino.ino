//Incluimos la libreria SD.
#include <SD.h>
#include <QTRSensors.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <BMP180.h>  //Library for the BMP180 barometer.



//Acelerometro MPU6050 
MPU6050 accelgyro;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//delantero
//define motores por cada puerto
AF_DCMotor motori_D(1); //motor izquierdo 
AF_DCMotor motord_D(2); //motor derecho

//Declaracion de Variables para sensores
   // SD
       const int chipSelect = 44;
   //Ultrasonicos 
     //Delantero
       const int trigPin_D = 30;
       const int echoPin_D = 32;
     //Derecho
       const int trigPin_R = 34;
       const int echoPin_R = 36;
     //Izquierdo
       const int trigPin_L= 28; // Pin disparador. Se puede usar otro pin digital
       const int echoPin_L= 26; // Pin eco. Se puede usar otro pin digital
   //Infrarojos
      
       int l1 = 44; //Morado
       int l2 = 42; //Gris
       int l3 = 40; //Blanco
  //Giroscopio y Acelerometro 
       int16_t ax, ay, az;
       int16_t gx, gy, gz;

  //HMC5883L Magnetometro Compass
       const int hmc5883Address = 0x1E; //0011110b, I2C 7bit address for compass
       const byte hmc5883ModeRegister = 0x02;
       const byte hmcContinuousMode = 0x00;
       const byte hmcDataOutputXMSBAddress = 0x03;

       int x,y,z; //triple axis data from HMC5883L.
  
       
   //variables de medicion de sensores
   //Sensores ultrasonicos 
       long duration;
       long inches;
       float cm_D; 
       float cm_R; 
       float cm_L; 
       float cm_A; 
       float dif_cm=0;
    //Sensor Infrarrojo
       int v1 = 0;
       int v2 = 0;
       int v3 = 0;    
       int cont_inf=0;
    //Sensor de Inclinación 
       //int sInc =    

//Variables de control
    // distancia para encontrar las paredes (centimetros)
       int d_enc=30; 
       int d_fte=10;
    //encuentro con las paredes del laberinto (0) libre, (1) encuentra la pared 
       int adelante=0;
       int derecha=0;
       int izquierda=0;
       int atras=0;
    //velocidad de oruga
       int ade_ordi=140;
       int medioi=115;
        int ade_ordd=190;
       int mediod=150;
       //long dif_cm=0;
       
    //Control de ultimo giro 
       String ul_giro="FR"; 
       String desvio="C";
    // espara para los giros
       int ang_ini=0;
       int ang_act=0; 
       int giroU=30;
       int Gizquierda=15;
       int Gderecha=-15; 


       
       int t_giroi=3750;      //tiempo para los giros de 90°
       int t_girod=4500;      //tiempo para los giros de 90°
       int t_u=10700;
       int ineD=1100;
       int ineI=1100;        
       int ine_ng=1500;       //inercia zona negra
       int esp_giro=4200;    //avanza despues de girar 180°
       //int t_giro_u=3500;   // tiempo para giros de 180°
       int tr=40;           // tiempo de retorno a la recta 
       int giro=0;  
       

void setup() {
   //Puerto serial 
   Serial.begin(9600); 
   
   // Sensor ultrasonico Delantero
      pinMode(trigPin_D, OUTPUT); // Establece pin como salida
      pinMode(echoPin_D, INPUT); // Establece pin como entrada
      digitalWrite(trigPin_D, LOW); // Pone el pin a un estado logico bajo
   
   // Sensor ultrasonico derecho
      pinMode(trigPin_R, OUTPUT); // Establece pin como salida
      pinMode(echoPin_R, INPUT); // Establece pin como entrada
      digitalWrite(trigPin_R, LOW); // Pone el pin a un estado logico bajo

   // Sensor ultrasonico izquierdo
      pinMode(trigPin_L, OUTPUT); // Establece pin como salida
      pinMode(echoPin_L, INPUT); // Establece pin como entrada 
      digitalWrite(trigPin_L, LOW); // Pone el pin a un estado logico bajo

   // Sensor Infrarojo
      pinMode(l1,INPUT);
      pinMode(l2,INPUT);
      pinMode(l3,INPUT);


    //------------------  Configuracion GY-87 ------------------------------------
      Wire.begin();
      // inicializa griroscopio
      accelgyro.initialize();
      // verifica la coneccion
      Serial.println("Testing device connections...");
      Serial.println(accelgyro.testConnection() ? "MPU6050 se realizo la coneccion" : "MPU6050 coneccion fallida");
      accelgyro.setI2CBypassEnabled(true);  //Esto configura el bypass para que el HMC5883L se pueda ver.
      //Inicializa el Compas digital
      Wire.beginTransmission(hmc5883Address);  //inicia la comunicacion
      Wire.write(hmc5883ModeRegister);  //seleccione el modo de registro
      Wire.write(hmcContinuousMode); //modo de medición continua
      Wire.endTransmission();
    //------------------  Configuracion GY-87 ------------------------------------
}
 
//------------------------------alto-----------------------------------------------
void alto(){
   motori_D.setSpeed(0);//velocidad de motor izquierdo
   motori_D.run(FORWARD); //polaridad de motor izquierdo
   motord_D.setSpeed(0);//velocidad de motor derecho
   motord_D.run(FORWARD);//polaridad de motor  derecho
   //Serial.println("____ALTO____________________");
   delay(1500);
   motori_D.setSpeed(ade_ordi); 
   motori_D.run(FORWARD);       
   motord_D.setSpeed(ade_ordd);  
   motord_D.run(FORWARD);        
}
//------------------------------regresa---------------------------------------------
void regresa(){
   motori_D.setSpeed(100);//velocidad de motor izquierdo
   motori_D.run(BACKWARD); //polaridad de motor izquierdo
   motord_D.setSpeed(100);//velocidad de motor derecho
   motord_D.run(BACKWARD);//polaridad de motor  derecho
   //Serial.println("____regresa____________________");
   delay(2000);
}



void loop() {
   Serial.println("Sensores ");
   /*motori_D.setSpeed(ade_ordi); 
   motori_D.run(FORWARD);       
   motord_D.setSpeed(ade_ordd);  
   motord_D.run(FORWARD);
   */
   angulo();
   Serial.print("Angulo:  ");
   Serial.println(angulo());

   
   v1=digitalRead(l1);
   v2=digitalRead(l2);
   v3=digitalRead(l3);

   ultra_D();
   ultra_R();
   ultra_L();
   Serial.print("delantero  ");
   Serial.println(cm_D);
   Serial.print("derecho  ");
   Serial.println(cm_R);
   Serial.print("Izquierdo ");
   Serial.println(cm_L);
   delay(1000);
   //Detremina si encontro la pared Delantera
   /*if (cm_D<d_fte) {
      adelante=0;
   }else {
      adelante=1;
   }
   //Detremina si encontro la pared Derecha
   if (cm_R<d_enc) {
      derecha=0;
   }else {
       derecha=1;
   }
   //Detremina si encontro la pared Izquierda
   if (cm_L<d_enc) {
       izquierda=0;
   }else {
       izquierda=1;
   }

   Serial.print("Pared delantera  ");
   Serial.println(adelante);
   Serial.print("Pared derecha  ");
   Serial.println(derecha);
   Serial.print("Pared Izquierda ");
   Serial.println(izquierda);
   Serial.print("iNF. 1  ");
   Serial.println(v1);
   Serial.print("inf 2  ");
   Serial.println(v2);
   Serial.print("inf 3 ");
   Serial.println(v3);

   
   //delay(3000);
   //Sin pared al frente y entre dos paredes avanza de frente alineandose a la IZQUIERDA
   //          |         |          |            |
   //          |    1    |          |     1      |
   //          |0       0|          |0         1 
   //          |         |          |            |
   if (((adelante==1) && (izquierda==0) && (derecha==0)) || ((adelante==1) && (izquierda==0) && (derecha==1))){
           //if ((cm_L>=4 && cm_L<=6)){
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           desvio="C"; 
        //}

        //algoritmo para centrar
        if(cm_L<5 || cm_R>=8 ){  //&& cm_R<d_enc
           //dataFile.print("   cm_L<   ");
           // GIRA A LA DERECHA
           motori_D.setSpeed(ade_ordi);
           motori_D.run(FORWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(BACKWARD);
           delay(550);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);                
           delay(700);
           desvio="D";
           //izquierda
           motori_D.setSpeed(ade_ordi);
           motori_D.run(BACKWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(FORWARD);
           delay(400);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           delay(500);
        }
        
        if(cm_R<5 || cm_L>=8){ // && cm_L<d_enc
           //dataFile.print("   cm_L>6");
           //GIRA IZQUIERDA
           motori_D.setSpeed(ade_ordi);
           motori_D.run(BACKWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(FORWARD);
           delay(450);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);                
           delay(750);
           // GIRA A LA DERECHA
           motori_D.setSpeed(ade_ordi);
           motori_D.run(FORWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(BACKWARD);
           delay(450);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);               
           delay(500);
           desvio="I";
        }
        
        ul_giro="FT"+desvio;
   }     
   
   //Pared al frente y a la izquierda gira a la derecha
   //          |----------|
   //          |    0    
   //          |0       1
   //          |          |
   if (((adelante==0)   && (izquierda==0) && (derecha==1))){
       // avanza un tiempo para centrar la vuelta
       alto();
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(ineD); //avanza para centrar el giro
       //gira a la derecha
       ang_ini=angulo();
       ang_act=ang_ini;
       while (ang_act<=ang_ini+Gderecha){
          motori_D.setSpeed(ade_ordi);
          motori_D.run(FORWARD); 
          motord_D.setSpeed(ade_ordd);
          motord_D.run(BACKWARD);
          ang_act=angulo();
       }
       alto();
       // avanza para centrarse
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       
       ul_giro="DE";
   }

   //Pared al frente y a la derecha gira a la izquierda 
   //          |----------|
   //                0    |
   //            1       0|
   //          |          |
   if ((adelante==0) && (izquierda==1) && (derecha==0)){
       alto();
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       //delay(ineI); //avanza para centrar el giro
       // Gira Izquierda
       ang_ini=angulo();
       ang_act=ang_ini;
       while (ang_act<=ang_ini+Gizquierda){
          motori_D.setSpeed(ade_ordi);
          motori_D.run(BACKWARD); 
          motord_D.setSpeed(ade_ordd);
          motord_D.run(FORWARD);
          ang_act=angulo();
       }
       alto();
       // se centra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       
       ul_giro="IZ";
   }
   
   //Pared al frente y sin pared a la derecha  y a la izquierda gira a la izquierda 
   //          |------------|
   //                 0    
   //             1       1
   //          |            |
   if ((adelante==0) && (izquierda==1) && (derecha==1)){
       alto();
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       //delay(ineI); //avanza para centrar el giro
       // Gira Izquierda
       ang_ini=angulo();
       ang_act=ang_ini;
       while (ang_act<=ang_ini+Gizquierda){
          motori_D.setSpeed(ade_ordi);
          motori_D.run(BACKWARD); 
          motord_D.setSpeed(ade_ordd);
          motord_D.run(FORWARD);
          ang_act=angulo();
       }
       alto();
       // se centra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       ul_giro="IZ";
   }

   //Sin Pared al frente con pared a la derecha sin pared a la izquierda gira a la izquierda
   //          |            |
   //                 1     |
   //             1       0 |
   //          |            |
   if ((adelante==1) && (izquierda==1) && (derecha==0)){
       alto();
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       //delay(ineI); //avanza para centrar el giro
       // Gira Izquierda
       ang_ini=angulo();
       ang_act=ang_ini;
       while (ang_act<=ang_ini+Gizquierda){
          motori_D.setSpeed(ade_ordi);
          motori_D.run(BACKWARD); 
          motord_D.setSpeed(ade_ordd);
          motord_D.run(FORWARD);
          ang_act=angulo();
       }
       alto();
       // se centra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       
       ul_giro="IZ";   }

   //Sin Pared al frente Sin pared a la derecha y Sin pared a la izquierda gira a la izquierda
    //          
    //                 1    
    //             1       1
    //          |             |
    if ((adelante==1) && (izquierda==1) && (derecha==1)){
       alto();
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       //delay(ineI); //avanza para centrar el giro
       // Gira Izquierda
       ang_ini=angulo();
       ang_act=ang_ini;
       while (ang_act<=ang_ini+Gizquierda){
          motori_D.setSpeed(ade_ordi);
          motori_D.run(BACKWARD); 
          motord_D.setSpeed(ade_ordd);
          motord_D.run(FORWARD);
          ang_act=angulo();
       }
       alto();
       // se centra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       
       ul_giro="IZ";    }

   
   
    //Pared al frente pared a la derecha y pared a la izquierda regresa
    //          |-----------|
    //          |     0     |
    //          | 0       0 |
    //          |           |
    if ((adelante==0) && (derecha==0) && (izquierda==0)){
        alto();
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       //delay(ineI); //avanza para centrar el giro
       // Gira en U
       ang_ini=angulo();
       ang_act=ang_ini;
       while (ang_act<=ang_ini+giroU){
          motori_D.setSpeed(ade_ordi);
          motori_D.run(BACKWARD); 
          motord_D.setSpeed(ade_ordd);
          motord_D.run(FORWARD);
          ang_act=angulo();
       }
       alto();
       // se centra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       
       ul_giro="U";
    }

//detecta negro
    if(v1==1 && v2==1 && v3==1){
      cont_inf++;
      v1=digitalRead(l1);
      v2=digitalRead(l2);
      v3=digitalRead(l3); 
      if(cont_inf>300){
          alto();
          // avanza un tiempo para centrar la vuelta
          motori_D.setSpeed(ade_ordi);  
           motori_D.run(FORWARD);        
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           //delay(ineI); //avanza para centrar el giro
           // Gira en U
           ang_ini=angulo();
           ang_act=ang_ini;
           while (ang_act<=ang_ini+giroU){
              motori_D.setSpeed(ade_ordi);
              motori_D.run(BACKWARD); 
              motord_D.setSpeed(ade_ordd);
              motord_D.run(FORWARD);
              ang_act=angulo();
           }
           alto();
          // se centra
          motori_D.setSpeed(ade_ordi);  
          motori_D.run(FORWARD);        
          motord_D.setSpeed(ade_ordd);  
          motord_D.run(FORWARD);        
          delay(esp_giro);
          ul_giro="U";
          cont_inf=0;
      }
    }*/
}
   

void ultra_D(){
  digitalWrite(trigPin_D, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin_D, LOW);
  duration = pulseIn(echoPin_D, HIGH); //Devuelve la longitud del pulso del pin Echo en us
  // Imprime valores por el puerto serie:
  // Convierte el tiempo de recepción del eco en distancia:
  inches = microsecondsToInches(duration);
  cm_D = microsecondsToCentimeters(duration);
}

void ultra_R(){
  digitalWrite(trigPin_R, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin_R, LOW);
  duration = pulseIn(echoPin_R, HIGH); //Devuelve la longitud del pulso del pin Echo en us
  // Imprime valores por el puerto serie:
  // Convierte el tiempo de recepción del eco en distancia:
  inches = microsecondsToInches(duration);
  cm_R = microsecondsToCentimeters(duration);
  //cm_R=10;
}

void ultra_L(){
  digitalWrite(trigPin_L, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin_L, LOW);
  duration = pulseIn(echoPin_L, HIGH); //Devuelve la longitud del pulso del pin Echo en us
  // Imprime valores por el puerto serie:
  // Convierte el tiempo de recepción del eco en distancia:
  inches = microsecondsToInches(duration);
  cm_L = microsecondsToCentimeters(duration);
}



// Calcula la distancia en pulgadas
float microsecondsToInches(long microseconds) {
    return microseconds/148;
}
// Calcula la distancia en cm
float microsecondsToCentimeters(long microseconds) {
    return microseconds/58;
} 

int angulo(){
    //------------------------giroscopio---------------------
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //Acceso a la brújula digital HMC5883L
    //Indica a la HMC5883L dónde comenzar a leer los datos
    Wire.beginTransmission(hmc5883Address);
    Wire.write(hmcDataOutputXMSBAddress);  //Selecciona los registros 3, X MSB register
    Wire.endTransmission();
    Wire.requestFrom(hmc5883Address,6);   //Leer datos de cada eje de la brújula digital
    if(6<=Wire.available())
    {
      x = Wire.read()<<8; //X msb
      x |= Wire.read();   //X lsb
      z = Wire.read()<<8; //Z msb
      z |= Wire.read();   //Z lsb
      y = Wire.read()<<8; //Y msb
      y |= Wire.read();   //Y lsb    
    }
    int angle = atan2(-y,x)/M_PI*180;
    if (angle < 0)
    {
      angle = angle + 360;
    }
    return angle;
}

