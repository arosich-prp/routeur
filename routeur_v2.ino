//--------envoyer le surplus de production PV au chauffe eau ou autre---------------------------------------

//Librairies:
  //Variateur est le nom pour le module AC dimmer qui est le variateur de puissance:
    //on peut télécharger la librairie zippée sur https://github.com/RobotDynOfficial/RBDDimmer:
      #include <RBDdimmer.h>
  //pour communique en mode I2C qui est le protocol utilisé par le support lcd:
    #include <Wire.h>   
  //pour utiliser le LCD:
    #include <LiquidCrystal_I2C.h>
    
//variables:
  int signe=0;
  int niveau=0;
  int Uedf=0;
  int Upv=0;
  int Pcumulus=0;
  int temps=0;
          
//Objets ou instances:
  //attribution du port de commande (3) au variateur:
    dimmerLamp Variateur(3);
  //Attribution de l'adresse matérielle et definition du lcd 20x4:
    LiquidCrystal_I2C lcd(0x27,20,4);
    
//Initialisations:
  void setup(){
    // initialisation de l'afficheur:
      lcd.init();
      lcd.backlight();    
    //initialisation du variateur de puissance:
      Variateur.begin(NORMAL_MODE, ON);
    //pas d'initialisation des ports A1 et A2 avec analoread
  }

//Boucle principale:
  void loop(){
   //affichage et lancement des fonctions de mesures et de réglage de puissance:
    if (signe>0){
      lcd.setCursor(0,0);
      lcd.print("injecte  :        W");
      lcd.setCursor(11,0);
      lcd.print(Uedf);
    }
    else if ((signe<0)|(Upv=0)){
      lcd.setCursor(0,0);
      lcd.print("consomme :        W");
      lcd.setCursor(11,0);
      lcd.print(Uedf);
    } 
    else if (Uedf=0){
      lcd.setCursor(0,0);
      lcd.print("Autoconsommation    ");
    }
    lcd.setCursor(0,2);    
    lcd.print("Conso Cumulus:     W");
    lcd.setCursor(14,2);
    lcd.print(niveau*15);
    InjectionReseau();
    signe=Uedf*Upv;
    InjectionCumulus();   
    //reinitialisation du variateur de puissance au bout d'une demi-heure:
    if (temps>=18000){
      Variateur.setState(OFF);
      delay (1000);
      Variateur.setState(ON);
      Variateur.setPower(niveau);
      temps=0;
    }
    delay(1000);
    temps+=1;    
  }  
  
//Définition de la fonction pour régler le variateur de puissance:
  void InjectionCumulus(){
    //réglage du variateur et renvoie de la valeur de puissance injectée:
    While ((Upv>500)|(Upv<-500){
      if (Uedf!=0){
        if ((signe>0)&(niveau<100)){niveau+=1;}
        else if((signe<0)&(niveau>0)){niveau-=1;}
        Variateur.setPower(niveau);
      }
    }
    if ((Upv<=500)&(Upv>=-500){
      niveau=0;
    }
  }
    
 //Définition de la fonction qui donne l'état "injection ou de consommation
  void InjectionReseau(){    
    int S1=0;
    int S2=0;
    int Uedf=0;
    int Upv=0;
    // recupération de 5 valeurs max de Uedf et de Upv correspondante pour calcul de la valeur moyenne
      for (int k=0; k<5; k+=1){
           Uedf=0;
           Upv=0;
           //lecture de 20 valeurs sur 40 périodes et recherche du max pour EDF et recupération de la valeur PV correspondante
             for (int i=1; i<20; i++){
               int U1 = safeAnalogRead(1);
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
      Uedf=S1/5-511;
      Upv=S2/5-511;      
 }

//fonction de lecture plus sécurisée, elle prend environ 20 ms
 int safeAnalogRead(int pin) {
     int x = analogRead(pin);  //première lecture pour initialiser l'entrée de l'ADC
     delay(9.89);              //délai de stabilisation du port
     x = analogRead(pin);      //lecture sur le port de l'ADC, environ 110µs=0,11ms
     delay(9.89);               // retour à 20ms
     return x;
}
           
