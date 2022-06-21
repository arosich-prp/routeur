

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
    //initialisation des pins A1 et A2 qui mesurent la tension de sortie des tors (pince ampèremétrique):
      pinMode(A1,INPUT);
      pinMode(A2,INPUT);
  }

//Boucle principale:
  void loop(){
   //affichage et lancement des fonctions de mesures et de réglage de puissance
   while (boucle<300000){
    if (signe>0){
      lcd.setCursor(0,0);
      lcd.print("EDF(inj)     PV");}
    else if (signe<0){
      lcd.setCursor(0,0);
      lcd.print("EDF(conso)   PV");}
    else {
      lcd.setCursor(0,0);
      lcd.print("EDF          PV");}
    lcd.setCursor(0,2);
    lcd.print("Injection Cumulus");         
    InjectionReseau();      
    InjectionCumulus();
    boucle+=1;
   }
   //reinitialisation du variateur de puissance
     Variateur.setState(OFF); 
     delay (200);
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
    //mesure de 10000 valeurs en 1 secondes (sur 50 periodes avec 100 lectures par périodes pour chaque tor): 
      for(k=0; k < 5000; k+=1) {
        int L1 = analogRead(A1);
        int L2 = analogRead(A2);
        if (L2>=Upv){
          Uedf=L1;
          Upv=L2;
        }             
       }
     //calcul de la tension mesurée par le tor (déphasage de 100µs de Upv):
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

               
   
  
