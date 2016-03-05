package p5;

public class Cliente extends Thread{
	public static Barberia monitorRecursos;
	public int ID;
	public Cliente(){
	}
	
	public Cliente(int ID){
		this.ID = ID;
		System.out.println("\tSoy cliente de ID "+ID+"\n");
		this.start();
	}
	@SuppressWarnings("deprecation")
	@Override
	public void run(){
		while(true){
			this.suspend();
			System.out.println("\tVoy a intentar entrar en la barberia\n");
			try {
				monitorRecursos.LLegadaCliente(this);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
