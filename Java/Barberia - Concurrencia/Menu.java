package p5;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;

public class Menu {

	@SuppressWarnings("deprecation")
	public static void main(String[] args){
		int opcion;
		Cliente c;
		Scanner keyboard = new Scanner(System.in);
		ArrayList<Thread> hilos = new ArrayList<Thread>();
		Barberia monitor = new Barberia();
		Cliente.monitorRecursos = monitor;
		Barbero.monitorRecursos = monitor;
		new Barbero();
		int ids = 1;
		int threadID = 0;
		while (true){
			System.out.println("1.Crear cliente");
			System.out.println("2.Mandar cliente");
			System.out.println("3.Terminar de cortar pelo al cliente");
			System.out.println("4.Salir");
			opcion = keyboard.nextInt();
			if(opcion == 2 || opcion == 3){
				System.out.println("\t Indique el thread ID");
				threadID = keyboard.nextInt();
			}
			switch(opcion){
				case 1:  c = new Cliente(ids);
					hilos.add(c);
					ids++;
					break;
				case 2: c = (Cliente) buscar(hilos,threadID);
					c.resume();
					break;
				case 3: monitor.terminarCorte(threadID);
					break;
				case 4:	keyboard.close();
					System.exit(0);
					return;
				default:
					break;
			}
		}
	}

	public static Thread buscar(ArrayList<Thread> hilos,int posicion){
		Thread hilo = null;
		Iterator<Thread> iterador = hilos.iterator();
		for(int i = 0;i<posicion;i++){
			hilo = iterador.next();
		}
		return hilo;
	}
}
