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
    lcd.print("          Watts");
    lcd.setCursor(0,1);
    lcd.print(Puissance());    
    lcd.setCursor(0, 2);
    lcd.print("Injection Cumulus");
    lcd.setCursor(0, 3);
    lcd.print(Variateur());
    lcd.Print(" %");
  }  
  
//réglage du variateur et renvoie de la valeur:
int Variateur(){
  int niveau=0;
  int injection=invertion();  
  if (injection<0){
    niveau+=2;
  }
  else {
    niveau-=2;
  }
  if (niveau<0){
    niveau=0;
  }
  elif (niveau>100){
    niveau=100;
  }
  Variateur.setPower(niveau);
  return Variateur.getPower();
}

//écoute de la puissance injectée sur le réseau edf pendant 2 secondes (analogRead prend environ 100µs):      
int Puissance(){
  int lecture=0;
  int k;
  for (k=0;k<20000;k+=1){
    int l=analogRead(A1);
    if (l>lecture){
      lecture=l;
    }
  //calcul de la puissance à partir d'un étalonnage sur excel (ici, R=40 ohms et le tor à un rapport de 1000):
  return (lecture-511)*20;
}

  //ecoute pour voir s'il y a du courant injecté sur le réseau:
  int inversion(){
    int courant1[3];
    int courant2[3];
    while(signe1<0 || signe2<0){
      for (i=0;i<3;i++){
        courant1[i]=analogread(A1);
        courant2[i]=analogRead(A2);
        }
      int variation1=(courant1[1]-courant1[0]);
      int variation2=(courant2[1]-courant2[0]);
      int signe1=(courant1[1]-courant1[0])*(courant1[2]-courant1[1]);
      int signe2=(courant2[1]-courant2[0])*(courant2[2]-courant2[1]);
    }
  return (variation1*variation2);
}
