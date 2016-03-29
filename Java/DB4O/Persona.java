package practica6;

public abstract class Persona {
	protected int ID;
	protected String nombre;
	protected String nacionalidad;
	
	public void setID(int ID){
		this.ID = ID;
	}
	public int getID(){
		return ID;
	}
	public void setNombre(String nombre){
		this.nombre = nombre;
	}
	public String getNombre(){
		return nombre;
	}
	
	public void setNacionalidad(String nacionalidad){
		this.nacionalidad = nacionalidad;
	}
	public String getNacionalidad(){
		return nacionalidad;
	}
}
