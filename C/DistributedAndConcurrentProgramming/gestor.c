#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include "mensajes.c"
#include <sys/shm.h>
#include <semaphore.h>

void gestor(void);
void meterEnLista(int pid,char Tipo,int nodo);
void sacarLista(int pid);


int i;
int buzon1;
int buzon2;
int buzon3;
int miBuzon;
int reservaMemoria;
int nodo;
process* lista;
sem_t* semaforosMemoria;

void main(int argc,char* argv[]){
	if(argc <2){
		printf("No hay argumentos suficientes\n");
		return;
	}
	nodo = atoi(argv[1]);
	if (nodo != 1 && nodo != 2 && nodo != 3){
		printf("Nodo distinto de 1 2 o 3 \n");
		return;
	}

	//********************ENLACE DE ZONAS DE MEMORIA**********************
	miBuzon = saberBuzones(nodo);
	int llave = ftok("/bin/ls",nodo);
	reservaMemoria = shmget(llave,sizeof(process)*21,0777 | IPC_CREAT);
	lista = (process *) shmat(reservaMemoria,NULL,0);

	//********************SEMAFOROS DE MEMORIA COMPARTIDA*******************
	int llaveSemaforos = ftok("/bin/ls",4);
	int reservaMemoriaSemaforos = shmget(llaveSemaforos,sizeof(sem_t)*3, 0777 | IPC_CREAT);
	semaforosMemoria = (sem_t *) shmat(reservaMemoriaSemaforos,NULL,0);
	sem_init(&semaforosMemoria[nodo-1],0,1);

	//**********************INICIALIZACION MEMORIA COMPARTIDA****************
	if(lista < 0)
		printf("Error asignando la lista\n");
	for(i = 0; i< 20;i++){//inicializo la lista a -1 para indicar que están libres
		lista[i].ID = -1;
	}
	if(nodo == 1){ //al crearse el nodo por primera vez si es el 1 tiene el testigo
		lista[20].ID = -2;//el puesto 21 es para indicar si este nodo
		lista[20].tipo = 's';//tiene el testigo (s) o no (n)
	}else{
		lista[20].ID = -2;
		lista[20].tipo = 'n';
	}
	gestor();
}

//****************GESTIONO LOS MENSAJES Y ACTUO SEGÚN LO QUE ME INDIQUEN************************
void gestor(){
	msg mensaje;
	char mensajeTemp[50];
	int pidTemp;
	char tipoTemp;
	while(1){
		msgrcv(miBuzon,(void*) &mensaje,50,1,0); //espero a los mensajes que gestionan los gestores
		sem_wait(&semaforosMemoria[nodo-1]);
		for(i = 2;i<50;i++){//leo el mensaje para meterlo en la lista
			if(mensaje.texto[i] == ','){
				mensajeTemp[i] = '\0';
				tipoTemp = mensaje.texto[i+1];
				break;
			}
			mensajeTemp[i] = mensaje.texto[i];
		}
		mensajeTemp[0] = '0';
		mensajeTemp[1] = '0';
		pidTemp = atoi(mensajeTemp);
		if(mensaje.texto[0] == 's')
			meterEnLista(pidTemp,tipoTemp,(((int)mensaje.texto[1])-48));//meto en la lista el pid y el tipo
		if(mensaje.texto[0] == 'f')
			sacarLista(pidTemp);
		sem_post(&semaforosMemoria[nodo-1]);
	}
}


//*********************METO PROCESO EN LISTA***********************
void meterEnLista(int pid,char Tipo,int nodo){
	int j = 0;
	process procesoTemporal;
	procesoTemporal.ID = pid;
	procesoTemporal.tipo = Tipo;
	procesoTemporal.nodo = nodo;
	for(j = 0;j<20;j++){
		if(lista[j].ID == -1){//encontré un hueco libre
			//printf("Introduzco en la lista el proceso %i tipo %c del nodo %i en el puesto %i\n",pid,Tipo,nodo,j);
			lista[j] = procesoTemporal;//lo meto
			break;
		}
	}
	//vamos a comprobar si la lista estaba vacia
	if(j == 0 && lista[20].tipo == 's'){//si el nodo tiene el token y el proceso se metió en el primer puesto
		for(j = 1;j < 20;j++){
			if(lista[j].ID != -1)
				break;
			if(j == 19){//la lista estaba vacia asi que hay que enviarle el token
				enviarToken(lista[0].ID,'n',lista[0].nodo,nodo);
				//printf("Lista vacía enviando token al proceso %i del nodo %i\n",lista[0].ID,lista[0].nodo);
				lista[20].tipo = 'n';//digo que el token no está en este nodo
			}
		}
	}
}
//*****************SACO PROCESO DE LISTA*******************************
void sacarLista(int pid){
	int  j = 0;
	int pos = 0;
	for(j = 0; j < 20;j++){
		if(lista[j].ID == pid){//encuentro el que quiero quitar
			pos = j;//marco en que lugar de la lista estaba
			//printf("El proceso %i ha acabado así que lo borro de la lista\n",pid);
			break;
		}
	}
	for(j = pos ;j<20;j++){
		if(lista[j+1].ID == -1){//si el siguiente puesto está libre
			lista[j].ID = -1;//indica que hay que eliminarlo
			break;
		}
		lista[j] = lista[j+1];//si no lo muevo en la lista para atrás
	}
}
