//--------PROGRAMME POUR envoyer le surplus de production PV au chauffe eau ou autre---------------------------------------

//Librairies:
  // librairie pour mesurer l'intensité avec un tor (à tester au cas où mais ma fonction Injection à l'air de bien fonctionner):
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
      int Ppas  = 2;
    //autre variable:
      int Puissance  = 0;

//Objets ou instances:
  //attribution du port de commande (3) au variateur:
    dimmerLamp Variateur(PortVariateur);
  // Creation de l'instance Injection:
    EnergyMonitor emon1;
  //Attribution de l'adresse matérielle et definition du lcd 20x4 (en cas de pb, changer le port 0x27 par 0x3F):
    LiquidCrystal_I2C lcd(0x27, 20, 4);

//Initialisations:
  void setup(){
    //Création du port série pour que l'arduino puisse envoyer les infos vers le pc:
      Serial.begin(9600);
    //petit message pour visuliser le démmarage:
      Serial.println(F("Initialisation du système"));               
    // calibrage du courant sur le pin 1 (coéfficient 111.1 à changer eventuellemnt pour ajuster):
      emon1.current(1, 111.1);  
    // initialisation de l'afficheur:
      lcd.init();
    //initialisation du variateur de puissance:
      Variateur.begin(NORMAL_MODE, ON);
    //lecture sur le pin A2 de la tension aux bornes de la résistance sur le tor (pince ampère métrique):
      pinMode(A2,INPUT);
    //lecture sur le pin A1 de la tension aux bornes de la résistance sur le tor (pince ampère métrique):
  }

//Boucle principale:
  void loop(){
    //test des fonctions:
      //test du variateur de puissance:
        //testVariateur();
      // test du calcul d'intensité avec emonlib:
        //Iemon();
      //test d'acquisition sur A2:
        Lecture();
      //test de l'affichage sur lcd:
        //AffichageLcd();
  }  
  
//Définition de la fonction pour tester le variateur de puissance:
  void testVariateur(){
    //essai avec une lampe à incandescence branchée en sortie du module AC Dimmer:  
      // augmentation de la puissance de 2% par seconde:
        for(Puissance=Pmax;Puissance<=Pmin;Puissance+=Ppas){  
          Variateur.setPower(Puissance); 
          //sortie sur le moniteur série du PC: 
            Serial.print("lampValue -> ");
            Serial.print(Variateur.getPower());
            Serial.println("%");
          //1/2 seconde d'attente pour les affichages:
            delay(500); //1/2 seconde d'attente
        }
    // diminution de la puissance de 2% par seconde:
      for(Puissance=Pmax;Puissance>=Pmin;Puissance-=Ppas){
          Variateur.setPower(Puissance); 
          //sortie sur le moniteur série du PC:
            Serial.print("lampValue -> ");
            Serial.print(Variateur.getPower());
            Serial.println("%");
          //1 seconde d'attente pour les affichages:
            delay(1000); 
        }
    }

//Définition de la fonction pour lire la tension sur une période (20ms)
    void Lecture(){
      //Les valeurs lues sur A2 sont stockées dans un tableau:
    float Um;
    float Ueff;
    float Ieff;
    float P;
    int U[500];
      int i;
      Serial.println("Recherche de Umax et calcul de I et de P\n");
      //tempo une seconde, juste pour laisser demarrer avant de mesurer: 
        delay(1000);
        //mesure sur une période (on est à 50hz donc 20ms "environ" car le secteur pas si stable), 
        //le temps d'acquisition analogique étant d'environ 0,1ms, on va faire le maximum de lectures pour essayer de choper Umax:
        for(i=0; i < 500; i++) {
          U[i] = analogRead(A2);              
        }
      //recheche de Umax:
        Um=U[0];
        for(i=0; i < 500; i++) {
          if (Um<U[i]){
            Um=U[i];
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
    }

//Définition de la fonction etalonnage du calcul d'intensité:
    void Iemon(){
      //Calcul d'intensités:
        float I=emon1.Irms;
        double Ieff = emon1.calcIrms(1480);
      // affichage du test sur le PC pour bien étalonner les calculs d'intensités:
        Serial.print("intensité instantanée");
        Serial.print(I);
        Serial.println(" Ampères");
        Serial.print("intensité moyenne");
        Serial.print(Ieff);
        Serial.println(" Ampères");
     }

// définition de la fonction d'affichage sur le LCD (test):
  void AffichageLcd(){
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print(" essai pour Lcd");
      lcd.setCursor(0,1);
      lcd.print(" deuxième ligne");
      lcd.setCursor(0, 2);
      lcd.print(" 3eme ligne");
      lcd.setCursor(0, 3);
      lcd.print(" fin du LCD");           
    }
//définition de la fonction qui détermine s'il y a injection ou consommation sur le réseau EDF:
  void Injection(){
    int i;
    Inj1[3];
    Inj2[3];
    int signe1=1;
    int signe2=-1;
    While (signe1*signe2<0){
      for (i=0;i<3;i+=1){
        Inj1[i]=analogRead(A1);
        Inj1[i]=analogRead(A2);
      }
      signe1=(Inj1[2]-Inj1[1])*(Inj1[1]-Inj1[0]);
      signe2=(Inj2[2]-Inj2[1])*(Inj2[1]-Inj2[0]);
    }
    if (signe1>0){
      Serial.print("le courant sur A1 est croissant");
    }
    else {
      Serial.print("le courant sur A1 est décroissant");
    }
    if (signe2>0){
      Serial.print("le courant sur A2 est croissant");
    }
    else {
      Serial.print("le courant sur A2 est décroissant");
    }
  }
    
  
