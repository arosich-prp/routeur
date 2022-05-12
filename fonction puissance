int Puissance(){
  const unsigned long duree_mesure = 1000; // en ms
  int valmin = 1023;
  int valmax = 0;
  unsigned long chrono = millis();
  while (millis() - chrono < duree_mesure) {
    int val = analogRead( A1 );
    if (val < valmin) valmin = val;
    if (val > valmax) valmax = val;
  }
  //calcul du coefficiant à partir d'un étalonnage sur excel (ici, R=40 ohms et le tor à un rapport de 1000):
  return ((valmax - valmin)/2-511)*20;
}

