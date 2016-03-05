package p5;

public class Barbero extends Thread{
	public static Barberia monitorRecursos;
	
	public Barbero(){
		System.out.println("He creado al barbero\n");
		this.start();
	}
	
	@Override
	public void run(){
		try {
			monitorRecursos.Entry(this);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
