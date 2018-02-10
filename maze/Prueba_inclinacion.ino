int SI=53;
int inclina=1;

void setup() {
 Serial.begin(9600);
 pinMode(SI,INPUT);

}

void loop() {
inclina=digitalRead(SI);

Serial.println(inclina);  

if(inclina==0){
  Serial.println("Inclinado");  
}
else{
  Serial.println("Horizontal");  
}

delay(2000);

}
