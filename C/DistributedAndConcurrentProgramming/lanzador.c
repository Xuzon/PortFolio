#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(int argc, char* argv[]){
	int lectores;
	int pagos;
	int prereservas;
	int anulaciones;
	if(argc < 2){
		printf("Falta indicar el nodo\n");
		return;
	}
	int pid = fork();
	if(!pid){
		printf("Lanzando proceso gestor del nodo %s\n",argv[1]);
		execl("gestor","gestor",argv[1],NULL);
	}
	while(1){
		printf("\t1.Prueba\n");
		printf("\t2.Salir\n");
		char c = getchar();
		if(c == '2')
			return;
		c = getchar();
		fflush(stdin);

		printf("\tCuantos procesos consulta?\n");
		c = getchar();
		lectores = c - '0';
		c = getchar();
		fflush(stdin);

                printf("\tCuantos procesos pagos?\n");
                c = getchar();
                pagos = c - '0';
                c = getchar();
                fflush(stdin);

                printf("\tCuantos procesos prereservas?\n");
                c = getchar();
                prereservas = c - '0';
                c = getchar();
                fflush(stdin);

                printf("\tCuantos procesos anulaciones?\n");
                c = getchar();
                anulaciones = c - '0';
                c = getchar();
                fflush(stdin);

		int i;
		//lanzamiento consultas
		for(i = 0;i < lectores;i++){
			pid = fork();
			if(!pid){
				execl("consulta","consulta",argv[1],NULL);
			}
		}
		//lanzamiento pagos
                for(i = 0;i < pagos;i++){
                        pid = fork();
                        if(!pid){
                                execl("escritor","escritor",argv[1],"PAGO",NULL);
                        }
                }
		//lanzamiento prereservas
                for(i = 0;i < prereservas;i++){
                        pid = fork();
                        if(!pid){
                                execl("escritor","escritor",argv[1],"PRERESERVA",NULL);
                        }
                }

		//lanzamiento anulaciones
                for(i = 0;i < anulaciones;i++){
                        pid = fork();
                        if(!pid){
                                execl("escritor","escritor",argv[1],"ANULACION",NULL);
                        }
                }

	}
}
