

  #include <ArduinoMotorShieldR3.h>

ArduinoMotorShieldR3 md;

const int EchoPin = 53;
const int TriggerPin = 52;
 
void setup() {
   Serial.begin(9600);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);

   Serial.begin(9600); 
   Serial.println("Arduino Motor Shield R3"); 
   md.init(); 

}
 
void loop() {
   int cm = ping(TriggerPin, EchoPin);
   Serial.print("Distancia: ");
   Serial.println(cm);
   delay(1000);
   /*
   if (cm<10);{
    ///RETROCEDER
    md.setM1Speed (-90);
    md. setM2Speed (-90);
    delay (1000);
    //Quieto
    md.setM1Speed (0);
    md.setM2Speed (0);
    delay (500);
    //GIRAR A LA DERECHA
    md.setM1Speed (-150);
    md.setM2Speed (190);
    delay (500);
     //AVANZAR
    md.setM1Speed (100);
    md.setM2Speed (100);
    delay (2500);
    //GIRAR A LA IZQUIERDA
    md.setM1Speed (180);
    md.setM2Speed (-160);
    delay (500);
    //AVANZAR
    md.setM1Speed (100);
    md.setM2Speed (100);
    delay (3100);
    //GIRAR A LA IZQUIERDA
    md.setM1Speed (150);
    md.setM2Speed (-190);
    delay (500);
    //AVANZAR
    md.setM1Speed (100);
    md.setM2Speed (100);
    delay (1000);
   //SE INCORPORA A LA LINEA
    md.setM2Speed (100);
    md.setM2Speed (100);
    delay (500);
    //Quieto
    md.setM1Speed (0);
    md.setM2Speed (0);
    delay (500);
    }*/
}

 int ping(int TriggerPin, int EchoPin) {
   long duration, distanceCm;
   
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
   return distanceCm;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
