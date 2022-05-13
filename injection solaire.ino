//--------PROGRAMME POUR envoyer le surplus de production PV au chauffe eau ou autre---------------------------------------

//Librairies: 
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

//Objets ou instances:
  //attribution du port de commande (3) au variateur:
    dimmerLamp Variateur(PortVariateur);
  // Creation de l'instance Injection:
    EnergyMonitor Injection;
  //Attribution de l'adresse matérielle et definition du lcd 20x4 (en cas de pb, changer le port 0x27 par 0x3F):
    LiquidCrystal_I2C lcd(0x27, 20, 4);

//Initialisations:
  void setup(){
    // initialisation de l'afficheur:
      lcd.init();
    //initialisation du variateur de puissance:
      Variateur.begin(NORMAL_MODE, ON);
    //lecture sur le pin A2 de la tension aux bornes de la résistance sur le tor (pince ampère métrique):
      pinMode(A1,INPUT);
      pinMode(A2,INPUT);
  }

//Boucle principale:
  void loop(){
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Surplus PV injection EDF");
    lcd.setCursor(0,1);
    lcd.print("        ");
    lcd.setCursor(0,1);
    lcd.print(Puissance());
    lcd.setCursor(0, 2);
    lcd.print("Injection Cumulus");
    lcd.setCursor(0, 3);
    lcd.print(Variateur());
  }  
  
//réglage du variateur et renvoie de la valeur:
  int Puissance(){
    if (
    {
      Variateur.setPower(  ); 
      //sortie sur le moniteur série du PC:
     
      Variateur.getPower();
       
     
    }
  }
