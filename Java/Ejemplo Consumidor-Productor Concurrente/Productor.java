package p12;

public class Productor extends Thread{
	public int ID;
	public static Monitor monitorRecursos;
	public Productor(){
	}
	
	public Productor(int ID){
		this.ID = ID;
		this.start();
		//this.run();
	}
	@SuppressWarnings("deprecation")
	@Override
	public void run(){
		System.out.println("Soy un productor de ID "+ID);
		while(true){
			this.suspend();
			System.out.println("Voy a intentar producir");
			monitorRecursos.buffer(this);
			System.out.println("He producido");
		}
	}
}
