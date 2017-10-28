//Incluimos la libreria SD.
#include <SD.h>
#include <QTRSensors.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//define motores por cada puerto
AF_DCMotor motori_D(4); //motor izquierdo delantero
AF_DCMotor motori_T(1); //motor izquierdo trasero
AF_DCMotor motord_D(2);   //motor derecho
AF_DCMotor motord_T(3);   //motor izquierdo

//Declaracion de Variables para sensores
  // SD
      const int chipSelect = 44;
  //Ultrasonicos 
     //Delantero
       const int trigPin_D = 46;
       const int echoPin_D = 48;
     //Derecho
       const int trigPin_R = 24;
       const int echoPin_R = 27;
     //Izquierdo
       const int trigPin_L= 26; // Pin disparador. Se puede usar otro pin digital
       const int echoPin_L= 28; // Pin eco. Se puede usar otro pin digital
     //pines para sensor infrarojo (b/n)
       int izq_1=33;   //s1
       int izq_2=35;  //s2
       int izq_3=37; //s3
       int izq_centro=39;  //s4
       int der_centro=41;  //s5
       int der_6=43;  //s6
       int der_7=45;  //s7
       int der_8=47;  //s8
       int s_inclina=30;  // sensor de inclinación
   //variables de medicion de sensores
   //Sensores ultrasonicos 
       long duration;
       long inches;
       float cm_D; 
       float cm_R; 
       float cm_L; 
       float cm_A; 
       float dif_cm=0;
   // lectura de sensores
       int l1=0;
       int l2=0;
       int l3=0;
       int l4=0;
       int l5=0;
       int l6=0;
       int l7=0;
       int l8=0;
   //led de victima
       int vic=23;
       int inclina=1;
       int mot_kit=36;
       float dif_temp=0;
       float temp1=0;
       float temp2=0;
       //int vic=22;
       int lee_mot=40;
       int pos_mot=0;
       int pm=1;
       int para;
       int cuenta=0;


       
       
//Variables de control
    // distancia para encontrar las paredes (centimetros)
       int d_enc=30; 
       int d_fte=15;
    //encuentro con las paredes del laberinto (0) libre, (1) encuentra la pared 
       int adelante=0;
       int derecha=0;
       int izquierda=0;
       int atras=0;
    //velocidad de ruedas
       int ade_ordi=120;
       int medioi=75;
       int ade_ordd=120;
       int mediod=75;
       //long dif_cm=0;
       
    //Control de ultimo giro 
       String ul_giro="FR"; 
       String desvio="C";
    // espara para los giros
       int t_giro=2800;      //tiempo para los giros de 90°
       int t_u=4700;
       int ine=900;         //
       int esp_giro=2200;    //avanza despues de girar 180°
       //int t_giro_u=3500;   // tiempo para giros de 180°
       int tr=40;           // tiempo de retorno a la recta 
       int giro=0;  
       
       
       

void setup() {
   //Puerto serial 
   Serial.begin(9600); 
   mlx.begin(); 
   pinMode(vic,OUTPUT);
   pinMode(mot_kit,OUTPUT);
   pinMode(lee_mot,INPUT);

   
   
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

   
   // Sensores infrarojos
      pinMode(izq_1,INPUT);     //s1
      pinMode(izq_2,INPUT);    //s2
      pinMode(izq_3,INPUT);   //s3
      pinMode(izq_centro,INPUT);   //s4
      pinMode(der_centro,INPUT);   //s5
      pinMode(der_6,INPUT);   //s6
      pinMode(der_7,INPUT);   //s7
      pinMode(der_8,INPUT);    //s8
   // sensor de inclinación
      pinMode(s_inclina,INPUT);

   // El pin CS por defecto de la placa arduino debe ser configurado como salida
   // aunque no se use (10 en la mayoria de las placas, 53 en Arduino Mega).
      pinMode(53, OUTPUT);
   // Si ha habido error al leer la tarjeta informamos por el puerto serie.
      if (!SD.begin(chipSelect)){
         Serial.println("Error al leer la tarjeta.");
         return;
      }
}



 

void alto(){
   motori_D.setSpeed(0);//velocidad de motor izquierdo
   motori_D.run(FORWARD); //polaridad de motor izquierdo
   motord_D.setSpeed(0);//velocidad de motor derecho
   motord_D.run(FORWARD);//polaridad de motor  derecho
   motori_T.setSpeed(0);//velocidad de motor izquierdo
   motori_T.run(FORWARD); //polaridad de motor izquierdo
   motord_T.setSpeed(0);//velocidad de motor derecho
   motord_T.run(FORWARD);//polaridad de motor  derecho
   //Serial.println("____ALTO____________________");
   delay(100);
}
void regresa(){
   motori_D.setSpeed(100);//velocidad de motor izquierdo
   motori_D.run(BACKWARD); //polaridad de motor izquierdo
   motord_D.setSpeed(100);//velocidad de motor derecho
   motord_D.run(BACKWARD);//polaridad de motor  derecho
   motori_T.setSpeed(100);//velocidad de motor izquierdo
   motori_T.run(BACKWARD); //polaridad de motor izquierdo
   motord_T.setSpeed(100);//velocidad de motor derecho
   motord_T.run(BACKWARD);//polaridad de motor  derecho
   //Serial.println("____regresa____________________");
   delay(2000);
}


void loop() {
   //delay(2000);
   l4=digitalRead(izq_centro);
   l5=digitalRead(der_centro);
   
   Serial.println("____Sensores de Izq. a Derecha_____");
   Serial.print("l4- ");
   Serial.println(l4);
   Serial.print("l5- ");
   Serial.println(l5);
   // Abrimos el fichero donde vamos a guardar los datos (Si no existe se crea automaticamente).
   File dataFile = SD.open("valores.txt", FILE_WRITE);
   // ponemos a bajo el led de Victima
   digitalWrite(vic,LOW);
   digitalWrite(mot_kit,LOW);
   //lectura de datos de temperatura
   temp1=mlx.readAmbientTempC();
   temp2=mlx.readObjectTempC();
   dataFile.println();
   dataFile.print("Ambiente = ");
   dataFile.print(temp1);
   dataFile.print("  ºC\tObjeto = ");
   dataFile.print(temp2);
   dataFile.println(" ºC");
   // si el objeto tiene tres grados mas detecta victima
   dif_temp=temp2-temp1;
   //----------------------------------------------------------------
   if (dif_temp>12){

       alto();
       //ARROJA EL KIT DE RESCATE 
       pos_mot=1;
       cuenta=0;
       digitalWrite(mot_kit,HIGH);
       pm=digitalRead(lee_mot);
       if(pm==0){
          para=1;      
       }else {
          para=0;
       }
       Serial.println("----");
       Serial.println(pm);
       Serial.println(para);
       while ((pos_mot!=0) && (cuenta<1000)){
           pm=digitalRead(lee_mot);
           if (pm==para){
              pos_mot=0;
           }
           Serial.println(pm);
           cuenta++;
       }
       Serial.println(cuenta);
       digitalWrite(mot_kit,LOW);
       delay(200);
       digitalWrite(mot_kit,HIGH);
       pos_mot=1;
      cuenta=0;
       while ((pos_mot!=0) && (cuenta<1000)){
           pm=digitalRead(lee_mot);
           if (pm==0){
              pos_mot=0;
           }
           Serial.println(pm);
           cuenta++;
       }
       Serial.println(cuenta);
       digitalWrite(mot_kit,LOW);
       digitalWrite(vic,HIGH);
       delay(2400);
       digitalWrite(vic,LOW);  
       motori_D.setSpeed(ade_ordi); 
       motori_D.run(FORWARD);       
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       
       
   }
   // Lectura de sensores ultrasonicos 
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
   // sensor de inclinación
   inclina=digitalRead(s_inclina);

   if (dataFile){
      
      dataFile.print(millis());
      dataFile.print("    inc->");
      dataFile.print(inclina);
      dataFile.print(", ");

      dataFile.print("    cm_D->");
      dataFile.print(cm_D);
      dataFile.print("-");
      dataFile.print(adelante);
   
      dataFile.print("  cm_R->");
      dataFile.print(cm_R);
      dataFile.print("-");
      dataFile.print(derecha);
   
      dataFile.print("   cm_L->");
      dataFile.print(cm_L);
      dataFile.print("-");
      dataFile.print(izquierda);

      dataFile.print("   cm_A->");
      dataFile.print(cm_A);
      dataFile.print("-");
      dataFile.print(atras);

      
      dataFile.print("Ambiente = ");
      dataFile.print(temp1);
      dataFile.print("   ºC\tObjeto = ");
      dataFile.print(temp2);
      dataFile.println("  ºC");

   }



if (inclina==1){
//--------------------------------------------------------------------------------------------------------------------------------------
// si el robot no esta inclinado
//--------------------------------------------------------------------------------------------------------------------------------------
   
   // si encuentra negro
   if (l4==1 && l5==1){  
          dataFile.print("--------piso negro-------");
             alto();
             regresa();
             // GIRO HACIA LA DERECHA
             motori_D.setSpeed(ade_ordi);
             motori_D.run(FORWARD); 
             motord_D.setSpeed(ade_ordi);
             motord_D.run(BACKWARD);
             motori_T.setSpeed(ade_ordi);
             motori_T.run(FORWARD); 
             motord_T.setSpeed(ade_ordi);
             motord_T.run(BACKWARD);
             delay(t_u);
             alto();
             ul_giro="U";
             dataFile.print(t_u);
             dataFile.print("   ----giro U----(8)");
             motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
             motori_D.run(FORWARD);        //polaridad de motor izquierdo
                 motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
             motord_D.run(FORWARD);        //polaridad de motor  derecho
             motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
             motori_T.run(FORWARD);        //polaridad de motor izquierdo
             motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
             motord_T.run(FORWARD);        //polaridad de motor  derecho
             delay(600);
         }else {
   //Sin pared al frente y entre dos paredes avanza de frente alineandose a la IZQUIERDA
   //          |         |          |           |
   //          |    1    |          |     1
   //          |0       0|          |0         1
   //          |         |          |           |
   if (((adelante==1) && (izquierda==0) && (derecha==0)) || ((adelante==1) && (izquierda==0) && (derecha==1))){
       if ((cm_L>=4 && cm_L<=7)){
           motori_D.setSpeed(150); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(150);  
           motord_D.run(FORWARD);        
           motori_T.setSpeed(150);  
           motori_T.run(FORWARD);        
           motord_T.setSpeed(150);  
           motord_T.run(FORWARD);        
           desvio="C";
           dataFile.print("   ----avanza ---derecho -(1)");
        }
        //algoritmo para centrar
        if(cm_L<4){ 
           dataFile.print("   cm_L<7   ");
           // GIRA A LA DERECHA
           motori_D.setSpeed(ade_ordi);
           motori_D.run(FORWARD); 
           motord_D.setSpeed(ade_ordi);
           motord_D.run(BACKWARD);
           motori_T.setSpeed(ade_ordi);
           motori_T.run(FORWARD); 
           motord_T.setSpeed(ade_ordi);
           motord_T.run(BACKWARD);
           delay(300);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           motori_T.setSpeed(ade_ordi);  
           motori_T.run(FORWARD);        
           motord_T.setSpeed(ade_ordd);  
           motord_T.run(FORWARD);        
           delay(350);
           desvio="D";
           //izquierda
           motori_D.setSpeed(ade_ordi);
           motori_D.run(BACKWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(FORWARD);
           motori_T.setSpeed(ade_ordi);
           motori_T.run(BACKWARD); //polaridad de motor izquierdo
           motord_T.setSpeed(ade_ordd);
           motord_T.run(FORWARD);
           delay(300);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           motori_T.setSpeed(ade_ordi);  
           motori_T.run(FORWARD);        
           motord_T.setSpeed(ade_ordd);  
           motord_T.run(FORWARD);        
           delay(100);
           dataFile.print("   ----separa de la pared -(2)");
           //alto();
           //delay(500);
        }
        
        if(cm_L>7){
           dataFile.print("   cm_L>6");
           //GIRA IZQUIERDA
           motori_D.setSpeed(ade_ordi);
           motori_D.run(BACKWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(FORWARD);
           motori_T.setSpeed(ade_ordi);
           motori_T.run(BACKWARD); 
           motord_T.setSpeed(ade_ordd);
           motord_T.run(FORWARD);
           delay(350);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           motori_T.setSpeed(ade_ordi);  
           motori_T.run(FORWARD);        
           motord_T.setSpeed(ade_ordd);  
           motord_T.run(FORWARD);        
           delay(350);
           // GIRA A LA DERECHA
           motori_D.setSpeed(ade_ordi);
           motori_D.run(FORWARD); 
           motord_D.setSpeed(ade_ordi);
           motord_D.run(BACKWARD);
           motori_T.setSpeed(ade_ordi);
           motori_T.run(FORWARD); 
           motord_T.setSpeed(ade_ordi);
           motord_T.run(BACKWARD);
           delay(300);
           //avanza
           motori_D.setSpeed(ade_ordi); 
           motori_D.run(FORWARD);       
           motord_D.setSpeed(ade_ordd);  
           motord_D.run(FORWARD);        
           motori_T.setSpeed(ade_ordi);  
           motori_T.run(FORWARD);        
           motord_T.setSpeed(ade_ordd);  
           motord_T.run(FORWARD);        
           delay(100);
           dataFile.print("   ----acerca a la pared -(3)");
           //alto();
           //delay(500);
           desvio="I";
        }
        ul_giro="FT"+desvio;
   }
   
   //Pared al frente y a la izquierda gira a la derecha
   //          |---------|
   //          |    0    
   //          |0       1
   //          |         |
   if (((adelante==0)   && (izquierda==0) && (derecha==1))){
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       delay(ine); //avanza para centrar el giro
       //gira a la derecha
       motori_D.setSpeed(ade_ordi);
       motori_D.run(FORWARD); 
       motord_D.setSpeed(ade_ordd);
       motord_D.run(BACKWARD);
       motori_T.setSpeed(ade_ordi);
       motori_T.run(FORWARD); 
       motord_T.setSpeed(ade_ordd);
       motord_T.run(BACKWARD);
       delay(t_giro);
       // avanza para cenrarse
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       delay(esp_giro);
       alto();
       // arranca de nuevo
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       //alto();
       ul_giro="DE";
       dataFile.print("   ----gira derecha----(4)");
   }
   
   //Pared al frente y a la derecha gira a la izquierda 
   //          |---------|
   //               0    |
   //           1       0|
   //          |         |
   if ((adelante==0) && (izquierda==1) && (derecha==0)){
       
       // avanza un tiempo para centrar la vuelta
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       delay(ine); //avanza para centrar el giro
       // Gira Izquierda
       motori_D.setSpeed(ade_ordi);
       motori_D.run(BACKWARD); 
       motord_D.setSpeed(ade_ordd);
       motord_D.run(FORWARD);
       motori_T.setSpeed(ade_ordi);
       motori_T.run(BACKWARD); 
       motord_T.setSpeed(ade_ordd);
       motord_T.run(FORWARD);
       delay(t_giro);
       // se ceb¿ntra
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       delay(esp_giro);
       alto();
       // arranca
       motori_D.setSpeed(ade_ordi);  
       motori_D.run(FORWARD);        
       motord_D.setSpeed(ade_ordd);  
       motord_D.run(FORWARD);        
       motori_T.setSpeed(ade_ordi);  
       motori_T.run(FORWARD);        
       motord_T.setSpeed(ade_ordd);  
       motord_T.run(FORWARD);        
       //alto();
       ul_giro="IZ";
       dataFile.print("   ---- giro izquierda----(5)");
          
   }
   
   //Pared al frente y sin pared a la derecha  y a la izquierda gira a la izquierda 
   //          |---------|
   //               0    
   //           1       1
   //          |         |
   if ((adelante==0) && (izquierda==1) && (derecha==1)){
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       delay(ine); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_T.run(BACKWARD); //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_T.run(FORWARD);//polaridad de motor  derecho
       delay(t_giro);
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       delay(esp_giro);
       alto();
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       ul_giro="IZ";
       //alto();
       dataFile.print("   ---- giro izquierda----(6)");    
   }

   //Sin Pared al frente con pared a la derecha sin pared a la izquierda gira a la izquierda
   //          |         |
   //               1    |
   //           1       0|
   //          |         |
   if ((adelante==1) && (izquierda==1) && (derecha==0)){
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       delay(ine); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_T.run(BACKWARD); //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_T.run(FORWARD);//polaridad de motor  derecho
       delay(t_giro);
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       delay(esp_giro);
       alto();
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       ul_giro="IZ";
       //alto();
       dataFile.print("   ---- giro izquierda----(7)");    
   }

   //Sin Pared al frente Sin pared a la derecha y Sin pared a la izquierda gira a la izquierda
    //          
    //               1    
    //           1       1
    //          |         |
    if ((adelante==1) && (izquierda==1) && (derecha==1)){
       delay(ine); //avanza para centrar el giro
       motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_D.run(BACKWARD); //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_D.run(FORWARD);//polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);//velocidad de motor izquierdo
       motori_T.run(BACKWARD); //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);//velocidad de motor derecho
       motord_T.run(FORWARD);//polaridad de motor  derecho
       delay(t_giro);
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       delay(esp_giro);
       alto();
       motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_D.run(FORWARD);        //polaridad de motor izquierdo
       motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_D.run(FORWARD);        //polaridad de motor  derecho
       motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
       motori_T.run(FORWARD);        //polaridad de motor izquierdo
       motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
       motord_T.run(FORWARD);        //polaridad de motor  derecho
       ul_giro="IZ";
       //alto();    
       dataFile.print("   ---- giro izquierda----(9)");    
    }

   
   
    //Pared al frente pared a la derecha y pared a la izquierda regresa
    //          |---------|
    //          |    0    |
    //          |0       0|
    //          |         |
    if ((adelante==0) && (derecha==0) && (izquierda==0)){
         motori_D.setSpeed(ade_ordi);
         motori_D.run(FORWARD); 
         motord_D.setSpeed(ade_ordi);
         motord_D.run(BACKWARD);
         motori_T.setSpeed(ade_ordi);
         motori_T.run(FORWARD); 
         motord_T.setSpeed(ade_ordi);
         motord_T.run(BACKWARD);
        delay(t_u);
        alto();
        ul_giro="U";
        dataFile.print(t_u);
        dataFile.print("   ----giro U----(8)");
        motori_D.setSpeed(ade_ordi);  //velocidad de motor izquierdo
        motori_D.run(FORWARD);        //polaridad de motor izquierdo
        motord_D.setSpeed(ade_ordd);  //velocidad de motor derecho
        motord_D.run(FORWARD);        //polaridad de motor  derecho
        motori_T.setSpeed(ade_ordi);  //velocidad de motor izquierdo
        motori_T.run(FORWARD);        //polaridad de motor izquierdo
        motord_T.setSpeed(ade_ordd);  //velocidad de motor derecho
        motord_T.run(FORWARD);        //polaridad de motor  derecho
        if (l4==1 && l5==1){
            delay(600);
        }
    }
   }
}else {
   motori_D.setSpeed(ade_ordi); 
   motori_D.run(FORWARD);       
   motord_D.setSpeed(ade_ordd);  
   motord_D.run(FORWARD);        
   motori_T.setSpeed(ade_ordi);  
   motori_T.run(FORWARD);        
   motord_T.setSpeed(ade_ordd);  
   motord_T.run(FORWARD);        
   desvio="C";
   dataFile.print("   ----subida---");
}
      
dataFile.print("   ul_giro->");
dataFile.println(ul_giro);        
dataFile.close();
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
