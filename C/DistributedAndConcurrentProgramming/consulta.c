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

char recibir(int id_cola, char* nodoEspecial);

int main(int argc, char *argv[]){

	if(argc!=2){
		printf("Error de parametros:\n./consulta [nº nodo]\n");
		exit(0);
	}
	int nodo = atoi(argv[1]);

	//************************INICIALIZACION DE SEMAFOROS********************
	sem_t* semaforosMemoria;//semaforos de acceso a memoria compartida
	int llaveSemaforos = ftok("/bin/ls",4);//llave de memoria
	int reservaMemoriaSemaforos = shmget(llaveSemaforos,sizeof(sem_t)*3, 0777 | IPC_CREAT);//id memoria
	semaforosMemoria = (sem_t *) shmat(reservaMemoriaSemaforos,NULL,0);//semaforos

	//************************ZONAS DE MEMORIA******************************
	key_t key;
	printf("PROCESO CONSULTA(PID->%d), LANZADO CON EXITO...\n\n", getpid());
	key  = ftok("/bin/ls", nodo);//llave de memoria de lista
	if(key ==-1)//comprobacion de seguridad
		printf("error creacion de key \n");
	int shmid = shmget(key,sizeof(process)*21, IPC_CREAT|0777);//id memoria
	if(shmid==-1)//comprobación
		printf("Error al crear la memoria compartida\n");
	process *addr = shmat(shmid, NULL,0);//lista de procesos
	if(addr==NULL)//comprobacion
		printf("Error al enlazar la memoria\n");
	int id_cola = saberBuzones(nodo);//dirección de mi buzon
	if(id_cola==-1) //comprobacion de buzon
		printf("Error al crearse el buzon\n");

	//***********************SOLICITUD DE SC******************************************
	srand(time(NULL));//inicializo semilla de aletoriedad
	int retardo = (rand())%5;//espero de 0 a 5 segundos antes de solicitar entrar en la sc
	sleep(retardo);
	enviarSolicitud(getpid(),'l', 's',nodo);//comunico a los nodos que quiero entrar en la sc
	char nodoespecial;
	char tipo_token = recibir(id_cola,&nodoespecial);//aqui me suspendo y espero a que me entreguen el testigo

	//**********************ENVIO DE TOKENS ESPECIALES***************************************************
	int contador;//variable auxiliar
	int pids[5];//guardo los pids de los procesos a quien envio token
	if(tipo_token == 'n'){//si tengo token normal envio especiales y marco
		int i = 0;
 		contador=0;
    		sem_wait(&semaforosMemoria[nodo-1]);//espero a acceder a memoria compatida
		for(i = 0; i < 5; i++)
			pids[i] = -1;
		for(i = 0; i < 20; i++){//envio a los siguientes en la cola a mi es por seguridad
			if(addr[i].ID == getpid())
				break;
		}
    		for(;i<20 && contador < 5;i++){//recorro la lista hasta el final o hasta despertar a 5
      			if(addr[i].ID != -1 && (addr[i].tipo == 'l') && addr[i].ID != getpid()){//si encuentro un lector que no sea yo
				enviarToken(addr[i].ID,'e',addr[i].nodo,nodo);//envio token especial
				pids[contador] = addr[i].ID;//lo añado a la lista de pids
				contador++;//cuento uno mas
      			}
    		}
    		addr[20].tipo='s';//marco
    		sem_post(&semaforosMemoria[nodo-1]);//dejé de acceder a memoria compartida
  	}
	else
		printf("TENGO UN TOKEN ESPECIAL\n");

	//*****************************SC****************************************
	retardo = ((rand())%4)+2;//Entro en la SC un tiempo entre 2 y 6
	printf("PROCESO CONSULTA(PID->%d): >>>>> ESTOY EN MI SECCION CRITICA <<<<<\n\n", getpid());
	sleep(retardo);

	//****************************SALIDA DE SC********************************
	printf("PROCESO CONSULTA(PID->%d), SALGO DE MI SECCION CRITICA...\n\n", getpid());
	if(tipo_token=='n'){//si tengo el token normal
	//***************************ESPERA DE FINALIZACION DE OTRAS SOLICITUDES
		while(contador != 0) {//hasta que no espere mas tokens especiales
			msg mensaje;
      			msgrcv(id_cola,&mensaje,50,3,0);//espero la respuesta de a quien envié tokens speciales
      			contador--;
    		}
		enviarSolicitud(getpid(),'l','f',nodo);//comunico a los gestores que acabé
		sem_wait(&semaforosMemoria[nodo-1]);//voy a acceder a memoria compartida
	//*************************ESCOGER AL SIGUIENTE PROCESO A ENVIAR TOKEN****************************
		int siguiente = -1;
		int nodoSiguiente;
		for(i = 0; i < 20 ; i++){//escojo el siguiente a quien enviar el token
			if(addr[i].ID == getpid())//si soy yo paso
				continue;
                	if(addr[i].ID == pids[0] || addr[i].ID == pids[1] || addr[i].ID == pids[2] || addr[i].ID == pids[3] || addr[i].ID == pids[4])//o uno a quien le envié el token
                        	continue;
			if(addr[i].ID != -1){//si es uno valido lo escojo
				siguiente = addr[i].ID;
				nodoSiguiente = addr[i].nodo;
				break;
			}
		}
	//***************************ENVIO O NO DE TOKEN*********************************************
		if(siguiente == -1){//si no hay siguiente
			printf("No hay siguiente en cola así que el token se destruye conmigo\n");
			addr[20].tipo = 's';//marco que se queda el token aqui(creo que sobra)
			sem_post(&semaforosMemoria[nodo-1]);
			return 0;
  		}
  		addr[20].tipo = 'n';//si hay siguiente marco que el token se va de este nodo
		sem_post(&semaforosMemoria[nodo-1]);//acabé el acceso a memoria compartida
		enviarToken(siguiente,'n',nodoSiguiente,nodo);//envio el token al siguiente proceso

	//***************************ENVIO DE ACK DE FINALIZACION A LA CONSULTA CON TOKEN*************
	}else{ //si tengo el token especial
    		msg mensaje;//variable donde guardar el mensaje
		enviarSolicitud(getpid(),'l','f',nodo);//comunico a los gestores que acabé
		mensaje.tipo = 3;//marco el mensaje a tipo 3 (tipo que espera a recibir el proceso consult "Jefe")
		msgsnd(saberBuzones((int)nodoespecial-48),&mensaje,50,0);//se lo envio al proceso consulta que me dio el token especial
  	}
	return 0;//acabé el programa
}


//************************FUNCION DE ESPERA DE TOKEN***********************
char recibir(int id_cola, char *nodoespecial){

	msg mensaje;
	char tipotoken = '0';
	char nodo = '0';
	while(1){
		msgrcv(id_cola,(void *)&mensaje,50,2,0);//espero por el token
		tipotoken = '0';
		nodo = '0';
		tipotoken=mensaje.texto[0];//guardo el tipo de token
		nodo = mensaje.texto[1];//y el nodo de quien lo envio
		mensaje.texto[0]='0';//modificamos el mensaje para usar atoi
		mensaje.texto[1]='0';
		int pid= atoi(mensaje.texto);//cojo el pid del proceso a quien está dirigido
		if(pid==getpid())//si es para mi
			break;
 		else{//si no
			mensaje.texto[0]=tipotoken;//restauro el mensaje
      			mensaje.texto[1]=nodo;
      			mensaje.tipo = 2;
      			msgsnd(id_cola,(void *)&mensaje,50,0);//y lo vuelvo a dejar en la cola
    		}
	}
	*nodoespecial=nodo;//almaceno de que nodo me llegó el token
	return tipotoken;
}









