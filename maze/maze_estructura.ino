//---------------------------------------------------------------------------------------------------------------------
//                                                 Librerias
//---------------------------------------------------------------------------------------------------------------------
#include <SD.h>
#include <QTRSensors.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Servo.h>

// --------------------------------------------------------------------------------------------------------------------
//                                 objetos derivados de librerias (temperatura y servo) 
//---------------------------------------------------------------------------------------------------------------------
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo kits;
//define motores por cada puerto
AF_DCMotor motori_D(1); //motor izquierdo 
AF_DCMotor motord_D(2); //motor derecho


//---------------------------------------------------------------------------------------------------------------------
//                                      Variables de control para sensores
//---------------------------------------------------------------------------------------------------------------------

//Ultrasonicos 
     //Delantero
       const int trigPin_D = 35;
       const int echoPin_D = 37;
     //Derecho
       const int trigPin_R = 39;
       const int echoPin_R = 41;
     //Izquierdo
       const int trigPin_L= 33; 
       const int echoPin_L= 31; 
    //Variables de trabajo para calculo de sensores ultrasonicos 
       long duration;
       long inches;
       float cm_D; 
       float cm_R; 
       float cm_L; 
       float cm_A; 
       float dif_cm=0;

// temperatura (victima)
    float dif_temp=0;
    float temp1=0;
    float temp2=0;
     int mot_kit=36;

 //Infrarojos   (piso negro)
    //pines de conexion
      int l1 = 49; //Morado
      int l2 = 47; //Gris
      int l3 = 45; //Blanco
    //Variales de lectura
      int v1 = 0;
      int v2 = 0;
      int v3 = 0; 

 //Inclinacion (desactiva centrado para subir y/o bajar la rampa)
    int SI=50;
    int inclina=1;
    int ind_vic=36;

//---------------------------------------------------------------------------------------------------------------------
//                                      Variables con parametros de funcionamiento
//---------------------------------------------------------------------------------------------------------------------
    // inicia ciclo
       int inicio=0;
    // tiempo de espera para pruebas 
    int espera=3000;
    // distancia para encontrar las paredes (centimetros)
       int d_enc=30; 
       int d_fte=15;
    //encuentro con las paredes del laberinto (0) libre, (1) encuentra la pared 
       int adelante=0;
       int derecha=0;
       int izquierda=0;
       
    //velocidad de oruga
       int ade_ordi=140;
       int medioi=115;
       int ade_ordd=190;
       int mediod=150;
       
    //----------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------
    // ////////////////////////////////////////////// espara para los giros
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
       //tiempo para los giros de 90° Izquierda, Derecha y giro en U
            int t_giroi=3450;      
            int t_girod=3800;    
            int t_ui=9000;
            int t_ud=10500;
        // Tiempo de inercia (espera para comenzar el giro al detectar la falta de pared) Izquierda, Derecha y Zona Negra
            int ineI=1100;        
            int ineD=1200;
            int ine_ng=1500; 
       //  Tiempo para avanzar al frente sin buscar paredes luego de efectuar un giro 
            int esp_giro=3000;    //avanza despues de girar 180°
            int espera_u=2000;   // ??
            //int tr=40;            ??
            //int giro=0;           ??

       String ul_giro;
       String desvio;


//------------------------------------------------------------------------------------------------------------------------------------------
//                                                              Setup
//------------------------------------------------------------------------------------------------------------------------------------------    
void setup() {
   //Puerto serial 
        Serial.begin(9600); 
   // Sensor ultrasonico Delantero
      pinMode(trigPin_D, OUTPUT); 
      pinMode(echoPin_D, INPUT); 
      digitalWrite(trigPin_D, LOW); 
   // Sensor ultrasonico derecho
      pinMode(trigPin_R, OUTPUT); 
      pinMode(echoPin_R, INPUT); 
      digitalWrite(trigPin_R, LOW); 

   // Sensor ultrasonico izquierdo
      pinMode(trigPin_L, OUTPUT); 
      pinMode(echoPin_L, INPUT); 
      digitalWrite(trigPin_L, LOW); 

   //Sensor de temperatura (victimas)
        mlx.begin(); 

   // Sensor Infrarojo (piso negro)
        pinMode(l1,INPUT);
        pinMode(l2,INPUT);
        pinMode(l3,INPUT);

   //Sensor de Inclinación (rampa)
        pinMode(SI,INPUT);
        pinMode(ind_vic,OUTPUT); 

  // Servo para dispensador de kits de salvamento 
        kits.attach(9);
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                              Loop
//------------------------------------------------------------------------------------------------------------------------------------------    

void loop() {
// --------------------------------------Lectura de sensores y calculos respectivos -------------------------------------------------------
  // Sensores ultrasonicos (distancia a la pared)
       if (inicio==0){
           for (int i=0;i<=100;i++){
               lee();
           }
           inicio=1;
       }

   lee();
   //muestra datos de sensores en el monitor serial
   //muestra();

   //ajuste para giros
   //giro_izquierda();
   //giro_derecha();
   //giro_u();
   //alto();
   //delay(3000);
   
   
   
   // busca el camino 
   busca();
  
}


void lee(){
         // lee sensores y determina distancia a las paredes
       ultra_D();
       ultra_R();
       ultra_L();
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
  
   //-Lectura de infrarojos (piso negro)
       v1=digitalRead(l1);
       v2=digitalRead(l2);
       v3=digitalRead(l3);

   // Inclinacion
       inclina=digitalRead(SI); 

   // lectura de temperatura 
       temp1=mlx.readAmbientTempC();
       temp2=mlx.readObjectTempC();
       // determina si encuentra una victima viva (diferencia de tempertura)
       dif_temp=temp2-temp1;

}

//---------------------------------------------------------------------------------------------------------------------------
//                                     muestra valores de lecturas en sensores
//---------------------------------------------------------------------------------------------------------------------------
void muestra(){
    // ultrasonicos 
    Serial.println("----------------------------------------------------------------------------------------------------------------");
    Serial.print("delantero  ");
    Serial.println(cm_D);
    Serial.print("derecho  ");
    Serial.println(cm_R);
    Serial.print("Izquierdo ");
    Serial.println(cm_L);
    Serial.print("Pared delantera  ");
    Serial.println(adelante);
    Serial.print("Pared derecha  ");
    Serial.println(derecha);
    Serial.print("Pared Izquierda ");
    Serial.println(izquierda);
    delay(espera);
   
    //-Lectura de infrarojos (piso negro)
    Serial.println("Infra rojos");
    Serial.print("l1: ");
    Serial.println(v1);
    Serial.print("l2: ");
    Serial.println(v2);
    Serial.print("l3: ");
    Serial.println(v3);
    delay(espera);
    // Inclinacion
    Serial.println("Inclinacion");
    Serial.println(inclina);
    delay(espera);
    // lectura de temperatura 
    Serial.println("lectura de datos de temperatura");
    Serial.println("Ambiente = ");
    Serial.println(temp1);
    Serial.print("tObjeto = ");
    Serial.println(temp2);
    // determina si encuentra una victima viva (diferencia de tempertura)
    Serial.print("Diferencia ");
    Serial.println(dif_temp);
    delay(espera);

}



//----------------------------------------------------------------------------------------------------------------------------
//                                                Procedimeintos de movimientos 
//----------------------------------------------------------------------------------------------------------------------------


//busca el camino
void busca(){
  //-----------------------------Movimientos derivados de la lectura de los sensores -----------------------------------------
       //  Si encuentra una victima caliente entrega el kit de salvamento 
       if (dif_temp>10){
          entrega_kit();
       }

       //Sin pared al frente y entre dos paredes avanza de frente alineandose a la IZQUIERDA
       //          |         |          |            |
       //          |    1    |          |     1      |
       //          |0       0|          |0         1 
       //          |         |          |            |
       if (((adelante==1) && (izquierda==0) && (derecha==0)) || ((adelante==1) && (izquierda==0) && (derecha==1))){
          if(inclina == 1){
                avanza();
                
          } else{
              rampa();
          }
          ul_giro="FT"; //+desvio;
       }     
    
       //Pared al frente y a la izquierda gira a la derecha
       //          |----------|
       //          |    0    
       //          |0       1
       //          |          |
       if (((adelante==0)   && (izquierda==0) && (derecha==1))){
          giro_derecha();
       }
    
       //Pared al frente y a la derecha gira a la izquierda 
       //          |----------|
       //                0    |
       //            1       0|
       //          |          |
       if ((adelante==0) && (izquierda==1) && (derecha==0)){
          giro_izquierda();
       }
       
       //Pared al frente y sin pared a la derecha  y a la izquierda gira a la izquierda 
       //          |------------|
       //                 0    
       //             1       1
       //          |            |
       if ((adelante==0) && (izquierda==1) && (derecha==1)){
           giro_izquierda();
       }
    
     //Sin Pared al frente con pared a la derecha sin pared a la izquierda gira a la izquierda
     //          |            |
     //                 1     |
     //             1       0 |
     //          |            |
     if ((adelante==1) && (izquierda==1) && (derecha==0)){
           giro_izquierda();
     }
  
     //Sin Pared al frente Sin pared a la derecha y Sin pared a la izquierda gira a la izquierda
      //          
      //                 1    
      //             1       1
      //          |             |
      if ((adelante==1) && (izquierda==1) && (derecha==1)){
           giro_izquierda();
      }
  
     
     
      //Pared al frente pared a la derecha y pared a la izquierda regresa
      //          |-----------|
      //          |     0     |
      //          | 0       0 |
      //          |           |
      if ((adelante==0) && (derecha==0) && (izquierda==0)){
          giro_u();
      }
  
      //detecta negro
      if(v2==1 && v3==1){
        delay(500);
        v1=digitalRead(l1);
        v2=digitalRead(l2);
        v3=digitalRead(l3);
        if(v2==1 && v3==1){
             motori_D.setSpeed(ade_ordi);  
             motori_D.run(BACKWARD);        
             motord_D.setSpeed(ade_ordd);  
             motord_D.run(BACKWARD); 
             delay(ine_ng+1500);
             giro_u();
             //sale del negro
             //motori_D.setSpeed(ade_ordi);  
             //motori_D.run(FORWARD);        
             //motord_D.setSpeed(ade_ordd);  
             //motord_D.run(FORWARD); 
             //delay(ine_ng+1000);
        }
      }
  
 
}








// se detiene 
void alto(){
   motori_D.setSpeed(0);//velocidad de motor izquierdo
   motori_D.run(FORWARD); //polaridad de motor izquierdo
   motord_D.setSpeed(0);//velocidad de motor derecho
   motord_D.run(FORWARD);//polaridad de motor  derecho
   delay(300);
}

// avanza y centra si se desvia a derecha o izquierda
void avanza(){
    motori_D.setSpeed(ade_ordi); 
    motori_D.run(FORWARD);       
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    //desvio="C"; 
    //algoritmo para centrar
    
    if(cm_L<5 || cm_R>=8 && cm_R<d_enc){ 
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
        //----------- delay(700);
        delay(400);
        //desvio="D";
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
        //---------delay(750);
        delay(350);
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
        //desvio="I";
    }
}

// avanza por la rampa a maxima potencia 
void rampa(){
   
    while (inclina==0){
        motori_D.setSpeed(255);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(255);  
        motord_D.run(FORWARD);         
        // Inclinacion
        inclina=digitalRead(SI);  
        if (inclina==1){
            delay(300);
            inclina=digitalRead(SI);  
        }
    }
     
}


// Gira a la derecha 90°
void giro_derecha(){
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

// Gira a la derecha 90°
void giro_izquierda(){
    // avanza un tiempo para centrar la vuelta
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    delay(ineI); //avanza para centrar el giro
    //Gira Izquierda
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


          
// Gira a la derecha 180° giro en U
void giro_u(){
     ultra_R();
     ultra_L();
     //Detremina a donde girar (derecha o izquierda)
       if (cm_R>cm_L) {
          // pegado a la izquierda debe girar a la derecha
          motori_D.setSpeed(ade_ordi);
          motori_D.run(FORWARD); 
          motord_D.setSpeed(ade_ordi);
          motord_D.run(BACKWARD);
          delay(t_ud);
       }else {
           // pegado a la derecha debe girar a la izquierda
          motori_D.setSpeed(ade_ordi);
          motori_D.run(BACKWARD); 
          motord_D.setSpeed(ade_ordi);
          motord_D.run(FORWARD);
          delay(t_ui);
       }
    alto();
    ul_giro="U";
    // avanza para para la iniercia
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    delay(espera_u);
}



// Entrega kits de salvamento 
void entrega_kit(){
   alto();
   Serial.println("gira----");
   digitalWrite(mot_kit,HIGH);
   delay(1000);
   kits.write(0);
   delay(4000);
   kits.write(360);
   delay(3000);
   kits.write(0);
   delay(5000);
}




//----------------------------------------------------------------------------------------------------------------------------
//    procedimientos de lectura de ultrasonicos y determinación de distancia al objeto
//----------------------------------------------------------------------------------------------------------------------------

// Sensor delantero
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
// Sensor derecho
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
// Sensor izquierdo
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

