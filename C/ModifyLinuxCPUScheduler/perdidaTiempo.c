#include<stdio.h>
#include<unistd.h>
#include<sched.h>

void ocuparProcesador(unsigned long long iteraciones) {
	unsigned long long i;
	for (i=0;i<=iteraciones;i++){
		if ((i%10000)==0) {
//			printf("Proceso con PID: %d lleva %llu iteraciones\n", getpid(), i);
		}
	}
}

char* getPolicy (pid_t pid) {
	int ipol;
	ipol = sched_getscheduler(pid);

	switch(ipol) {
		case 0: return "SCHED_NORMAL";
		case 1: return "SHCED_FIFO";
		case 2: return "SCHED_RR";
		case 3: return "SCHED_BATCH";
		case 4: return "SCHED_ISO"; //not implemented in system
		case 5: return "SCHED_IDLE";
		case 6: return "SCHED_DEADLINE";
		default: return "unknow";
	}
}
