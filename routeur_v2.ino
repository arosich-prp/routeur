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
    }
    else if (signe<0){
      lcd.setCursor(0,0);
      lcd.print("consomme :        W");
    } 
    else {
      lcd.setCursor(0,0);
      lcd.print("Autoconsommation    ");
    }
    lcd.setCursor(0,2);
    lcd.print("Injection Cumulus:      W");         
    InjectionReseau();      
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
    //mesure entre 50 et 100 valeurs pour une période car environ 100µs de lecture par analogread et on a de 2 à 4 lectures: 
    int L1 = analogRead(A1);
    int L2 = analogread(A2);
    //calcul ddes tensions mesurées par les tors:
    Uedf=L1-511;
    Upv=L2-511;
    signe=Uedf*Upv; 
  }
           
