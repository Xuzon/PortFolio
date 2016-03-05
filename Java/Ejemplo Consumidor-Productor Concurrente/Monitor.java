package p12;

import java.util.ArrayList;

public class Monitor {
	private ArrayList<String> buffer;
	private int posicionPunteroBuffer;
	Consumidor c;
	Productor p;
	public Monitor(){
			buffer = new ArrayList<String>();
			posicionPunteroBuffer = 0;
	}
	
	@SuppressWarnings("deprecation")
	public synchronized void buffer(Thread t){
		if(t instanceof Productor){
			p = (Productor) t;
			System.out.println("Produciendo");
		}
		if(t instanceof Consumidor){
			c = (Consumidor) c;
			System.out.println("Consumiendo");
		}
		t.suspend();
	}
}
