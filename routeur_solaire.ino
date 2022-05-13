//--------PROGRAMME POUR envoyer le surplus de production PV au chauffe eau ou autre---------------------------------------

//Librairies:
  // librairie pour mesurer l'intensité avec un tor:
    #include <EmonLib.h> 
  //Variateur est le nom pour le module AC dimmer qui est le variateur de puissance:
      //on peut télécharger la librairie zippée sur https://github.com/RobotDynOfficial/RBDDimmer:
        #include <RBDdimmer.h>
  //pour communique en mode I2C qui est le protocol utilisé par le support lcd:
    #include <Wire.h>
  //pour utiliser le LCD:
    #include <LiquidCrystal_I2C.h>
//variables:
  //variables du variateur:
    //Paramètres:
      const int ZeroCrossPin  = 2;
      const int PortVariateur  = 3;
    //bornes de pourcentage de puissance et pas d'incrémentation:
      int Pmin = 0;
      int Pmax  = 100;
      int Ppas  = 20;
    //autre variable:
      int Puissance  = 0;

//Objets ou instances:
  //attribution du port de commande (3) au variateur:
    dimmerLamp Variateur(PortVariateur);
  // Creation de l'instance Injection:
    EnergyMonitor emon1;
  //Attribution de l'adresse matérielle et definition du lcd 20x4 (en cas de pb, changer le port 0x27 par 0x3F):
    LiquidCrystal_I2C lcd(0x27,20,4);

//Initialisations:
  void setup(){
    //Création du port série pour que l'arduino puisse envoyer les infos vers le pc:
      Serial.begin(9600);
    //petit message pour visuliser le démmarage:
      Serial.println(F("Initialisation du système"));               
    // calibrage du courant sur le pin 1 (coéfficient 111.1 à changer):
      emon1.current(A1,24.7);  
    // initialisation de l'afficheur:
      lcd.init();
    //initialisation du variateur de puissance:
      Variateur.begin(NORMAL_MODE, ON);
    //lecture sur le pin A2 de la tension aux bornes de la résistance sur le tor (pince ampère métrique):
      pinMode(A1,INPUT);
  }

//Boucle principale:
  void loop(){
    //test des fonctions:
      //test du variateur de puissance:
        testVariateur();
      // test du calcul d'intensité avec emonlib:
        //Iemon();
      //test de d'acquisition et de calcul de Umax:
        Umax();
      //test de l'affichage sur lcd:
        AffichageLcd();
  }  
  
//Définition de la fonction pour tester le variateur de puissance:
  void testVariateur(){
    //essai avec une lampe à incandescence branchée en sortie du module AC Dimmer:  
      // augmentation de la puissance de 2% par seconde:
       
        if (Puissance<30){
          Puissance=50;  
        }
        else{
          Puissance=0;
        }
          Variateur.setPower(Puissance); 
          //sortie sur le moniteur série du PC: 
            Serial.print("lampValue -> ");
            Serial.print(Variateur.getPower());
            Serial.println("%");
          
    }

//Définition de la fonction pour lire la tension sur une période (20ms)
    void Umax(){
      //Les valeurs lues sur A2 sont stockées dans un tableau:
    float Um;
    float Ueff;
    float Ieff;
    float P;
    int U[50];
      int k;
    //mesure sur une période (on est à 50hz donc 20ms "environ" car le secteur pas si stable), le temps d'acquisition analogique étant d'environ 0,1ms, on va faire le maximum de lectures pour choper au lus proche Umax:
        for(k=0; k < 50; k++) {
          U[k] = analogRead(A2);              
        }
      //recheche de Umax:
        Um=U[0];
        for(k=0; k < 50; k++) {
          if (Um<U[k]){
            Um=U[k];
          }         
        }
      //calcul de Umax et de Uefficace et Ieff:
        Um=Um*5/1023-2.497556;
        Ueff=Um/1.414;
        Ieff=Ueff/38.5*1000;
        P=Ieff*230;
      //affichage et contrôle des résultats sur le moniteur série:
        Serial.println("Résultats :");
        
        Serial.println("");
        Serial.print("Umax= ");
        Serial.println(Um);
        Serial.print("U= ");
        Serial.println(Ueff);
        Serial.print("d'ou I= ");
        Serial.print(Ieff);
        Serial.print("A et P= ");
        Serial.print(P);
        Serial.println("W");
        delay(2000);
    }

//Définition de la fonction etalonnage du calcul d'intensité:
    void Iemon(){
      //Calcul d'intensités:
        double Iinjection = emon1.calcIrms(1480);
      // affichage du test sur le PC pour bien étalonner les calculs d'intensités:
        Serial.print(Iinjection); // intensité 
        Serial.println(" Ampères");
    }

// définition de la fonction d'affichage sur le LCD (test):
  void AffichageLcd(){
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print(" essai pour Lcd");
      lcd.setCursor(0,1);
      lcd.print(" deuxième ligne");
      lcd.setCursor(0,2);
      lcd.print(" 3eme ligne");
      lcd.setCursor(0,3);
      lcd.print(" fin du LCD");           
    }
  
