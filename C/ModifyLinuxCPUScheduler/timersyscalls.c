#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/signal.h>
//Hago el timer como variable global para luego poder borrarlo, tambien puedo mandarlo como argumento
//a la funcion que vaya a llamar al timer pero por ahora no me molesté, sin embargo lo cambiaré más
//tarde para optimizar
struct timer_list timerColas;
int ultimaCola=-1;
//CUANTOS
unsigned long cuantoColaAltaPrioridad=1200;
unsigned long cuantoColaMediaPrioridad=300;
unsigned long cuantoColaBajaPrioridad=50;

#define AltaPrioridad 3
#define MediaPrioridad 2
#define BajaPrioridad 1

SYSCALL_DEFINE0(iniciarTimer){
	static int inicializado=0;
	if(inicializado==0){
		timerColas.expires=jiffies+200;//hago que el timer se active tras el tiempo en el que estamos más un delay
		timerColas.function=&sys_interrupcion; //digo que cuando el tiempo se complete llame a interrupcion
		init_timer(&timerColas); //inicializo el timer antes de añadirlo si no KERNEL PANIC
		add_timer(&timerColas); //añado el timer a la cola
		printk(KERN_INFO "He creado el timer"); //Mensaje Debug
		inicializado=-1;
	}else{
		printk(KERN_INFO "Ya habia sido inicializado");
	}
	return 0;
}

SYSCALL_DEFINE1(interrupcion,unsigned long,arg){
	del_timer(&timerColas);//lo borro ya que si no está continuamente llamando a esta función
	unsigned long nuevaInterrupcion;//variable del nuevo tiempo que se va a asignar al timer
	
	int hayEnColaAlta=0,hayEnColaMedia=0,hayEnColaBaja=0;
        struct task_struct* proceso;
        for_each_process(proceso){
		switch(proceso->rt_priority){
			case AltaPrioridad: hayEnColaAlta=1;
				break;
			case MediaPrioridad: hayEnColaMedia=1;
				break;
			case BajaPrioridad: hayEnColaBaja=1;
				break;
		}
        }	

	switch(ultimaCola){
		case -1: //este es el caso de por si aun no se inicializo
		case BajaPrioridad:
			if(hayEnColaAlta==1){
				ultimaCola=AltaPrioridad; //si la ultima vez estuvo en baja prioridad
				nuevaInterrupcion = cuantoColaAltaPrioridad; //le pongo el cuanto de alta prioridad
				sys_cambiarCola(BajaPrioridad,AltaPrioridad); //cambio colas
			}else{
				if(hayEnColaMedia){
					ultimaCola=MediaPrioridad;
					nuevaInterrupcion = cuantoColaMediaPrioridad;
					sys_cambiarCola(BajaPrioridad,MediaPrioridad);
				}
				else{	
					ultimaCola=BajaPrioridad;
					nuevaInterrupcion = cuantoColaBajaPrioridad;
				}
			}
			break;
		case AltaPrioridad:
			if(hayEnColaMedia==1){ 
				ultimaCola=MediaPrioridad; // lo mismo con el resto de prioridades
				nuevaInterrupcion = cuantoColaMediaPrioridad;
				sys_cambiarCola(AltaPrioridad,MediaPrioridad);
			}else{
				if(hayEnColaBaja==1){
					ultimaCola=BajaPrioridad;
					nuevaInterrupcion = cuantoColaBajaPrioridad;
					sys_cambiarCola(AltaPrioridad,BajaPrioridad);
				}else{
					ultimaCola=AltaPrioridad;
					nuevaInterrupcion = cuantoColaAltaPrioridad;
				}
			}
			break;
		case MediaPrioridad: 
			if(hayEnColaBaja==1){
				ultimaCola=BajaPrioridad;
				nuevaInterrupcion = cuantoColaBajaPrioridad;
				sys_cambiarCola(MediaPrioridad,BajaPrioridad);
			}else{
				if(hayEnColaAlta==1){
					ultimaCola=AltaPrioridad;
					nuevaInterrupcion = cuantoColaAltaPrioridad;
					sys_cambiarCola(MediaPrioridad,AltaPrioridad);
				}else{
					ultimaCola= MediaPrioridad;
					nuevaInterrupcion = cuantoColaMediaPrioridad;
				}
			}
			break;
	}
	timerColas.expires=jiffies+nuevaInterrupcion;//modificar el timer de las colas y activarlo para la siguiente interrupcion
	timerColas.function=&sys_interrupcion;
	init_timer(&timerColas);
	add_timer(&timerColas);
	return 0;
}

SYSCALL_DEFINE2(cambiarCola,int ,colaActiva,int, siguienteCola){ //al final los rq eran un lio acceder a ellos etc..
	printk("He llamado a cambiar cola con cola Activa %d y siguiente Cola %d\n",colaActiva,siguienteCola);
	struct task_struct* proceso;//asi que decidí recorrer todos los procesos y ver en cada uno su prioridad, si la prioridad
	for_each_process(proceso){	//es la cola activa se deshabilita el proceso si es la cola a activar se activa
		if(proceso->rt_priority==colaActiva){
			force_sig(SIGSTOP,proceso);
		}
		if(proceso->rt_priority==siguienteCola){
			force_sig(SIGCONT,proceso);
		}
	}
} 
