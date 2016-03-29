package practica6;

import java.util.Vector;

public class Pelicula {
	private int ID;
	private String titulo;
	private String nacionalidad;
	private float interes;
	private Director director;
	private Vector<Actor> actores;
	public Pelicula(){}
	
	public Pelicula(int ID, String titulo, String nacionalidad, float interes, Director director){
		this.ID = ID;
		this.titulo = titulo;
		this.nacionalidad = nacionalidad;
		this.interes = interes;
		this.director = director;
		actores = new Vector<Actor>();
	}
	
	public void setID(int ID){
		this.ID = ID;
	}
	
	public int getID(){
		return ID;
	}
	
	public void setTitulo(String titulo){
		this.titulo = titulo;
	}
	
	public String getTitulo(){
		return titulo;
	}
	
	public void setNacionalidad(String nacionalidad){
		this.nacionalidad = nacionalidad;
	}
	
	public String getNacionalidad(){
		return nacionalidad;
	}
	
	public boolean setInteres(float interes){
		if(interes <= 5 && interes >= 0)
			this.interes = interes;
		else
			return false;
		return true;
	}
	public float getInteres(){
		return interes;
	}
	
	public String toString(){
		return "ID: " +ID + " titulo: "+ titulo + " nacionalidad: " + nacionalidad + " interés: " + interes;
	}
	
	public void setDirector(Director director){
		this.director = director;
	}
	public Director getDirector(){
		return director;
	}
	public void añadirActor(Actor actor){
		actores.addElement(actor);
	}
	public Vector<Actor> getActores(){
		return actores;
	}
	
	public void borrarActor(Actor actor){
		actores.remove(actor);
	}
}
