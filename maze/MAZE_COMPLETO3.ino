//---------------------------------------------------------------------------------------------------------------------
//                                                 Librerias
//---------------------------------------------------------------------------------------------------------------------

#include <SD.h>
#include <QTRSensors.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MLX90614.h>
#include <Servo.h>

extern "C" {
  #include "utility/twi.h" // from Wire library, so we can do bus scanning
}
//                                                                                        #include <Adafruit_HMC5883_U.h>
#define TCAADDR 0x70



// --------------------------------------------------------------------------------------------------------------------
//                                 objetos derivados de librerias (temperatura y servo) 
//---------------------------------------------------------------------------------------------------------------------
Adafruit_MLX90614 m1x = Adafruit_MLX90614();
Adafruit_MLX90614 m2x = Adafruit_MLX90614();
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
       float cm_DL;
       float cm_DR;  
       float cm_R; 
       float cm_L; 
       float cm_A; 
       float dif_cm=0;

// temperatura (victima)
    float dif_temp1=0;
    float temp1_1=0;
    float temp1_2=0;
    float dif_temp2=0;
    float temp2_1=0;
    float temp2_2=0;
    int mot_kit=43;

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
    
//Sensores de contacto
    //izquierda
       int CII = 28;
       int CEI = 26;
       int SII=1;
       int SEI=1;
    //derecha
       int CID = 24;
       int CED = 22;
       int SID=1;
       int SED=1;
    
//---------------------------------------------------------------------------------------------------------------------
//                                      Variables con parametros de funcionamiento
//---------------------------------------------------------------------------------------------------------------------
    // inicia ciclo
      int inicio=0;
    // tiempo de espera para pruebas 
      int espera=5000;
    // distancia para encontrar las paredes (centimetros)
      int d_enc=30; 
      int d_fte=5;
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
            int t_giroi=2000; //3450;      
            int t_giroi_flaso=2800;
            int t_girod_flaso=3200;
            int t_girod=2000; //3800;    
            int t_ui=9000;
            int t_ud=10500;
        // Tiempo de inercia (espera para comenzar el giro al detectar la falta de pared) Izquierda, Derecha y Zona Negra
            int ineI=0;        
            int ineI_falso=0;        
            int ineD=0;
            int ine_ng=1500; 
       //  Tiempo para avanzar al frente sin buscar paredes luego de efectuar un giro 
            int esp_giro=1500;    //avanza despues de girar 180°
            int espera_u=1200;   // ??
            //int tr=40;            ??
            //int giro=0;           ??

       //corrige desviacion 
           int dif_sf=0;
       int cont_ciclo=0;
    

       String ul_giro;
       String desvio;

//------------------------------------------------------------------------------------------------------------------------------------------
//                                           Selecciona Puerto del Multiplexor
//------------------------------------------------------------------------------------------------------------------------------------------    
void tcaselect(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}








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

   // Inicializa el 1er sensor de temperatura Izquierdo
    tcaselect(0);
    if(!m1x.begin()){
       Serial.println("Ooops, Primer MLX90614 no detectado ... verifica tu cableado!");
       while(1);
    }else {
       Serial.println("Inicie Primer MLX90614 !");
    }

    // Inicializa el 2° sensor de temperatura Derecho
    tcaselect(1);
    if(!m2x.begin()){
       Serial.println("Ooops, Segundo MLX90614 no detectado ... verifica tu cableado!");
       while(1);
    }else {
       Serial.println("Inicie Segundo MLX90614 !");
    }

   // Led de victima encontrada
      pinMode(mot_kit, OUTPUT); 

   // Sensor Infrarojo (piso negro)
        pinMode(l1,INPUT);
        pinMode(l2,INPUT);
        pinMode(l3,INPUT);

   //Sensor de Inclinación (rampa)
        pinMode(SI,INPUT);
        pinMode(ind_vic,OUTPUT); 
  //Sensores de Contacto
        pinMode(CEI, INPUT); 
        pinMode(CII, INPUT); 
        pinMode(CED, INPUT); 
        pinMode(CID, INPUT); 
 
  // Servo para dispensador de kits de salvamento 
        kits.attach(10);
        
}

//------------------------------------------------------------------------------------------------------------------------------------------
//                                                              Loop
//------------------------------------------------------------------------------------------------------------------------------------------    

void loop() {
   //inicia posicion del servo del dispensador de kits
   kits.write(90);
   // --------------------------------------Lectura de sensores y calculos respectivos -------------------------------------------------------
   // inicia los senores  
   if (inicio==0){
      for (int i=0;i<=50;i++){
         lee();
      }
      inicio=1;
   }
        
   lee();
   //muestra datos de sensores en el monitor serial
   muestra();

   // busca el camino 
    //busca();

   //ajuste para giros
   //giro_izquierda();
   //giro_derecha();
   //giro_u();
   //alto();
   //delay(3000);
   //if (dif_temp1>10 || dif_temp2>10){
   //  entrega_kit();
   //}

  
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
    sensors_event_t event;
    // lectura de temperatura Izquierdo
    tcaselect(0);
    temp1_1=m1x.readAmbientTempC();
    temp1_2=m1x.readObjectTempC();
    // determina si encuentra una victima viva (diferencia de tempertura)
    dif_temp1=temp1_2-temp1_1;
    tcaselect(1);
    // lectura de temperatura Derecho 
    temp2_1=m2x.readAmbientTempC();
    temp2_2=m2x.readObjectTempC();
    // determina si encuentra una victima viva (diferencia de tempertura)
    dif_temp2=temp2_2-temp2_1;

    //Sensores de contacto
     SEI=digitalRead(CEI);
     SII=digitalRead(CII);
     SED=digitalRead(CED);
     SID=digitalRead(CID);

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
    //Lectura de sensores de contacto
    Serial.print("\nContacto Externo Izquierdo: ");
    Serial.println(SEI);
    Serial.print("Contacto Interno Izquierdo: ");
    Serial.println(SII);
    Serial.print("Contacto Interno Derecho: ");
    Serial.println(SID);
    Serial.print("Contacto Externo Derecho: ");
    Serial.println(SED);
   
    //-Lectura de infrarojos (piso negro)
    Serial.println("Infra rojos");
    Serial.print("l1: ");
    Serial.println(v1);
    Serial.print("l2: ");
    Serial.println(v2);
    Serial.print("l3: ");
    Serial.println(v3);
    // Inclinacion
    Serial.println("Inclinacion");
    Serial.println(inclina);
    // lectura de temperatura izquierda
    Serial.println("lectura de datos de temperatura 1");
    Serial.println("Ambiente = ");
    Serial.println(temp1_1);
    Serial.print("tObjeto = ");
    Serial.println(temp1_2);
    // determina si encuentra una victima viva (diferencia de tempertura)
    Serial.print("Diferencia ");
    Serial.println(dif_temp1);
    
    // lectura de temperatura derecha
    Serial.println("lectura de datos de temperatura 2");
    Serial.println("Ambiente = ");
    Serial.println(temp2_1);
    Serial.print("tObjeto = ");
    Serial.println(temp2_2);
    // determina si encuentra una victima viva (diferencia de tempertura)
    Serial.print("Diferencia ");
    Serial.println(dif_temp2);
    delay(espera);
}



//----------------------------------------------------------------------------------------------------------------------------
//                                                Procedimeintos de movimientos 
//----------------------------------------------------------------------------------------------------------------------------

//busca el camino
void busca(){
  //-----------------------------Movimientos derivados de la lectura de los sensores -----------------------------------------
       //  Si encuentra una victima caliente entrega el kit de salvamento 
       if (dif_temp1>10 || dif_temp2>10){
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
                ul_giro="C";
          } else{
              rampa();
              ul_giro="R";
          }
       }     
    
       //Pared al frente y a la izquierda gira a la derecha
       //          |----------|
       //          |    0    
       //          |0       1
       //          |          |
       if (((adelante==0)   && (izquierda==0) && (derecha==1))){
          ineD=0;
          giro_derecha();
          ul_giro="D";
       }
    
       //Pared al frente y a la derecha gira a la izquierda 
       //          |----------|
       //                0    |
       //            1       0|
       //          |          |
       if ((adelante==0) && (izquierda==1) && (derecha==0)){
          giro_izquierda();
          ul_giro="I";
       }
       
       //Pared al frente y sin pared a la derecha  y a la izquierda gira a la izquierda 
       //          |------------|
       //                 0    
       //             1       1
       //          |            |
       if ((adelante==0) && (izquierda==1) && (derecha==1)){
           giro_izquierda();
           ul_giro="I";
       }
    
       //Sin Pared al frente con pared a la derecha sin pared a la izquierda gira a la izquierda
       //          |            |
       //                 1     |
       //             1       0 |
       //          |            |
       if ((adelante==1) && (izquierda==1) && (derecha==0)){
             ultra_D();
             if (cm_D<30){
                 ineI=2200;
                 giro_izquierda();
             }else {
              
                 ineI_falso=1400;
                 giro_izquierda_falso();
             }
             ul_giro="I";
       }
    
       //Sin Pared al frente Sin pared a la derecha y Sin pared a la izquierda gira a la izquierda
        //          
        //                 1    
        //             1       1
        //          |             |
        if ((adelante==1) && (izquierda==1) && (derecha==1)){
            ver_delante();
            alto();
            ultra_D();
            if (cm_D<30){
               // motori_D.setSpeed(ade_ordi); 
               // motori_D.run(FORWARD);       
               // motord_D.setSpeed(ade_ordd);  
               // motord_D.run(FORWARD);
               // delay(2500);
                //alto();
                ineI_falso=0;
                giro_izquierda();
            }
            else{
              ineI_falso=1500;
              giro_izquierda_falso();
            }
            //giro_izquierda_falso();
            ul_giro="I";
        }
    
       
       
        //Pared al frente pared a la derecha y pared a la izquierda regresa
        //          |-----------|
        //          |     0     |
        //          | 0       0 |
        //          |           |
        if ((adelante==0) && (derecha==0) && (izquierda==0)){
            giro_u();
            ul_giro="U";
        }

      //detecta negro
      
      if(v2==1 && v3==1){
        delay(150);
        v1=digitalRead(l1);
        v2=digitalRead(l2);
        v3=digitalRead(l3);
        if(v2==1 && v3==1){
             motori_D.setSpeed(ade_ordi);  
             motori_D.run(BACKWARD);        
             motord_D.setSpeed(ade_ordd);  
             motord_D.run(BACKWARD); 
             delay(ine_ng+1500);
             giro_u_falso();
             //sale del negro
             //motori_D.setSpeed(ade_ordi);  
             //motori_D.run(FORWARD);        
             //motord_D.setSpeed(ade_ordd);  
             //motord_D.run(FORWARD); 
             //delay(ine_ng+1000);
        }
      }
  
 
}


void ver_delante(){
  // obstaculos
  //SEI,SII,SED,SID
  if (SEI==0 && SED==0){
      alto();
      lee();
      if (izquierda==0 && derecha==1){
         while (SEI==0 && SED==0){
            aladerecha();
            lee();
         }
         delay(200);
         while (SEI==1){
            alaizquierda();
            lee();
         }
      }else {
            if (izquierda==1 && derecha==0){
                 while (SEI==0 && SED==0){
                    alaizquierda();
                    lee();
                 }
                 delay(200);
                 while (SEI==1){
                    aladerecha();
                    lee();
                 }
            }
      }
      
  }else {
      if (SEI==0){
           alto();
           delay(1000);
           atras();
           delay(1000);
           aladerecha();
        }
        if (SII==0){
           alto();
           delay(1000);
           atras();
           delay(1000);
           aladerecha();
           aladerecha();
        }
        if (SID==0){
           alto();
           delay(1000);
           atras();
           delay(1000);
           alaizquierda();
        }
        if (SED==0){
           alto();
           delay(1000);
           atras();
           delay(1000);
           alaizquierda();
           alaizquierda();
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

//Atras
void atras(){
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(BACKWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(BACKWARD); 
}

void aladerecha(){
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

void alaizquierda(){
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



// avanza y centra si se desvia a derecha o izquierda
void avanza(){
    //??????????????????????????????????????????????????????????????negro();
    ineI_falso=1800;
    dif_sf=cm_DL-cm_DR;
    // DESVIADO A LA IZQUIERDA
    if (SEI==0){
        motori_D.setSpeed(ade_ordi); 
        motori_D.run(FORWARD);       
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(BACKWARD);        
        delay(200);
    }
    if (SED==0){
        motori_D.setSpeed(ade_ordi); 
        motori_D.run(BACKWARD);       
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);        
        delay(200);
    }
    motori_D.setSpeed(ade_ordi); 
    motori_D.run(FORWARD);       
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);              
    
    //desvio="C"; 

    //algoritmo para centrar
    
    //if(cm_L<5 || cm_R>=8 && cm_R<d_enc){ 
    if(cm_L<2){
        aladerecha();
    }
    //if(cm_R<5 || cm_L>=8 && cm_L<d_enc){
    if(cm_L>4){
        alaizquierda();
    }
}

void avanza_falso(){
    ineI_falso=1800;
    motori_D.setSpeed(ade_ordi); 
    motori_D.run(FORWARD);       
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
}


// avanza por la rampa a maxima potencia 
void rampa(){
    while (inclina==0){
        motori_D.setSpeed(255);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(255);  
        motord_D.run(FORWARD); 
        delay(500);   
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
    ineI_falso=0;
    // avanza un tiempo para centrar la vuelta
    alto();
    while ((SII!=0) && (SID!=0)){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);  
        lee();
    }
    alto();
    //delay(ineD); 
    // inicia el giro a la derecha para separar el sensor izquierdo
    motori_D.setSpeed(ade_ordi);
    motori_D.run(FORWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(BACKWARD);
    delay(t_girod);
    //gira a la derecha hasta encontrar la pared izquierda
    ultra_L();
    while (cm_L>5) {
        motori_D.setSpeed(ade_ordi);
        motori_D.run(FORWARD); 
        motord_D.setSpeed(ade_ordd);
        motord_D.run(BACKWARD);
        ultra_L();
    }
    alto();
    delay(1000);
     // termina el giro a la derecha para separar el sensor izquierdo
    motori_D.setSpeed(ade_ordi);
    motori_D.run(FORWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(BACKWARD);
    delay(1200);
    // avanza para centrarse
    for (int cont_ciclo=0;cont_ciclo<=4;cont_ciclo++){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);        
        delay(esp_giro/4);
        lee();
        ver_delante();
    }
    alto();
    //delay(1000);
    // arranca de nuevo
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    ul_giro="DE";

}

// Gira a la derecha 90° sin pared de referencia 
void giro_derecha_falso(){
    ineI_falso=0;
    // avanza un tiempo para centrar la vuelta
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    delay(ineD); 
    // inicia el giro a la derecha para separar el sensor izquierdo
    motori_D.setSpeed(ade_ordi);
    motori_D.run(FORWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(BACKWARD);
    delay(t_girod);
    //gira a la derecha hasta encontrar la pared izquierda
    motori_D.setSpeed(ade_ordi);
    motori_D.run(FORWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(BACKWARD);
    delay(t_girod_flaso);
    alto();
    delay(1000);
     // termina el giro a la derecha para separar el sensor izquierdo
    motori_D.setSpeed(ade_ordi);
    motori_D.run(FORWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(BACKWARD);
    delay(1050);
    // avanza para centrarse
    for (int cont_ciclo=0;cont_ciclo<=4;cont_ciclo++){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);        
        delay(esp_giro/4);
        lee();
        ver_delante();
    }
    alto();
    //delay(1000);
    // arranca de nuevo
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    ul_giro="DE";

}


// Gira a la izquierda 90°
void giro_izquierda(){
    ineI_falso=0;
    // avanza un tiempo para centrar la vuelta
    //alto();
    while ((SII!=0) && (SID!=0)){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);  
        lee();
    }
    alto();
    // inicia el giro a la izquierda para separar el sensor derecho
    motori_D.setSpeed(ade_ordi);
    motori_D.run(BACKWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(FORWARD);
    delay(t_giroi);
    //Gira Izquierda hasta encontrar pared derecha
    ultra_R();
    while (cm_R>5) {
        motori_D.setSpeed(ade_ordi);
        motori_D.run(BACKWARD); 
        motord_D.setSpeed(ade_ordd);
        motord_D.run(FORWARD);
        ultra_R();
    }
    alto();
    delay(1000);
    // termina el giro a la izquierda para separar el sensor derecho
    motori_D.setSpeed(ade_ordi);
    motori_D.run(BACKWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(FORWARD);
    delay(800);
    // avanza para centrarse
    for (int cont_ciclo=0;cont_ciclo<=4;cont_ciclo++){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);        
        delay(esp_giro/4);
        lee();
        ver_delante();
    }
    alto();
    //delay(1000);
    // arranca
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    ul_giro="IZ";
 
}

// Gira a la izquierda 90°, sin pared de referencia 
void giro_izquierda_falso(){
    // avanza un tiempo para centrar la vuelta
    
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    delay(ineI_falso); //avanza para centrar el giro
    // inicia el giro a la izquierda para separar el sensor derecho
    motori_D.setSpeed(ade_ordi);
    motori_D.run(BACKWARD); 
    motord_D.setSpeed(ade_ordd);
    motord_D.run(FORWARD);
    delay(t_giroi_flaso);
    // avanza para centrarse
    for (int cont_ciclo=0;cont_ciclo<=4;cont_ciclo++){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);        
        delay(esp_giro/4);
        lee();
        ver_delante();
    }
    alto();
    // arranca
    motori_D.setSpeed(ade_ordi);  
    motori_D.run(FORWARD);        
    motord_D.setSpeed(ade_ordd);  
    motord_D.run(FORWARD);        
    ul_giro="IZ";
    ineI_falso=0;
}

          
// Gira a la derecha 180° giro en U
void giro_u(){
       ultra_R();
       ultra_L();
       //Detremina a donde girar (derecha o izquierda)
       if (cm_R>cm_L) {
            // pegado a la izquierda debe girar a la derecha
            // inicia el giro a la derecha para separar el sensor izquierdo
            motori_D.setSpeed(ade_ordi);
            motori_D.run(FORWARD); 
            motord_D.setSpeed(ade_ordd);
            motord_D.run(BACKWARD);
            delay(t_girod);
            //gira a la derecha hasta encontrar la pared izquierda
            ultra_R();
            ultra_L();
            while (cm_L>5 || cm_R >5 ) {
                motori_D.setSpeed(ade_ordi);
                motori_D.run(FORWARD); 
                motord_D.setSpeed(ade_ordd);
                motord_D.run(BACKWARD);
                ultra_R();
                ultra_L();
            }
             motori_D.setSpeed(ade_ordi);
             motori_D.run(FORWARD); 
             motord_D.setSpeed(ade_ordd);
             motord_D.run(BACKWARD);
             delay(700);
       }else {
            // pegado a la derecha debe girar a la izquierda
            // inicia el giro a la izquierda para separar el sensor derecho
            motori_D.setSpeed(ade_ordi);
            motori_D.run(BACKWARD); 
            motord_D.setSpeed(ade_ordd);
            motord_D.run(FORWARD);
            delay(t_giroi);
            //Gira Izquierda hasta encontrar pared derecha
            ultra_R();
            ultra_L();
            while (cm_L>5 || cm_R >5 ) {
                motori_D.setSpeed(ade_ordi);
                motori_D.run(BACKWARD); 
                motord_D.setSpeed(ade_ordd);
                motord_D.run(FORWARD);
                ultra_R();
                ultra_L();
            }
            motori_D.setSpeed(ade_ordi);
            motori_D.run(BACKWARD); 
            motord_D.setSpeed(ade_ordd);
            motord_D.run(FORWARD);
            delay(700);
       }
      alto();
      ul_giro="U";
      // avanza para centrarse
    for (int cont_ciclo=0;cont_ciclo<=4;cont_ciclo++){
        motori_D.setSpeed(ade_ordi);  
        motori_D.run(FORWARD);        
        motord_D.setSpeed(ade_ordd);  
        motord_D.run(FORWARD);        
        delay(espera_u/4);
        lee();
        ver_delante();
    }        
      
      
}



// Entrega kits de salvamento 
void entrega_kit(){
    alto();
    Serial.println("-------entrega kit----");
    digitalWrite(mot_kit,HIGH);
    kits.write(180);
    //ajustar tiempo para arrojar el kit de salvamwento
    delay(5000);
    kits.write(90);
    // tiempo de led de victima encontrada
    delay(3000);
    digitalWrite(mot_kit,LOW);
    //avanza();
    //delay(1500);
    //delay(5000);
}

void giro_u_falso(){
       ultra_R();
       ultra_L();
       //Detremina a donde girar (derecha o izquierda)
       if (cm_R>cm_L) {
            // pegado a la izquierda debe girar a la derecha
            motori_D.setSpeed(ade_ordi);
            motori_D.run(FORWARD); 
            motord_D.setSpeed(ade_ordd);
            motord_D.run(BACKWARD);
            delay(6000);
           
       }else {
            // pegado a la derecha debe girar a la izquierda
            motori_D.setSpeed(ade_ordi);
            motori_D.run(BACKWARD); 
            motord_D.setSpeed(ade_ordd);
            motord_D.run(FORWARD);
            delay(6000);
       }
      alto();
      ul_giro="U";
      // avanza para la iniercia
      motori_D.setSpeed(ade_ordi);  
      motori_D.run(FORWARD);        
      motord_D.setSpeed(ade_ordd);  
      motord_D.run(FORWARD);        
      delay(espera_u);
      
}

void negro(){
      v1=digitalRead(l1);
      v2=digitalRead(l2);
      v3=digitalRead(l3);
      if(v2==1 && v3==1){
          delay(150);
          v1=digitalRead(l1);
          v2=digitalRead(l2);
          v3=digitalRead(l3);
          if(v2==1 && v3==1){
               motori_D.setSpeed(ade_ordi);  
               motori_D.run(BACKWARD);        
               motord_D.setSpeed(ade_ordd);  
               motord_D.run(BACKWARD); 
               delay(ine_ng+1500);
               giro_u_falso();
             
          }
        }
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
    // Convierte el tiempo de recepción del eco en distancia:
    inches = microsecondsToInches(duration);
    cm_R = microsecondsToCentimeters(duration);
}
// Sensor izquierdo
void ultra_L(){
    digitalWrite(trigPin_L, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin_L, LOW);
    duration = pulseIn(echoPin_L, HIGH); //Devuelve la longitud del pulso del pin Echo en us
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
