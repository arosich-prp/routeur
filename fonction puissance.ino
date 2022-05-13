int Puissance(){
  int lecture=0;
  int k;
  for (k=0;k<10000;k+=1){
    int l=analogRead(A1);
    if (l>lecture){
      lecture=l;
    }
  //calcul de la puissance à partir d'un étalonnage sur excel (ici, R=40 ohms et le tor à un rapport de 1000):
  return (lecture-511)*20;
}

