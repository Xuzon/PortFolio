#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <string.h>

typedef struct mensaje{
	long tipo;
	char texto[50];
} msg;

typedef struct proceso{
	pid_t ID;
	int nodo;
	char tipo;
} process;

void enviarToken(int pid,char tipo,int nodo,int nodoOrigen);
void enviarSolicitud(int pid,char tipoProceso, char tipo,int nodo);
int saberBuzones(int buzon);

int i;

void enviarToken(int pid,char tipo,int nodo,int nodoOrigen){
	msg mensajeTemp;
	mensajeTemp.tipo = 2;//digo que es de tipo 2
	char textoTemp[50];
	mensajeTemp.texto[0] = tipo;//meto el tipo en el puesto 0
	if(tipo != 'e')
		mensajeTemp.texto[1] = (char)(nodo+48);//pongo el nodo en el puesto 1
	else
		mensajeTemp.texto[1] = (char)(nodoOrigen+48);
	mensajeTemp.texto[2] = '\0';//pongo fin de cadena para concatenar
	sprintf(textoTemp, "%d", pid);//paso el pid a string
	char* stringCopia = strcat(mensajeTemp.texto,textoTemp); //concateno en variable auxiliar
	for(i = 0;i<50;i++){
		mensajeTemp.texto[i]=stringCopia[i];
		if(stringCopia[i] == '\0')
			break;
	}
	printf("ENVIANDO TOKEN A PID: %i TIPO: %c NODO: %i MENSAJE: %s\n",pid,tipo,nodo,mensajeTemp.texto);
	int NODO = saberBuzones(nodo);
	msgsnd(saberBuzones(nodo),(void*)&mensajeTemp,50,0);//envio al nodo
}
void enviarSolicitud(int pid,char tipoProceso,char tipo,int nodo){
        msg mensajeTemp;
        mensajeTemp.tipo = 1;//digo que es de tipo 1
        char textoTemp[50];
        mensajeTemp.texto[0] = tipo;//meto el tipo en el puesto 0
        mensajeTemp.texto[1] = (char)(nodo+48);//pongo el nodo
        mensajeTemp.texto[2] = '\0';//pongo fin de cadena para concatenar
        sprintf(textoTemp, "%d", pid);//paso el pid a string
        char* stringCopia = strcat(mensajeTemp.texto,textoTemp); //concateno en variable auxiliar
        for(i = 0;i<50;i++){
                mensajeTemp.texto[i]=stringCopia[i];
                if(stringCopia[i] == '\0')
                        break;
        }

	for(i = 1;i<50;i++){//añado la coma y el tipo escritor/lector
		if(mensajeTemp.texto[i] == '\0'){
			mensajeTemp.texto[i] = ',';
			mensajeTemp.texto[i+1] = tipoProceso;
			mensajeTemp.texto[i+2] = '\0';
			break;
		}
	}
	//printf("Solicitud PID: %i TipoProceso: %c Nodo: %i Mensaje: %s\n",pid,tipoProceso,nodo,mensajeTemp.texto);
        for(i = 1; i < 4; i++)
		msgsnd(saberBuzones(i),(void*)&mensajeTemp,50,0);//envio al nodo
}
int saberBuzones(int buzon){//funcion que según el nodo devuelve el buzon
        key_t llave1 = ftok("gestor.c",1);
        key_t llave2 = ftok("gestor.c",2);
        key_t llave3 = ftok("gestor.c",3);
        int flag = 0777 | IPC_CREAT;
        int buzon1 = msgget(llave1,flag);
        int buzon2 = msgget(llave2,flag);
        int buzon3 = msgget(llave3,flag);
	switch(buzon){
		case 1: return buzon1;
		case 2: return buzon2;
		case 3: return buzon3;
	}
}
