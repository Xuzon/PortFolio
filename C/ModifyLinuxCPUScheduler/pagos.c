#include<stdio.h>
#include<time.h>
#include<sys/times.h>
#include "perdidaTiempo.c"

int main(void) {
	clock_t ini = time(NULL);
	clock_t start;
	struct tms otime;
	struct tm *lt;
	start=clock();
	lt=localtime(&ini);

	printf("Inicio el procesado de pagos con PID: %d y policy %s a las %d:%d:%d\n", getpid(),getPolicy(getpid()),lt->tm_hour,lt->tm_min,lt->tm_sec);
	ocuparProcesador(65535);
	printf("Fin del procesado de pagos con PID: %d a las %d:%d:%d tras %f segundos con %ld segundos en procesador\n", getpid(),lt->tm_hour,lt->tm_min,lt->tm_sec,((double)(clock()-start)/CLOCKS_PER_SEC),otime.tms_utime);
	return 0;
}
