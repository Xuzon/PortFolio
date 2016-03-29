package practica6;

import java.util.Vector;

import com.db4o.ObjectContainer;
import com.db4o.ObjectSet;
import com.db4o.query.Predicate;
import com.db4o.query.Query;

public class Consultor {
	ObjectContainer  db;
	public Consultor(ObjectContainer db){
		this.db = db;
	}
	void getAllPeliculas(){
		ObjectSet<Object> peliculas = (ObjectSet<Object>) db.queryByExample(Pelicula.class);
		for(Object o : peliculas)
			System.out.println(((Pelicula) o).toString());
	}
	
	void getAllPeliculasNacionalidad(String nac){
		ObjectSet<Object> peliculas = (ObjectSet<Object>) db.queryByExample(new Pelicula(0,null,nac,0,null));
		for(Object o : peliculas)
			System.out.println(((Pelicula) o).toString());
	}
	
	void consultaEj6(){
		ObjectSet<Pelicula> peliculas = db.query(
			new Predicate<Pelicula>(){
				private static final long serialVersionUID = 1L;
				@Override
				public boolean match(Pelicula pel) {
					Vector<Actor> actores = pel.getActores();
					if(actores.size() >= 3 && pel.getTitulo().length() > 10)
						return true;
					return false;
				}
			}
		);
		
		for(Pelicula pel : peliculas)
			System.out.println(pel.toString());
	}
	
	void soda(){
		Query query = db.query();
		query.constrain(Persona.class);
		query.descend("nacionalidad").constrain(new String("Americana")).not();
		ObjectSet<Persona> objetos = query.execute();
		for(Persona p : objetos){
			if(p instanceof Actor)
				System.out.println(((Actor) p).toString());
			else
				System.out.println(((Director) p).toString());
		}
	}
}
