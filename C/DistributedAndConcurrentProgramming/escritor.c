#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <time.h>
#include "mensajes.c"
#include <sys/shm.h>
#include <semaphore.h>

char recibir(int id_cola);

int main(int argc, char *argv[]){

	if(argc<3){
		printf("Error de parametros:\n");
    		exit(0);
  	}

	//***********************INICIALIZACION DE SEMAFOROS******************
	int nodo = atoi(argv[1]);
	sem_t* semaforosMemoria;//semaforos de acceso a memoria compartida
	int llaveSemaforos = ftok("/bin/ls",4);//llave de memoria de semaforos
	int reservaMemoriaSemaforos = shmget(llaveSemaforos,sizeof(sem_t)*3, 0777 | IPC_CREAT);//id de memoria de semaforos
	semaforosMemoria = (sem_t *) shmat(reservaMemoriaSemaforos,NULL,0);//asignacion de memoria de semaforos

	//**********************ZONAS DE MEMORIA********************
	key_t key;
  	printf("PROCESO %s (PID->%d), LANZADO CON EXITO...\n\n",argv[2], getpid());
  	key  = ftok("/bin/ls", nodo);//llave de memoria compartida
	if(key ==-1)//comprobacion
		printf("error creacion de key \n");
	int shmid = shmget(key,sizeof(process)*21, IPC_CREAT|0777);//id memoria compartida
	if(shmid==-1)//comprobacion
		printf("Error al crear la memoria compartida\n");
	process *addr = shmat(shmid, NULL,0);//lista en memoria compartida
	if(addr==NULL)//comprobacion
		printf("Error al enlazar la memoria\n");
	int id_cola = saberBuzones(nodo);//direccion de buzon propio
	if(id_cola==-1)//comprobacion
		printf("Error al crearse el buzon\n");

	//*************************SOLICITUD DE SC*********************
	srand(time(NULL));//inicializamos semilla
	int retardo = (rand())%5;//random de 0 a 5
	sleep(retardo);//esperamos el random antes de solicitar entrada a sc
	enviarSolicitud(getpid(),'e', 's',nodo);//envio solicitud de entrada a sc a todos los nodos
	recibir(id_cola);//me suspendo hasta que me llegue el token

	//*************************SC*********************************
	retardo = ((rand())%4)+2;//Entro en la SC un tiempo entre 2 y 6
	printf("PROCESO %s (PID->%d): >>>>> ESTOY EN MI SECCION CRITICA <<<<<\n\n",argv[2], getpid());
	sleep(retardo);

	//************************SALIDA DE SC*******************************
	printf("PROCESO %s (PID->%d), SALGO DE MI SECCION CRITICA...\n\n",argv[2], getpid());
	enviarSolicitud(getpid(),'e','f',nodo);//aviso a los nodos que acabé mi SC

	//********************ESCOGER SIGUIENTE PROCESO A ENVIAR TOKEN*************************
	sem_wait(&semaforosMemoria[nodo-1]);//voy a acceder a memoria compartida asi q EM
	int siguiente = -1;
	int nodoSiguiente;
	for(i = 0; i < 20 ; i++){//escojo el siguiente proceso a enviar el token
		if(addr[i].ID == getpid())//si soy yo paso
			continue;
		if(addr[i].ID != -1){//lo selecciono
			siguiente = addr[i].ID;
			nodoSiguiente = addr[i].nodo;
			break;
		}
  	}

	//***********************ENVIO O NO DE TOKEN********************************************
  	if(siguiente == -1){//si no hay siguiente
		printf("No hay siguiente en cola así que el token se destruye conmigo\n");
		addr[20].tipo = 's';//marco en memoria que se quedó el token aqui
		sem_post(&semaforosMemoria[nodo-1]);//acabé memoria compartida
		return 0;
 	}
  	addr[20].tipo = 'n';//si hay siguiente desmarco que tenemos el token
	sem_post(&semaforosMemoria[nodo-1]);//dejo de acceder a memoria compartida
	enviarToken(siguiente,'n',nodoSiguiente,nodo);//paso el token
	return 0;
}


//********************************FUNCION DE ESPERAR A TU TOKEN************************
char recibir(int id_cola){//igual que la función recibir de consulta pero sin
	char tipotoken = '0';//la necesidad de guardar el nodo de donde vino el token
	char nodo = '0';
	msg mensaje;
	while(1){
    		msgrcv(id_cola,(void *)&mensaje,50,2,0);
    		tipotoken = '0';
    		nodo = '0';
    		tipotoken=mensaje.texto[0];
    		nodo = mensaje.texto[1];
    		mensaje.texto[0]='0';
    		mensaje.texto[1]='0';
    		int pid= atoi(mensaje.texto);
    		if(pid==getpid())
			break;
    		else{
      			mensaje.texto[0]=tipotoken;
      			mensaje.texto[1]=nodo;
      			mensaje.tipo = 2;
      			msgsnd(id_cola,(void *)&mensaje,50,0);
    		}
  	}
  return tipotoken;
}//fin de recibir









