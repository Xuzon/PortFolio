package practica6;

import com.db4o.ObjectSet;
import com.db4o.ObjectContainer;
public class Modificador {
	ObjectContainer db;
	public Modificador(ObjectContainer db){
		this.db = db;
	}
	void modificarValoraciones(int ID,float nuevaValoracion){
		ObjectSet<Object> peliculas = (ObjectSet<Object>) db.queryByExample(new Pelicula(ID,null,null,0, null));
		for(Object o : peliculas){
			Pelicula pel = (Pelicula) o;
			pel.setInteres(nuevaValoracion);
			db.store(pel);
		}
	}
	
	void eliminarPeliculaByID(int ID){
		ObjectSet<Object> peliculas = (ObjectSet<Object>) db.queryByExample(new Pelicula(ID,null,null,0,null));
		for(Object o : peliculas){
			Pelicula pel = (Pelicula) o;
			db.delete(pel);
		}
	}
}
