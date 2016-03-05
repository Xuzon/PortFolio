package p12;

public class Consumidor extends Thread{
	public int ID;
	public Consumidor(){
	}
	public static Monitor monitorRecursos;

	public Consumidor(int ID){
		this.ID = ID;
		this.start();
		//this.run();
	}
	@SuppressWarnings("deprecation")
	@Override
	public void run(){
		System.out.println("Soy un consumidor de ID "+ID);
		while(true){
			this.suspend();
			System.out.println("Voy a intentar retirar");
			monitorRecursos.buffer(this);
			System.out.println("He consumido un elemento");
		}
	}
}
