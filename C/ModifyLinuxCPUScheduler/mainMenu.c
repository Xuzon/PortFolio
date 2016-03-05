#include<stdio.h>
#include<unistd.h>

void crearProcesos(int numeroProcesos[]);

int main() {
	int read;
	int nprocesos[5];
	printf("Numero de procesos de consulta: ");
	scanf("%d%*c",&read);
	nprocesos[0]=read;
	printf("Numero de procesos de pre-reserva: ");
	scanf("%d%*c",&read);
	nprocesos[1]=read;
	printf("Numero de procesos de anulacion: ");
	scanf("%d%*c",&read);
	nprocesos[2]=read;
	printf("Numero de procesos de pago: ");
	scanf("%d%*c", &read);
	nprocesos[3]=read;
	printf("Numero procesos de pre-reserva larga: ");
	scanf("%d%*c",&read);
	nprocesos[4]=read;	

	crearProcesos(nprocesos);
	wait();
	return 0;
}


void crearProcesos(int numeroProcesos[]){
	int i,j;
	pid_t cpid;
	for (j=0;j<5;j++) {
		for (i=0;i<numeroProcesos[j];i++) {
			cpid=fork();
			if (cpid==0) {
				if (j==0) {
					printf("Lanzo consulta con pid: %d\n",getpid());
					execl("consulta","consulta",NULL);
				}
				else if (j==1) {
					printf("Lanzo prereserva con pid: %d\n",getpid());
					execl("prereserva","prereserva",NULL);
				}
				else if (j==2) {
					printf("Lanzo anulacion con pid: %d\n", getpid());
					execl("anulacion","anulacion",NULL);
				}
				else if (j==3) {
					printf("Lanzo pagos con pid: %d\n", getpid());
					execl("pagos","pagos",NULL);
				}
				else {
					printf("Lanzo preserva larga con pid: %d\n", getpid());
					execl("prereserval","prereserval",NULL);
				}
			}
			else if (cpid<0){
				perror("error fork\n");
			}
			else {
				printf("--SIGO tras crear %d--\n",cpid);
			}
		} //fin for i
	} //fin for j
}
