//variables
int Uedf=0;
int Upv=0;

void setup() {    
     //moniteur série
     Serial.begin(9600); 
 }

 void loop() {
      int S1=0;
      int S2=0;
      // recupération de 5 valeurs max de Uedf et de Upv correspondante pour calcul de la valeur moyenne
      for (int k=0, k<5, k+=1){
           //lecture de 20 valeurs sur 40 périodes et recherche du max pour EDF et recupération de la valeur PV correspondante
           for (int i=1, i<20, i++){
               int U1= safeAnalogRead(1);
               int U2 = safeAnalogRead(2);
               if (U1>=Uedf) {
                    Uedf=U1;
                    Upv=U2;
               }
                //déphasage de 1ms pou changer de position sur la sinusoïde
                delay(1);
           }
           // somme des valeurs de tension successives
           S1+=Uedf;
           S2+=Upv;
      }
      //calcul des valeurs moyennes
      Uedf=S1/5;
      Upv=S2/5;
      //affichage sur le port série
      Serial.println(Uedf); 
      Serial.println(Upv);
 }

//fonction de lecture plus sécurisée, elle prend environ 20 ms
int safeAnalogRead(int pin) {
     int x = analogRead(pin);  //première lecture pour initialiser l'entrée de l'ADC
     delay(9.89);              //délai de stabilisation du port
     x = analogRead(pin);      //lecture sur le port de l'ADC, environ 110µs=0,11ms
     delay(9.89)               // retour à 20ms
     return x;
}
