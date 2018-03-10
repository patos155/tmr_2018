//Incluimos la libreria SD.
#include <SD.h>
#include <QTRSensors.h>
#include <AFMotor.h>
#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_MLX90614.h>
/////////////////////////////////////////////////////////
  uint8_t _addr  = 0x5A;

#define MLX90614_I2CADDR 0x5A
// RAM
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08
// EEPROM
#define MLX90614_TOMAX 0x20
#define MLX90614_TOMIN 0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_EMISS 0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x0E
#define MLX90614_ID1 0x3C
#define MLX90614_ID2 0x3D
#define MLX90614_ID3 0x3E
#define MLX90614_ID4 0x3F
///////////////////////////////////////////////////////////

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SoftwareWire myWire(48,46);

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
       float temp1_2=0;
       float temp2_2=0;
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
   if(_begin()){
      Serial.println("Sensor iniciado bien prron");
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
   Serial.println("lectura de datos de temperatura");
   temp1=mlx.readAmbientTempC();
   temp2=mlx.readObjectTempC();
   Serial.println("Ambiente = ");
   Serial.print(temp1);
   Serial.println("  ºC\tObjeto = ");
   Serial.print(temp2);
   Serial.println(" ºC");
   // si el objeto tiene tres grados mas detecta victima
   dif_temp=temp2-temp1;
   Serial.println("------------------------------------------------------------------------------------"); 
   //----------------------------------------------------------------
    Serial.println("lectura de datos de temperatura");
   temp1_2=readTemp(0x07);
   temp2_2=readTemp(0x06);
   Serial.println("Ambiente 2 = ");
   Serial.print(temp1_2);
   Serial.println("  ºC\tObjeto 2 = ");
   Serial.print(temp2_2);
   Serial.println(" ºC");
   // si el objeto tiene tres grados mas detecta victima
   Serial.println("------------------------------------------------------------------------------------");

   
}


//////////////////////////////////////////////////////////////////////////////////////////


