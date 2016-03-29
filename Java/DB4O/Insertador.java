package practica6;
import java.io.BufferedReader;
import java.io.IOException;

import com.db4o.ObjectContainer;
public class Insertador {
	ObjectContainer db;
	
	public Insertador(ObjectContainer db){
		this.db = db;
	}
	
	public void nuevaPelicula(BufferedReader br){
		try{
			System.out.println("Introduzca ID");
			int ID = Integer.parseInt(br.readLine());
			System.out.println("Introduzca titulo");
			String titulo = br.readLine();
			System.out.println("Introduzca nacionalidad");
			String nacionalidad = br.readLine();
			System.out.println("Introduzca valoracion (entre 0 y 5)");
			float interes = Float.parseFloat(br.readLine());
			Pelicula pel = new Pelicula(ID,titulo,nacionalidad,interes,new Director(0,null,null,null));
			db.store(pel);
		}catch(IOException io){
			io.printStackTrace();
		}
	}
	
	public void nuevoActor(BufferedReader br){
		try{
			System.out.println("Introduzca ID");
			int ID = Integer.parseInt(br.readLine());
			System.out.println("Introduzca nombre");
			String nombre = br.readLine();
			System.out.println("Introduzca nacionalidad");
			String nacionalidad = br.readLine();
			System.out.println("Introduzca IMDB");
			String IMDB = br.readLine();
			Actor actor = new Actor(ID,nombre,nacionalidad,IMDB);
			db.store(actor);
		}catch(IOException io){
			io.printStackTrace();
		}
	}
	
	public void nuevoDirector(BufferedReader br){
		try{
			System.out.println("Introduzca ID");
			int ID = Integer.parseInt(br.readLine());
			System.out.println("Introduzca nombre");
			String nombre = br.readLine();
			System.out.println("Introduzca nacionalidad");
			String nacionalidad = br.readLine();
			System.out.println("Introduzca IMDB");
			String IMDB = br.readLine();
			Director director = new Director(ID,nombre,nacionalidad,IMDB);
			db.store(director);
		}catch(IOException io){
			io.printStackTrace();
		}
	}
	
	public void introducirEjemplo(){
		Director tarantino = new Director(1,"Quentin Tarantino","Americana", "sdsdadsa");
		Director delToro = new Director(2,"Guillermo Del Toro","Mexicana", "asdasdasdasdasd");
		Director ted = new Director(3, "Ted", "Americana", "TEDOOOOOO");
		Director kojima = new Director(4, "Hideo Kojima", "Japonesa", "HIDEGO");
		Director indie = new Director(5, "Un indie", "Británica" , "indie");
		
		Pelicula pulpFiction = new Pelicula(1,"Pulp Fiction","Americana", 5, tarantino);
		tarantino.añadirPelicula(pulpFiction);
		Pelicula pacificRim = new Pelicula(2,"Pacific Rim","Americana", 4.5f, delToro);
		delToro.añadirPelicula(pacificRim);
		Pelicula rambo = new Pelicula(3, "Rambo", "Americana", 4f, ted);
		ted.añadirPelicula(rambo);
		Pelicula metalGear = new Pelicula(4, "Metal Gear", "Japonesa", 5f, kojima);
		kojima.añadirPelicula(metalGear);
		Pelicula diasDespues28 = new Pelicula(5, "28 Días Después", "Británica" , 2, indie);
		indie.añadirPelicula(diasDespues28);
		
		Actor willis = new Actor(6,"Bruce Willis","Alemana", "Die hard");
		Actor segura = new Actor(7,"Santiago Segura","Española", "si hijo si");
		Actor stallone = new Actor(8, "Sylvester Stallone", "Americana", "Rambo");
		Actor hayter = new Actor(9, "David Hayter", "Americana", "ODIADOR");
		Actor murphy = new Actor(10, "Cilian Murphy", "Británica" , "otro indie");
		
		añadirReferenciaActorPelicula(willis,pulpFiction);
		añadirReferenciaActorPelicula(stallone,pulpFiction);
		añadirReferenciaActorPelicula(segura,pulpFiction);
		añadirReferenciaActorPelicula(segura,pacificRim);
		añadirReferenciaActorPelicula(stallone,rambo);
		añadirReferenciaActorPelicula(hayter,metalGear);
		añadirReferenciaActorPelicula(murphy,diasDespues28);
		
		
		db.store(pulpFiction);
		db.store(pacificRim);
		db.store(rambo);
		db.store(metalGear);
		db.store(diasDespues28);
		
		db.store(tarantino);
		db.store(delToro);
		db.store(ted);
		db.store(kojima);
		db.store(indie);
		
		db.store(willis);
		db.store(segura);
		db.store(stallone);
		db.store(hayter);
		db.store(murphy);
	}
	
	void añadirReferenciaActorPelicula(Actor actor,Pelicula pelicula){
		actor.añadirPelicula(pelicula);
		pelicula.añadirActor(actor);
	}
}
