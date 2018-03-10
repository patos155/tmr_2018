#include <QTRSensors.h>

#define NUM_SENSORS   8     // Número de sensores
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low

QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8, 9, 10},
  NUM_SENSORS, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

void setup() {
  
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  qtrrc.read(sensorValues);
  unsigned char i;
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print("   ");
  }
  Serial.println(" ");
  delay(250);
}


