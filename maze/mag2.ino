#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"

HMC5883L magnetometro;

int16_t mx, my, mz;
float declinacion=-1.26; //declinación de -1°16'(Trujillo-Perú)
void setup() {
    
    Serial.begin(9600);
    
    Serial.println("Inicializando Magnetometro...");
    //Inicializamos la comunicación I2C y el magnetómetro
    Wire.begin();
    magnetometro.initialize();
}

void loop() {
    //Obtenemos del magnetómetro las componentes del campo magnético
    magnetometro.getHeading(&mx, &my, &mz);

    //Calculamos el ángulo del eje X con respecto al norte
    float angulo = atan2(my, mx);
    angulo=angulo*(180/M_PI);//convertimos de Radianes a grados
    angulo=angulo-declinacion; //corregimos la declinación magnética
    //Mostramos el angulo entre el eje X y el Norte
    Serial.print("AnguloX-N: ");
    Serial.print(angulo,0);

    //calculamos el ángulo equivalente de [-180 180] a [0 360]
    if(angulo<0) angulo=angulo+360;
    Serial.print("\tN");
    Serial.println(angulo,0);  
    delay(5000);
}
