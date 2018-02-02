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

//Variables de control
    // espara para los giros
       int t_giroi=5000;      //tiempo para los giros de 90°
       int t_girod=4700;      //tiempo para los giros de 90°
       int t_u=12000;
       int ineD=2300;
       int ineI=2500;        
       int ine_ng=1500;       //inercia zona negra
       int esp_giro=4200;    //avanza despues de girar 180°
       //int t_giro_u=3500;   // tiempo para giros de 180°
       int tr=40;           // tiempo de retorno a la recta 
       int giro=0;  
       
    //velocidad de oruga
       int ade_ordi=140;
       int medioi=115;
        int ade_ordd=190;
       int mediod=150;
       
    // CONTROL
      int control=1;   

void setup() {
   //Puerto serial 
   Serial.begin(9600); 
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

//------------------------loop-----------------------------------------------------
void loop() {
       if (control==1){
           
           //gira a la derecha
           motori_D.setSpeed(ade_ordi);
           motori_D.run(FORWARD); 
           motord_D.setSpeed(ade_ordd);
           motord_D.run(BACKWARD);
           delay(t_girod);
           alto();
           delay(10000);
           //gira a la izquierda
           motori_D.setSpeed(ade_ordi);//velocidad de motor izquierdo
           motori_D.run(BACKWARD); //polaridad de motor izquierdo
           motord_D.setSpeed(ade_ordd);//velocidad de motor derecho
           motord_D.run(FORWARD);//polaridad de motor  derecho
           delay(t_giroi);
           alto();
           delay(10000);
           
           //giro en U
           motori_D.setSpeed(ade_ordi);
           motori_D.run(BACKWARD); 
           motord_D.setSpeed(ade_ordi);
           motord_D.run(FORWARD);
           delay(t_u);
           alto();
           delay(10000);
           control=0;
       }
}
   

