package practica6;

import java.util.Vector;

public class Director extends Persona {
	String imdb;
	Vector<Pelicula> peliculasDirigidas;
	
	public Director(int ID,String nombre, String nacionalidad, String IMDB){
		this.ID = ID;
		this.nombre = nombre;
		this.nacionalidad = nacionalidad;
		this.imdb = IMDB;
		peliculasDirigidas = new Vector<Pelicula>();
	}
	
	public void setIMDB(String imdb){
		this.imdb = imdb;
	}
	
	public String getIMDB(){
		return imdb;
	}
	
	public String toString(){
		return "Director ID:" + ID + " nombre: " + nombre + " nacionalidad: " + nacionalidad + " IMDB: " + imdb;
	}
	
	public void añadirPelicula(Pelicula pelicula){
		peliculasDirigidas.addElement(pelicula);
	}
}
