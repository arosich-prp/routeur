void setup() {    
     Serial.begin(9600); 
 }

 void loop() {  
     int Uedf= safeAnalogRead(1);
     int Upv = safeAnalogRead(2);
     Serial.println(Uedf); 
     Serial.println(Upv);
 }
int safeAnalogRead(int pin)
{
  int x = analogRead(pin);  //make an initial reading to set up the ADC
  delay(9.9);                //let the ADC stabilize
  x = analogRead(pin);      //toss the first reading and take one we will keep
  delay(9.9);                //delay again to be friendly to future readings
  return x;
}
