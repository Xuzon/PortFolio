package practica6;

import java.util.Vector;

public class Actor extends Persona {
	private String imdb;
	private Vector<Pelicula> peliculas;
	
	public Actor(int ID,String nombre, String nacionalidad, String IMDB){
		this.ID = ID;
		this.nombre = nombre;
		this.nacionalidad = nacionalidad;
		this.imdb = IMDB;
		peliculas = new Vector<Pelicula>();
	}
	public void setIMDB(String imdb){
		this.imdb = imdb;
	}
	
	public String getIMDB(){
		return imdb;
	}
	
	public String toString(){
		return "Actor ID:" + ID + " nombre: " + nombre + " nacionalidad: " + nacionalidad + " IMDB: " + imdb;
	}
	
	public void añadirPelicula(Pelicula pelicula){
		peliculas.addElement(pelicula);
	}
	
	public Vector<Pelicula> getPeliculas(){
		return peliculas;
	}
	
	public void borrarPelicula(Pelicula pelicula){
		peliculas.remove(pelicula);
	}
}
