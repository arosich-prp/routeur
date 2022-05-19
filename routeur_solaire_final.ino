

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
  const int ZeroCrossPin  = 2;
  float signe=1;
  int niveau=0;
  int U1=0;
  int U2=0;
         
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
    //initialisation des pins A1 et A2 qui mesurent la tension de sortie des tors (pince ampèremétrique):
      pinMode(A1,INPUT);
      pinMode(A2,INPUT);
  }

//Boucle principale:
  void loop(){    
   lcd.setCursor(0,0);
   lcd.print("Surplus PV");
   InjectionReseau();
   lcd.setCursor(0,2);
   lcd.print("consommation cumulus");
   InjectionCumulus();   
  }  
  
//Définition de la fonction pour régler le variateur de puissance:
  void InjectionCumulus(){
    //récupération du niveau antérieur:
      int niveau0=niveau;
    //réglage du variateur et renvoie de la valeur:
      if (signe<0){niveau+=1;}
      else if(signe>0){niveau-=1;}
      if (niveau<8){niveau=8;}
      else if (niveau>100){niveau=100;}
      if (niveau!=niveau0){    
      Variateur.setPower(niveau);
      }
      lcd.setCursor(0,3);
      lcd.print("   ");
      lcd.setCursor(0,3);
      lcd.print(niveau);
      lcd.setCursor(4,3);
      lcd.print("%");
    }

//Définition de la fonction pour lire la puissance:
  void InjectionReseau(){    
    int k;
    int U1max=0;
    //mesure sur un grand nombre de valeurs en 1 secondes (sur au moins 10 periodes donc 500 lectures par périodes sur chaque tor): 
      for(k=0; k < 5000; k+=1) {
        U1 = analogRead(A1);
        if (U1max<U1){
        U1max=U1;
        U2 = analogRead(A2);
        }          }                      
      }
      //calcul des Puissances:
        float P1=((U1max-524)*5.065)/1023;
        float P2=((U2-524)*5.065)/1023;
        signe=P1*P2;
      //valeur retournée par la fonction:
        lcd.setCursor(0,1);
        lcd.print("           ");
        lcd.setCursor(0,1);
        lcd.print(P1*3055.5);
        lcd.setCursor(8,1);
        lcd.print("Watts");
  }

               
   
  
