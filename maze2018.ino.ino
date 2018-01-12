//Incluimos la libreria SD.
#include <SD.h>
#include <QTRSensors.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
//delantero
//define motores por cada puerto
AF_DCMotor motori_D(1); //motor izquierdo 
AF_DCMotor motord_D(2);   //motor derecho

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
   //variables de medicion de sensores
   //Sensores ultrasonicos 
       long duration;
       long inches;
       float cm_D; 
       float cm_R; 
       float cm_L; 
       float cm_A; 
       float dif_cm=0;


       
       
//Variables de control
    // distancia para encontrar las paredes (centimetros)
       int d_enc=30; 
       int d_fte=15;
    //encuentro con las paredes del laberinto (0) libre, (1) encuentra la pared 
       int adelante=0;
       int derecha=0;
       int izquierda=0;
       int atras=0;
    //velocidad de oruga
       int ade_ordi=150;
       int medioi=115;
       int ade_ordd=200;
       int mediod=150;
       //long dif_cm=0;
       
    //Control de ultimo giro 
       String ul_giro="FR"; 
       String desvio="C";
    // espara para los giros
       int t_giro=6000;      //tiempo para los giros de 90°
       int t_u=4700;
       int ine=2800;         //
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
   
   }
 
//------------------------------alto-----------------------------------------------
void alto(){
   motori_D.setSpeed(0);//velocidad de motor izquierdo
   motori_D.run(FORWARD); //polaridad de motor izquierdo
   motord_D.setSpeed(0);//velocidad de motor derecho
   motord_D.run(FORWARD);//polaridad de motor  derecho
   //Serial.println("____ALTO____________________");
   delay(100);
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
   //delay(1000);
   Serial.println("Sensores ");
   /*motori_D.setSpeed(ade_ordi); 
   motori_D.run(FORWARD);       
   motord_D.setSpeed(ade_ordd);  
   motord_D.run(FORWARD);        */
   ultra_D();
   ultra_R();
   ultra_L();
   Serial.print("delantero  ");
   Serial.println(cm_D);
   Serial.print("derecho  ");
   Serial.println(cm_R);
   Serial.print("Izquierdo ");
   Serial.println(cm_L);
   //delay(1000);
   //Detremina si encontro la pared Delantera
   if (cm_D<d_fte) {
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
        if(cm_L<3){ 
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
           delay(500);
           desvio="D";
           //izquierda
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
           delay(300);
        }
        
        if(cm_R<3){
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
           delay(550);
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
           delay(300);
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
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(ine); //avanza para centrar el giro
       //gira a la derecha
       motori_D.setSpeed(ade_ordi);
       motori_D.run(FORWARD); 
       motord_D.setSpeed(ade_ordd);
       motord_D.run(BACKWARD);
       delay(t_giro);
       // avanza para centrarse
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       alto();
       // arranca de nuevo
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       ul_giro="DE";
   }
   
   //Pared al frente y a la derecha gira a la izquierda 
   //          |----------|
   //                0    |
   //            1       0|
   //          |          |
   if ((adelante==0) && (izquierda==1) && (derecha==0)){
       
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(ine); //avanza para centrar el giro
       // Gira Izquierda
       motori_D.setSpeed(ade_ordi);
       motori_D.run(BACKWARD); 
       motord_D.setSpeed(ade_ordd);
       motord_D.run(FORWARD);
       delay(t_giro);
       // se centra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       delay(esp_giro);
       alto();
       // arranca
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       ul_giro="IZ";
   }
   
   //Pared al frente y sin pared a la derecha  y a la izquierda gira a la izquierda 
   //          |------------|
   //                 0    
   //             1       1
   //          |            |
   if ((adelante==0) && (izquierda==1) && (derecha==1)){
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       delay(ine); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       delay(t_giro);
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       delay(esp_giro);
       alto();
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       ul_giro="IZ";
   }

   //Sin Pared al frente con pared a la derecha sin pared a la izquierda gira a la izquierda
   //          |            |
   //                 1     |
   //             1       0 |
   //          |            |
   if ((adelante==1) && (izquierda==1) && (derecha==0)){
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       delay(ine); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       delay(t_giro);
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       delay(esp_giro);
       alto();
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       ul_giro="IZ";
   }

   //Sin Pared al frente Sin pared a la derecha y Sin pared a la izquierda gira a la izquierda
    //          
    //                 1    
    //             1       1
    //          |             |
    if ((adelante==1) && (izquierda==1) && (derecha==1)){
       delay(ine); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       delay(t_giro);
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       delay(esp_giro);
       alto();
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       ul_giro="IZ";
    }

   
   
    //Pared al frente pared a la derecha y pared a la izquierda regresa
    //          |-----------|
    //          |     0     |
    //          | 0       0 |
    //          |           |
    if ((adelante==0) && (derecha==0) && (izquierda==0)){
        motori_D.setSpeed(ade_ordi);
        motori_D.run(FORWARD); 
        motord_D.setSpeed(ade_ordi);
        motord_D.run(BACKWARD);
        delay(t_u);
        alto();
        ul_giro="U";
        motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
        motori_D.run(FORWARD);        //polaridad de motor izquierdo
        motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
        motord_D.run(FORWARD);        //polaridad de motor  derecho
    }

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
