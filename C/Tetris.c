#include <stdio.h>
#include <stdlib.h>

/*En las declaraciones de funciones no usé los parámetros de entrada porque siempre estaba cambiando las funciones*/

void nuevo();
void juego();
void cargar();
void pintar();
void cuadrado(); //pieza=1
void L(); //pieza=3
void L2(); //pieza=2
void L3(); //pieza=4
void L4(); //pieza=5
int lineallena();
void I(); //pieza=0
void I2(); // pieza=6
int lose();
void apano();
void cargar();
void guardar();
int main(){
  char mat[21][12];
  int menuprincipal,marcador=0,cont=1;
  int j=0,k=4,pieza=0,cargardo=0,dificultad=0;
	FILE *pf;
  srand (time(NULL));
	while (cont==1){
  printf("Pulse 1 para iniciar una nueva partida modo normal\nPulse 2 para cargar partida\nPulse 3 para salir\nPulse 4 para activar la musica\nPulse 5 para parar la musica\nPulse 6 para iniciar nueva partida modo dificil\nPulse 7 para iniciar nueva partida modo imposible\n");
  scanf ("%i",&menuprincipal); //Escogemos en el menú prinicpal
  switch (menuprincipal){
  case 1: nuevo(mat,dificultad);  //Iniciamos nuevo juego
    pintar(mat,&marcador);
    printf("\n\n\n");
     juego(mat,j,k,pieza,cargardo);
    break;
  case 2:pf=fopen("mipartida.txt","r"); //Comprueba si hay partida guardada
		if (pf==NULL){ //bueno, la comprobación real es aquí
	printf("No hay partida guardada\n\n");
	break;} 
	fclose(pf);
	cargar(mat);  //Cargamos anterior juego
    pintar(mat,&marcador);
    break;
  case 3: system("killall -q mpg123"); //Para la musica al salir del programa
	return 0;  //fin
    break;
case 4: system("killall -q mpg123"); //Para la musica antes de reproducirla si no ocurre un bucle
	system("mpg123 -q ./*.mp3 &"); //Empezar musica
	break;
case 5: system("killall -q mpg123"); //Terminar musica
	break;
  case 6: dificultad=1;
	nuevo(mat,dificultad);  //Iniciamos nuevo juego
    pintar(mat,&marcador);
    printf("\n\n\n");
     juego(mat,j,k,pieza,cargardo);
    break;
  case 7: dificultad=2;
	nuevo(mat,dificultad);  //Iniciamos nuevo juego
    pintar(mat,&marcador);
    printf("\n\n\n");
     juego(mat,j,k,pieza,cargardo);
    break;
  default: system("killall -q mpg123"); //para la musica al salir del programa 
	printf("Has pulsado otro boton por lo que salgo del programa\n");
return 0;    
break;
  }}
  return 0;}


void nuevo(char mat[][12],int dificultad){
  int j,k;
  //genero matriz
  for(j=0; j< 21; j++)
    for(k= 0; k< 12; k++)
      mat[j][k]= ' ';

  //paredes
  for(j= 0; j< 21; j++){
    mat[j][0]= '|';
    mat[j][11]= '|';
  }

  //suelo
  j=20;
  for (k=0;k<12;k++){
    mat[j][k]='=';}
if (dificultad==1){
for (j=10;j<20;j++){
	for (k=1;k<11;k++){
	mat[j][k]='$';}}}
if (dificultad==2){
for (j=5;j<20;j++){
	for (k=1;k<11;k++){
	mat[j][k]='$';}}}


  return; }



void pintar(char mat[][12],int *marcador){
  int j,k,b;
  for(j=0;j<21;j++){
    for(k=0;k<12;k++){
      if (mat[j][k]=='#'){printf("\E[45m %c\E[00m",' ');} //Para dibujar el cuadrado en color
	if (mat[j][k]=='%'){printf("\E[41m %c\E[00m",' ');} //Para dibujar el cuadrado en color
	if (mat[j][k]=='o'){printf("\E[44m %c\E[00m",' ');} //Para dibujar el cuadrado en color
	if (mat[j][k]=='$'){printf("\E[42m %c\E[00m",' ');} //Para dibujar el cuadrado en color
	if (mat[j][k]!='#' && mat[j][k]!='%' && mat[j][k]!='o' && mat[j][k]!='$'){
    printf(" %c",mat[j][k]);} //Para dibujar el resto de la matriz
    if (k==11){     //hacer salto de linea para seguir dibujando matriz
    printf("\n");
  }
    }}
    b = *marcador; //dibuja el marcador
printf("\n\n");
printf("          *****\n");
printf("          *   *\n");
printf("          *%2d *\n", b);
printf("          *   *\n");
printf("          *****");
printf("\n\n\n");
    
    
    
  return;}




void juego(char mat[][12],int j, int k ,int pieza,int cargardo){
     int menu,marcador;
     int tj,tk;
     char cont='s';
  int lineaabajo=0;
  int contador=0;
  int perder=0;
int llj=0,llk=0;
if (cargardo!=1){marcador=0;
                 pieza=rand()%3;
 if (pieza==2){pieza=3;}
 //----------------------------------------------------------------------
 //---------------------------------------------------------------------

  if (mat[j][k]==' ' && mat[j][k+1]==' ' && mat[j+1][k]==' ' && mat[j+1][k+1]==' ' && pieza ==1){ //comprueba si el espacio
     //de inserción  está vacio
     //para el cuadrado
    cuadrado(mat,j,k);
    pintar(mat,&marcador);}
 //----------------------------------------------------------------------
 //---------------------------------------------------------------------

    if (mat[j][k]==' ' && mat[j+1][k]==' ' && mat[j+2][k]==' ' && mat[j+2][k+1]== ' ' && pieza ==3){ //comprueba si el espacio
     //de inserción  está vacio
     //para la ele
    L(mat,j,k);
    pintar(mat,&marcador);}
 //----------------------------------------------------------------------
 //---------------------------------------------------------------------

    if (mat[j][k]==' ' && mat[j][k+1]==' ' && mat[j][k+2]==' ' && mat[j][k+3]==' ' && pieza ==0){ //comprueba si el espacio
     //de inserción  está vacio
     //para la I
    I(mat,j,k);
    pintar(mat,&marcador);}}


while (cont=='s' && perder==0){            
  printf("1 izquierda 2 derecha 3 para girar 4 abajo 44 para bajar del todo\n5 Save 6 The end\n10 para activar musica\n11 para detener musica\nPulse 50 si es ubaldo\n");
  scanf("%i",&menu);
  printf ("\n");

if (marcador>=5){ //Crea una linea imposible de eliminar
	llj=19; 
	for (llk=1;llk<11;llk++)
	mat[llj][llk]='$';
}

if (marcador>=10){ //Crea una linea imposible de eliminar
	llj=18; 
	for (llk=1;llk<11;llk++)
	mat[llj][llk]='$';
}
if (marcador>=15){ //Crea una linea imposible de eliminar
	llj=17; 
	for (llk=1;llk<11;llk++)
	mat[llj][llk]='$';
}
if (marcador>=17){ //Crea una linea imposible de eliminar
	llj=16; 
	for (llk=1;llk<11;llk++)
	mat[llj][llk]='$';
}

if (marcador>=20){ //Crea una linea imposible de eliminar
	llj=15; 
	for (llk=1;llk<11;llk++)
	mat[llj][llk]='$';
}
  

    switch (menu){ //Switch del menú principal
  case 1: system("clear all");
	if (mat[j][k-1]==' ' && mat[j+1][k-1]==' ' && pieza ==1){ //comprueba si el espacio
     //de la izquierda está vacio
     //para el cuadrado
    mat[j][k+1]=' '; //Borro el espacio sobrante
     mat[j+1][k+1]=' ';
     k-=1;      //Resto la coordenada del eje x
    cuadrado(mat,j,k);
    pintar(mat,&marcador);}

 //-------------------------------------------------------------------------
    //------------------------------------------------------------------------
    if (mat[j][k-1]==' ' && mat[j+1][k-1]==' ' && pieza==3){ //comprueba si el espacio
      // de la izquierda está vacio
      //para la ele 1
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';
      k-=1; //Resto la coordenada del eje x
      L(mat,j,k);
      pintar (mat,&marcador);}

 //-------------------------------------------------------------------------
    //------------------------------------------------------------------------
    if (mat[j+1][k-1]==' ' && mat[j][k+1]==' '  && pieza==4){ //comprueba si el espacio
      // de la izquierda está vacio
      //para la ele 3
      mat[j+1][k]=' ';//Borro el espacio sobrante
      mat[j+1][k+1]=' ';
      mat[j+1][k+2]=' ';
      mat[j][k+2]=' ';
      k-=1; //Resto la coordenada del eje x
      L3(mat,j,k);
      pintar (mat,&marcador);}

 //-------------------------------------------------------------------------
    //------------------------------------------------------------------------
    if (mat[j][k-1]==' ' && mat[j+1][k]==' ' && mat[j+2][k]==' ' && pieza==5){ //comprueba si el espacio
      // de la izquierda está vacio
      //para la ele 4
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j][k+1]=' ';
      mat[j+1][k+1]=' ';
      mat[j+2][k+1]=' ';
      k-=1; //Resto la coordenada del eje x
      L4(mat,j,k);
      pintar (mat,&marcador);}




    //-------------------------------------------------------------------------
    //------------------------------------------------------------------------
    if (mat[j][k-1]==' ' && mat[j+1][k-1]==' ' && mat[j+2][k-1] && pieza==2){ //comprueba si el espacio
      // de la izquierda está vacio
      //para la ele 2
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+2][k+1]=' ';
      k-=1; //Resto la coordenada del eje x
      L2(mat,j,k);
      pintar (mat,&marcador);}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 if (mat[j][k-1]==' ' && mat[j+1][k-1]==' ' && mat[j+2][k-1] && mat[j+3][k-1]==' ' && pieza==6){ //comprueba si el espacio
      // de la izquierda está vacio
      //para la I 2
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+3][k]=' ';
      k-=1; //Resto la coordenada del eje x
      I2(mat,j,k);
      pintar (mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 if (mat[j][k-1]==' ' && pieza==0){ //comprueba si el espacio
      // de la izquierda está vacio
      //para la I 
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';
      mat[j][k+3]=' ';
      k-=1; //Resto la coordenada del eje x
      I(mat,j,k);
      pintar (mat,&marcador);}





 break;

 case 2: system("clear all");
 if (mat[j][k+2]==' ' && mat[j+1][k+2]==' ' && pieza==1){ //comprueba si el espacio
//de la derecha está vacio
//para el cuadrado 
    mat[j][k]=' '; //Borro el espacio sobrante
    mat[j+1][k]=' ';
     k+=1;      //sumo la coordenada del eje x
    cuadrado(mat,j,k);
      pintar(mat,&marcador);}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------
    if (mat[j][k+3]==' ' && mat[j+1][k+1]==' ' && pieza==3){ //comprueba si el espacio
      // de la derecha está vacio
      //para la ele 1
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';
      k+=1; //Sumo la coordenada del eje x
      L(mat,j,k);
      pintar (mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------
    if (mat[j][k+3]==' ' && mat[j+1][k+3]==' ' && pieza==4){ //comprueba si el espacio
      // de la derecha está vacio
      //para la ele 3
      mat[j+1][k]=' ';//Borro el espacio sobrante
      mat[j+1][k+1]=' ';
      mat[j+1][k+2]=' ';
      mat[j][k+2]=' ';
      k+=1; //Sumo la coordenada del eje x
      L3(mat,j,k);
      pintar (mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------
    if (mat[j][k+2]==' ' && mat[j+1][k+2]==' ' && mat[j+2][k+2]==' ' && pieza==5){ //comprueba si el espacio
      // de la derecha está vacio
      //para la ele 4
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j][k+1]=' ';
      mat[j+1][k+1]=' ';
      mat[j+2][k+1]=' ';
      k+=1; //Sumo la coordenada del eje x
      L4(mat,j,k);
      pintar (mat,&marcador);}



//-------------------------------------------------------------------------
//------------------------------------------------------------------------
    if (mat[j][k+1]==' ' && mat[j+1][k+1]==' ' && mat[j+2][k+2]==' ' && pieza==2){ //comprueba si el espacio
      // de la derecha está vacio
      //para la ele 2
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+2][k+1]=' ';
      k+=1; //Sumo la coordenada del eje x
      L2(mat,j,k);
      pintar (mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 if (mat[j][k+4]==' ' &&  pieza==0){ //comprueba si el espacio
      // de la derecha está vacio
      //para la I 
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';
      mat[j][k+3]=' ';
      k+=1; //sumo la coordenada del eje x
      I(mat,j,k);
      pintar (mat,&marcador);}


//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 if (mat[j][k+1]==' ' && mat[j+1][k+1]==' ' && mat[j+2][k+1] && mat[j+3][k+1]==' ' && pieza==6){ //comprueba si el espacio
      // de la derecha está vacio
      //para la I 2
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+3][k]=' ';
      k+=1; //sumo la coordenada del eje x
      I2(mat,j,k);
      pintar (mat,&marcador);}

 break;

    case 3: system ("clear all"); //Función de girar, basicamente borro la anterior figura y cambio la figura
if (pieza==1){pintar(mat,&marcador); } 

      if (pieza==0 && mat[j+1][k]==' ' && mat[j+2][k]== ' ' && mat[j+3][k]==' '){pieza=6;
   mat[j][k]=' ';
   mat[j][k+1]=' ';
   mat[j][k+2]=' ';
   mat[j][k+3]=' ';
   I2(mat,j,k);
   pintar(mat,&marcador);
break;}
      
      if (pieza==6    && mat[j][k+1]==' ' && mat[j][k+2]== ' ' && mat[j][k+3]==' '){pieza=0;

 mat[j][k]=' ';
   mat[j+1][k]=' ';
   mat[j+2][k]=' ';
   mat[j+3][k]=' ';
   I(mat,j,k);
   pintar(mat,&marcador);
break;  
}
      
      if (pieza==3 && mat[j+2][k]==' ' && mat[j+2][k+1]==' '){pieza=2;
	mat[j][k]=' ';
	mat[j+1][k]=' ';
	mat[j][k+1]=' ';
	mat[j][k+2]=' ';
	L2(mat,j,k);
	  pintar (mat,&marcador);
	break;}
      if (pieza==2 && mat[j+1][k+1]==' '&& mat[j+1][k+2]==' ' && mat[j][k+2]==' ') {pieza=4;
	mat[j][k]=' ';
	mat[j+1][k]=' ';
	mat[j+2][k]=' ';
	mat[j+2][k+1]=' ';
	L3(mat,j,k);
	  pintar (mat,&marcador);
	break;}



      if (pieza==4 && mat[j][k]==' ' && mat[j][k+1]==' ' && mat[j+2][k+1]==' '){pieza=5;
	mat[j+1][k]=' ';
	mat[j+1][k+1]=' ';
	mat[j+1][k+2]=' ';
	mat[j][k+2]=' ';
	L4(mat,j,k);
	  pintar (mat,&marcador);
	break;}

      if (pieza==5 && mat[j+1][k]==' ' && mat[j][k+2]==' ') {pieza=3;
	mat[j][k]=' ';
	mat[j][k+1]=' ';
	mat[j+1][k+1]=' ';
	mat[j+2][k+1]=' ';
	L(mat,j,k);
	  pintar (mat,&marcador);
	break;}



      break;





 case 4:    system("clear all");
	system("mpg123 -q ./FX/bit/bit.mp3 &"); //ejecuta el sonido de caer
      
          
       if (mat[j+2][k]==' ' && mat[j+2][k+1]==' ' && pieza==1){ //comprueba si el espacio
                  //de abajo está vacio para el cuadrado
    mat[j][k]=' '; //Borro el espacio sobrante
     mat[j][k+1]=' ';
     j+=1;      //sumo la coordenada del eje y
    cuadrado(mat,j,k);
      pintar(mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 if (mat[j+3][k]==' ' && mat[j+3][k+1]==' ' && pieza==2){ //comprueba si el espacio
                  //de abajo está vacio para la L2
mat[j][k]=' ';//Borro La figura
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+2][k+1]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L2(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 if (mat[j+2][k]==' ' && mat[j+1][k+1]==' ' && mat[j+1][k+2]==' ' && pieza==3){ //comprueba si el espacio
                  //de abajo está vacio para la L
   mat[j][k]=' ';//Borro La figura
      mat[j+1][k]=' ';
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 if (mat[j+2][k]==' ' && mat[j+2][k+1]==' ' && mat[j+2][k+2]==' ' && pieza==4){ //comprueba si el espacio
                  //de abajo está vacio para la L3
mat[j+1][k]=' ';//Borro La figura
      mat[j+1][k+1]=' ';
      mat[j+1][k+2]=' ';
      mat[j][k+2]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L3(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);}


//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 if (mat[j+3][k+1]==' ' && mat[j+1][k]==' ' && pieza==5){ //comprueba si el espacio
                  //de abajo está vacio para la L2
mat[j][k]=' ';//Borro La figura
      mat[j][k+1]=' ';
      mat[j+1][k+1]=' ';
      mat[j+2][k+1]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L4(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);}





//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 if (mat[j+1][k]==' ' && mat[j+1][k+1]==' ' && mat[j+1][k+2]==' ' && mat[j+1][k+3]==' ' && pieza==0){ //comprueba si el espacio
      // de debajo está vacio
      //para la I 
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';
      mat[j][k+3]=' ';
      j+=1; //sumo la coordenada del eje Y
      I(mat,j,k);
      pintar (mat,&marcador);}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 if (mat[j+4][k]==' '  && pieza==6){ //comprueba si el espacio
      // de debajo está vacio
      //para la I 
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+3][k]=' ';
      j+=1; //sumo la coordenada del eje Y
      I2(mat,j,k);
      pintar (mat,&marcador);}
      
      //-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+2][k]!=' ' || mat[j+2][k+1]!=' ') && pieza==1){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+1][k]!=' ' || mat[j+3][k+1]!=' ') && pieza==5){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+3][k]!=' ' || mat[j+3][k+1]!=' ') && pieza==2){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+2][k]!=' ' || mat[j+1][k+1]!=' ' || mat[j+1][k+2]!=' ') && pieza==3){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+2][k]!=' ' || mat[j+2][k+1]!=' ' || mat[j+2][k+2]!=' ') && pieza==4){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;
  }

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if (mat[j+4][k]!=' ' && pieza==6){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+1][k]!=' ' || mat[j+1][k+1]!=' ' || mat[j+1][k+2]!=' ' || mat[j+1][k+3]!= ' ') && pieza==0){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}
      
 marcador+= lineallena(mat,marcador);//Siempre comprueba si la linea se llena
  if (lineaabajo==1){j=0;  //Si la pieza llega abajo se regeneran las coordenadas
    k=4;
 perder=lose(mat,j,k); //Comprueba si se puede iniciar la partida arrib   
 pieza=(rand()%3);
if (pieza==2){pieza=3;}
if (pieza==1){cuadrado(mat,j,k);}
if (pieza==0){I(mat,j,k);}
if (pieza==3){L(mat,j,k);}
    lineaabajo=0;}
 pintar(mat,&marcador);
 break;
 case 5: guardar(mat,j,k,pieza,marcador);
 case 6: cont=' '; //Para salir
 break;
case 10: system("killall -q mpg123"); //Para la musica antes de reroducirla
	system("mpg123 -q ./*.mp3 &"); //Reproduce musica
break;
case 11: system("killall -q mpg123"); //Para la musica
break;
case 50: system("killall -q mpg123"); //Para la musica
	system("mpg123 -q ./ubaldo/*.mp3 &");
 case 51111: contador+=10; //truco
 pintar(mat,&contador);
 break;
 case 3672: mat[0][0]='0'; //truco
      mat[15][6]='0';
      mat[8][3]='0';
      pintar(mat,&contador);
      break;
case 123456789:  //truco
                for(tj=0;tj<20;tj++){
                                     mat[tj][0]='@';
	mat[tj][11]='@';}
                                     pintar(mat,&marcador);
                                     break;
case 8642: for (tj=0;tj<20;tj++){ //truco
             for(tk=1;tk<11;tk++){
                               mat[tj][tk]=' ';}
                               }
                               pintar(mat,&contador);
                               lineaabajo=1;
                               break; 
 case 44: system("clear all"); 
	system("mpg123 -q ./FX/bit/bit.mp3 &"); //Sonido de caer
 
        while (mat[j+2][k]==' ' && mat[j+2][k+1]==' ' && pieza==1){ //comprueba si el espacio
                  //de abajo está vacio para el cuadrado
	system("clear all");
    mat[j][k]=' '; //Borro el espacio sobrante
     mat[j][k+1]=' ';
     j+=1;      //sumo la coordenada del eje y
    cuadrado(mat,j,k);
      pintar(mat,&marcador);
      printf("\n");}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 while (mat[j+3][k]==' ' && mat[j+3][k+1]==' ' && pieza==2){ //comprueba si el espacio
                  //de abajo está vacio para la L2
system("clear all");
mat[j][k]=' ';//Borro La figura
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+2][k+1]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L2(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);
      printf("\n");}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 while (mat[j+2][k]==' ' && mat[j+1][k+1]==' ' && mat[j+1][k+2]==' ' && pieza==3){ //comprueba si el espacio
                  //de abajo está vacio para la L
system("clear all");
   mat[j][k]=' ';//Borro La figura
      mat[j+1][k]=' ';
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);
      printf("\n");}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 while (mat[j+2][k]==' ' && mat[j+2][k+1]==' ' && mat[j+2][k+2]==' ' && pieza==4){ //comprueba si el espacio
                  //de abajo está vacio para la L3
system("clear all");
mat[j+1][k]=' ';//Borro La figura
      mat[j+1][k+1]=' ';
      mat[j+1][k+2]=' ';
      mat[j][k+2]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L3(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);
      printf("\n");}


//-------------------------------------------------------------------------
//------------------------------------------------------------------------

 while (mat[j+3][k+1]==' ' && mat[j+1][k]==' ' && pieza==5){ //comprueba si el espacio
                  //de abajo está vacio para la L2
system("clear all");
mat[j][k]=' ';//Borro La figura
      mat[j][k+1]=' ';
      mat[j+1][k+1]=' ';
      mat[j+2][k+1]=' ';   
     j+=1;      //sumo la coordenada del eje y
     L4(mat,j,k); //la vuelvo a crear
      pintar(mat,&marcador);
      printf("\n");}





//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 while (mat[j+1][k]==' ' && mat[j+1][k+1]==' ' && mat[j+1][k+2]==' ' && mat[j+1][k+3]==' ' && pieza==0){ //comprueba si el espacio
      // de debajo está vacio
      //para la I 
system("clear all");
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j][k+1]=' ';
      mat[j][k+2]=' ';
      mat[j][k+3]=' ';
      j+=1; //sumo la coordenada del eje Y
      I(mat,j,k);
      pintar (mat,&marcador);
      printf("\n");}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------
 while (mat[j+4][k]==' '  && pieza==6){ //comprueba si el espacio
      // de debajo está vacio
      //para la I 
	system("clear all");
      mat[j][k]=' ';//Borro el espacio sobrante
      mat[j+1][k]=' ';
      mat[j+2][k]=' ';
      mat[j+3][k]=' ';
      j+=1; //sumo la coordenada del eje Y
      I2(mat,j,k);
      pintar (mat,&marcador);
      printf("\n");}
      
             //-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+2][k]!=' ' || mat[j+2][k+1]!=' ') && pieza==1){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+1][k]!=' ' || mat[j+3][k+1]!=' ') && pieza==5){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+3][k]!=' ' || mat[j+3][k+1]!=' ') && pieza==2){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+2][k]!=' ' || mat[j+1][k+1]!=' ' || mat[j+1][k+2]!=' ') && pieza==3){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+2][k]!=' ' || mat[j+2][k+1]!=' ' || mat[j+2][k+2]!=' ') && pieza==4){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;
}

//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if (mat[j+4][k]!=' ' && pieza==6){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------

   if ((mat[j+1][k]!=' ' || mat[j+1][k+1]!=' ' || mat[j+1][k+2]!=' ' || mat[j+1][k+3]!= ' ') && pieza==0){  //Si hay algo debajo la variable se vuelve 1 
  lineaabajo=1;}
 


      
      
      
      marcador+= lineallena(mat,marcador);//Siempre comprueba si la linea se llena
       if (lineaabajo==1){j=0;  //Si la pieza llega abajo se regeneran las coordenadas
    k=4;
 perder=lose(mat,j,k); //Comprueba si se puede iniciar la partida arrib   
 pieza=(rand()%3);
if (pieza==2){pieza=3;}
if (pieza==1){cuadrado(mat,j,k);}
if (pieza==0){I(mat,j,k);}
if (pieza==3){L(mat,j,k);}
    lineaabajo=0;}
 pintar(mat,&marcador);
 break;
  }
  }

system("mpg123 -q ./FX/perder/perder.mp3 &"); //Sonido al perder


//SI pierdes sale todo el array lleno
 //genero matriz
  for(j=0; j< 21; j++)
    for(k= 0; k< 12; k++)
      mat[j][k]= '@';

  //paredes
  for(j= 0; j< 21; j++){
    mat[j][0]= '|';
    mat[j][11]= '|';
  }

  //suelo
  k=0;
  for (j=20;k<12;k++){
    mat[j][k]='=';}
  pintar(mat,&marcador);
  printf("HAS PERDIDO\n");

  return;}





void cuadrado(char mat[][12],int j,int k){ //funcion para hacer el cuadrado
  int max1=j,max2=k;
  for (j=max1;j<(max1+2);j++){
    for (k=max2;k<(max2+2);k++){
      mat[j][k]='o';}}
return;
}


void L(char mat[][12],int j,int k){ //Función para hacer la L
  mat[j][k]='#';
  mat[j+1][k]='#';
  mat[j][k+1]='#';
  mat[j][k+2]='#';
  return; }





void L2(char mat[][12],int j,int k){ //Función para hacer la L
  mat[j][k]='#';
  mat[j+1][k]='#';
  mat[j+2][k]='#';
  mat[j+2][k+1]='#';
  return; }


void L3(char mat[][12],int j,int k){ //Función para hacer la L
  mat[j+1][k]='#';
  mat[j+1][k+1]='#';
  mat[j+1][k+2]='#';
  mat[j][k+2]='#';
  return; }

void L4(char mat[][12],int j,int k){ //Función para hacer la L
  mat[j][k]='#';
  mat[j+1][k+1]='#';
  mat[j+2][k+1]='#';
  mat[j][k+1]='#';
  return; }


void I(char mat[][12],int j,int k){ //función para hacer la I
  mat[j][k]='%';
 mat[j][k+1]='%';
 mat[j][k+2]='%';
 mat[j][k+3]='%';
 return;}




  
void I2(char mat[][12],int j,int k){ //función para hacer la I
  mat[j][k]='%';
 mat[j+1][k]='%';
 mat[j+2][k]='%';
 mat[j+3][k]='%';
 return;}

int lineallena(char mat[][12], int contador){
  int j, k,cut;
  contador=0;
  for (j=0;j<20;j++){ //Recorro todas las lineas
  if (mat[j][0]!=' '  && mat[j][1]!=' ' && mat[j][2]!=' ' && mat[j][3]!=' ' && mat[j][4]!=' ' && mat[j][5]!=' ' && mat[j][6]!=' ' && mat[j][7]!=' ' && mat[j][8]!=' ' && mat[j][9]!=' ' && mat[j][10]!=' ' && mat[j][11]!=' ' &&
	mat[j][0]!='$'  && mat[j][1]!='$' && mat[j][2]!='$' && mat[j][3]!='$' && mat[j][4]!='$' && mat[j][5]!='$' && mat[j][6]!='$' && 
mat[j][7]!='$' && mat[j][8]!='$' && mat[j][9]!='$' && mat[j][10]!='$' && mat[j][11]!='$'){ //comprueba la linea designada con J
    cut=j;
system("mpg123 -q ./FX/Completalinea/Completalinea.mp3 &"); //Sonido al completar linea
    for (k=1;k<11;k++){ //vacia la ultima linea entre las paredes
      mat[j][k]=' ';}// sigue la instruccion
  contador+=1;
  apano(mat,cut);//baja las lineas
  contador+=lineallena(mat,contador); //vuelve a comprobar si hay otra linea llena
 }
  }
  return contador;
}


void apano(char mat[][12],int cut){ //Es una función la cual baja el resto de lineas al completar una
     int j,k;
     for (j=cut;j>0 && j<=cut;j--){
         for (k=0;k<12;k++){
             mat[j][k]=mat[j-1][k];
             mat[j-1][k]=' ';}}
             mat[0][0]='|';
             mat[0][11]='|';
             mat[1][0]='|';
             mat[20][0]='=';
             return;}
     
     

int lose(char mat[][12],int j,int k){ //Función de si se pierde
  int contador=0;
  int pieza;
  pieza=1;
  if ((mat[j][k]!=' ' || mat[j][k+1]!=' ' || mat[j+1][k]!=' '|| mat[j+1][k+1]!=' ') && pieza ==1){ //comprueba si el espacio
     //de inserción  está vacio
     //para el cuadrado
    contador=1;}
 //----------------------------------------------------------------------
 //---------------------------------------------------------------------

  pieza=2;

  if ((mat[j][k]!=' ' || mat[j+1][k]!=' ' || mat[j+2][k]!=' '|| mat[j+2][k+1]!= ' ') && pieza ==2){ //comprueba si el espacio
     //de inserción  está vacio
     //para la ele
   
   contador=1;}
 //----------------------------------------------------------------------
 //---------------------------------------------------------------------
    pieza=0;
    if ((mat[j][k]!=' ' || mat[j][k+1]!=' ' || mat[j][k+2]!=' ' || mat[j][k+3]!=' ') && pieza ==0){ //comprueba si el espacio
     //de inserción  está vacio
     //para la I
    
    contador=1;}

    return contador;}
    
    
    void guardar(char mat[21][12], int j, int k, int pieza,int marcador){ //Funcion guardar
         //-----------------------------------------------------------------------------------------
         int fil,col;
         FILE *pf;
         //-----------------------------------------------------------------------------------------
         pf=fopen("mipartida.txt","w"); //SIEMPRE SE VA A CREAR
         for(fil=0;fil<21;fil++){
                                  for (col=0;col<12;col++)
                                  {
                                      fprintf(pf,"%c",mat[fil][col]);
                                      
                                      }
                                      }
                                      fprintf(pf,"%i ",j);
                                      fprintf(pf,"%i ",k);
                                      fprintf(pf,"%i ",pieza);
                                      fprintf(pf,"%i ",marcador);
                                      fclose(pf);
                                      return;
                                      }
                                      
                                      
  void cargar(char mat[21][12]){ //Funcion para cargar
       //-----------------------------
       int fil,col,j=0,k=4,pieza,cargardo=1,marcador=0;
       FILE *pf;
       
       //-----------------------------
      
	pf=fopen("mipartida.txt","r");
       for(fil=0;fil<21;fil++)
       {
                               for(col=0;col<12;col++){
                                                        fscanf(pf,"%c",&mat[fil][col]);
                                                        }
                                                        }
                                                        fscanf(pf,"%d",&j);
                                                        fscanf(pf,"%d",&k);
                                                        fscanf(pf,"%d",&pieza);
                                                        fscanf(pf,"%d",&marcador);
                                                        fclose(pf);
                                                        pintar(mat,&marcador);
                                                        printf("Tus coordenadas son %i %i y tu pieza es %i\n",j,k,pieza);
                                                        juego(mat,j,k,pieza,cargardo);
                                                        
                                                        return;}
