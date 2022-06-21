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
  int Pedf=0;
  int Ppv=0;
  int boucle=0;
          
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
   //affichage et lancement des fonctions de mesures et de réglage de puissance
   while (boucle<1800){
    if (signe>0){
      lcd.setCursor(0,0);
      lcd.print("EDF(inj)     PV");}
    else if (signe<0){
      lcd.setCursor(0,0);
      lcd.print("EDF(conso)   PV");}
    else {
      lcd.setCursor(0,0);
      lcd.print("EDF          PV");
    }
    lcd.setCursor(0,2);
    lcd.print("Injection Cumulus");         
    InjectionReseau();      
    InjectionCumulus();
    boucle+=1;
    delay(1000);
   }
   //reinitialisation du variateur de puissance
     Variateur.setState(OFF); 
     delay (1000);
     Variateur.setState(ON);
     boucle=0;
  }  
  
//Définition de la fonction pour régler le variateur de puissance:
  void InjectionCumulus(){
    //réglage du variateur et renvoie de la valeur:
      if (Pedf!=0){
        if ((signe>0)&(niveau<100)){niveau+=1;}
        else if((signe<0)&(niveau>0)){niveau-=1;}
        Variateur.setPower(niveau);
      }
        lcd.setCursor(0,3);
        lcd.print("   ");
        lcd.setCursor(0,3);
        lcd.print(niveau);
        lcd.setCursor(4,3);
        lcd.print("%");
    }
    
//Définition de la fonction des lectures et détermination de l'état d'injection ou de consammation
  void InjectionReseau(){    
    int k;
    int Uedf=0;
    int Upv=0;
    //mesure entre 50 et 100 valeurs pour une période car environ 100µs de lecture par analogread et on a de 2 à 4 lectures: 
      for(k=0; k < 100; k+=1) {
        int poubelle=analogread(A1);
        int L1 = analogRead(A1);
      if (L1>=Uedf){
          Uedf=L1;
        int poubelle=analogread(A2);
        int L2 = analogread(A2);
          Upv=L2;
        }             
       }
     //calcul de la tension mesurée par le tor (déphasage de 200µs de Upv):
        Pedf=Uedf-511;
        Ppv=Upv-511;
        signe=Pedf*Ppv;        
     //valeur retournée par la fonction:
        lcd.setCursor(0,1);
        lcd.print("        ");
        lcd.setCursor(0,1);
     //puissance d'injection ou de soutirage EDF
        lcd.print(Pedf*16);
        lcd.setCursor(6,1);
        lcd.print("Watts");
        lcd.setCursor(13,1);
        lcd.print("        ");
        lcd.setCursor(13,1);
      //puissance Photovoltaïque
        lcd.print(Ppv*16);     
  }
