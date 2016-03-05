package p12;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;

public class Menu {

	@SuppressWarnings("deprecation")
	public static void main(String[] args){
		int opcion;
		Consumidor c;
		Productor p;
		Scanner keyboard = new Scanner(System.in);
		ArrayList<Thread> hilos = new ArrayList<Thread>();
		Monitor monitor = new Monitor();
		Productor.monitorRecursos = monitor;
		Consumidor.monitorRecursos = monitor;
		Iterator<Thread> iterador;
		int ids = 1;
		int threadID = 0;
		while (true){
			System.out.println("1.Crear Productor");
			System.out.println("2.Crear Consumidor");
			System.out.println("3.Productor intenta introducir elemento");
			System.out.println("4.Productor finaliza la acción introducir elemento");
			System.out.println("5.Consumidor intenta retirar elemento");
			System.out.println("6.Consumidor finaliza la acción de consumir elemento");
			System.out.println("7.Salir");
			opcion = keyboard.nextInt();
			if(opcion != 1 && opcion !=2 && opcion != 7){
				System.out.println("\t Indique el thread ID");
				threadID = keyboard.nextInt();
				iterador = hilos.iterator();
			}
			switch(opcion){
				case 1:  p = new Productor(ids);
					hilos.add(p);
					ids++;
					break;
				case 2: c = new Consumidor(ids);
					hilos.add(c);
					ids++;
					break;
				case 3: p = (Productor) buscar(hilos,threadID);
					p.resume();
					break;
				case 4: p = (Productor) buscar(hilos,threadID);
					p.resume();
					break;
				case 5: c = (Consumidor) buscar(hilos,threadID);
					c.resume();
					break;
				case 6:	c = (Consumidor) buscar(hilos,threadID);
					c.resume();
					break;
				case 7:
					iterador = hilos.iterator();
					while(iterador.hasNext()){
						iterador.next().interrupt();
					}
					keyboard.close();
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
