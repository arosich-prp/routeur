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
  return sqrt((variation1*variation2)*(variation1*variation2))/(variation1*variation2);
}
