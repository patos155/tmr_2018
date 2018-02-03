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
   //Inclinación
       int SI=53;
    
      
       
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
    //Sensor de Inclinación 
       int inclina=1;    


       
       
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
       int ade_ordi=140;
       int medioi=115;
        int ade_ordd=190;
       int mediod=150;
       //long dif_cm=0;
       
    //Control de ultimo giro 
       String ul_giro="FR"; 
       String desvio="C";
    // espara para los giros
       int t_giroi=4400;      //tiempo para los giros de 90°
       int t_girod=4400;      //tiempo para los giros de 90°
       int t_u=11800;
       int ineD=2000;
       int ineI=2150;        
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
   //Sensor de Inclinación
     pinMode(SI,INPUT);  
   
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
   //delay(2000);
   Serial.println("Sensores ");
   /*motori_D.setSpeed(ade_ordi); 
   motori_D.run(FORWARD);       
   motord_D.setSpeed(ade_ordd);  
   motord_D.run(FORWARD);
   */
  
   v1=digitalRead(l1);
   v2=digitalRead(l2);
   v3=digitalRead(l3);

   inclina=digitalRead(SI); 
/*
   Serial.print("l1:");
   Serial.println(v1);
   Serial.print("l2:");
   Serial.println(v2);
   Serial.print("l3:");
   Serial.println(v3);
  */  
   Serial.print("Inclina:");
   Serial.println(inclina);
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
  // delay(3000);


   if(inclina == 1){
    

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
        if(cm_L<5 || cm_R>=8 && cm_R<d_enc){ 
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
        
        if(cm_R<5 || cm_L>=8 && cm_L<d_enc){
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
       delay(ineD); //avanza para centrar el giro
       //gira a la derecha
       motori_D.setSpeed(ade_ordi);
       motori_D.run(FORWARD); 
       motord_D.setSpeed(ade_ordd);
       motord_D.run(BACKWARD);
       delay(t_girod);
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
       delay(ineI); //avanza para centrar el giro
       // Gira Izquierda
       motori_D.setSpeed(ade_ordi);
       motori_D.run(BACKWARD); 
       motord_D.setSpeed(ade_ordd);
       motord_D.run(FORWARD);
       delay(t_giroi);
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
       delay(ineI); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       delay(t_giroi);
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
       delay(ineI); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       delay(t_giroi);
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
       delay(ineI); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       delay(t_giroi);
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
        delay(t_u-300);
        alto();
        ul_giro="U";
        motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
        motori_D.run(FORWARD);        //polaridad de motor izquierdo
        motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
        motord_D.run(FORWARD);        //polaridad de motor derecho
    }

    //detecta negro
    if(v1==1 && v2==1 && v3==1){
      delay(1000);
      v1=digitalRead(l1);
      v2=digitalRead(l2);
      v3=digitalRead(l3);
      if(v1==1 && v2==1 && v3==1){
           //avanza
           motori_D.setSpeed(ade_ordi);  
           motori_D.run(FORWARD);        
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           delay(ine_ng-1200);
           //gira
           motori_D.setSpeed(ade_ordi);
           motori_D.run(BACKWARD); 
           motord_D.setSpeed(ade_ordi);
           motord_D.run(FORWARD);
           delay(t_u+200);
           alto();
           ul_giro="U";
           //avanza
           motori_D.setSpeed(ade_ordi);  
           motori_D.run(FORWARD);        
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD); 
           delay(ine_ng+1000);
      }
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
