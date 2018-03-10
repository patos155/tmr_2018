#include <AFMotor.h>
#include <Servo.h>
Servo kits;

int servo_position=0;


void setup() {
  // put your setup code here, to run once:
  kits.attach(9);
}

void loop() {
  kits.write(0);
  delay(8000);
  kits.write(360);
  delay(1000);
  
}
