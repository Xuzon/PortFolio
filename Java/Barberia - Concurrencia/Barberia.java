package p5;

import java.util.concurrent.locks.*;
public class Barberia {
	private Cliente  c;
	private boolean barbero;
	private Lock mi_lock;
	private Condition cola;
	private Condition silla;
	private Condition cortandose1;
	private int clientes = 0;
	private Cliente clientesCortandose[];
	public Barberia(){
			clientesCortandose = new Cliente[1];
			mi_lock = new ReentrantLock();
			cola = this.mi_lock.newCondition();
			silla = this.mi_lock.newCondition();
			cortandose1 = this.mi_lock.newCondition();
	}
	
	
	public void LLegadaCliente(Thread t) throws InterruptedException{
		this.mi_lock.lock();
		clientes++;
		if(clientes > 1){
			System.out.println("\tHay alguien cortandose el pelo, me pondré a la cola");
			this.cola.await();
		}
		Entry(t);
	}
	
	
	public void Entry(Thread t) throws InterruptedException{
		if(t instanceof Cliente){
			c = (Cliente) t;
			if(barbero){//si el barbero está durmiendo
				silla.signal();//despierto al barbero
				silla.await();
			}
			clientesCortandose[0] = c;
			System.out.println("\t[Cliente "+c.ID+"] Hola");
			System.out.println("\t[Cliente "+c.ID+"] Me están cortando el pelo\n");
			cortandose1.await();//me pongo esperando a que me corten el pelo
			System.out.println("\t[Cliente "+c.ID+"] Muchas gracias por el corte, Adiós");
			clientes--;
			silla.signal();
			this.mi_lock.unlock();
		}else{//parte del barbero
			this.mi_lock.lock();
			while(true){
				if(clientes == 0){
					barbero = true;//estoy en la silla
					System.out.println("\t[Barbero] Durmiendo\n");
					silla.await();//me pongo a dormir
					barbero = false;//digo que no estoy en la silla
					System.out.println("\t[Barbero] Me despierta\n");
					System.out.println("\t[Barbero] Hola");
					silla.signal();//el cliente esperando a ocupar la silla la ocupa
				}else{
					System.out.println("\t[Barbero] Hola");
					cola.signal();
				}
				silla.await();//y yo me pongo de pie a esperar (CORTANDO EL PELO)
				System.out.println("\t[Barber] Adios");
			}
		}
	}
	public void terminarCorte(int ID){
		this.mi_lock.lock();
		if(clientesCortandose[0].ID == ID)
			cortandose1.signal();
		else
			System.out.println("Esa ID no pertenece al cliente que está cortandose");
		this.mi_lock.unlock();
	}
}
