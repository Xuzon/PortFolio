package practica6;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.db4o.ObjectContainer;

public class Menu {
	static ObjectContainer db;
	static Conexion conexion;
	static Insertador insertador;
	static Consultor consultor;
	static Modificador modificador;
	public static void main(String[] args) {
		inicializacion(args);
		//insertador.introducirEjemplo();
		menu();
		db.close();
	}
	
	static void inicializacion(String[] args){
		if(args.length > 0)
			conexion = new Conexion(args[0]);
		else
			conexion = new Conexion();
		insertador = new Insertador(db);
		consultor = new Consultor(db);
		modificador = new Modificador(db);
	}
	
	static void menu(){
		boolean exit = false;
		BufferedReader br = new BufferedReader( new InputStreamReader(System.in));
		int opcion = -1;
		int ID;
		float interes;
		try{
			while(!exit){
				imprimirCabeceraMenu();
				opcion = Integer.parseInt(br.readLine());
				switch(opcion){
				case 1: 
					consultor.getAllPeliculas();
					break;
				case 2: 
					System.out.println("Introduzca nacionalidad");
					consultor.getAllPeliculasNacionalidad(br.readLine());
					break;
				case 3:
					insertador.nuevaPelicula(br);
					break;
				case 4:
					System.out.println("Introduzca ID");
					ID = Integer.parseInt(br.readLine());
					System.out.println("Introduzca valoracion (entre 0 y 5)");
					interes = Float.parseFloat(br.readLine());
					modificador.modificarValoraciones(ID, interes);
					break;
				case 5:
					System.out.println("Introduzca ID");
					ID = Integer.parseInt(br.readLine());
					modificador.eliminarPeliculaByID(ID);
					break;
				case 6:
					consultor.consultaEj6();
					break;
				case 7:
					consultor.soda();
					break;
				case 8:
					exit = true;
					break;
				}
			}
		}catch(IOException io){
			io.printStackTrace();
		}
	}
	
	static void imprimirCabeceraMenu(){
		System.out.println();
		System.out.println();
		System.out.println("1)Para consultar todas las peliculas");
		System.out.println("2)Para consultar todas las peliculas de una nacionalidad");
		System.out.println("3)Para introducir una película nueva");
		System.out.println("4)Para modificar el interes de una película (debe saber la ID)");
		System.out.println("5)Para eliminar una película (debe saber la ID)");
		System.out.println("6)Para consultar peliculas con más de 3 actores y más de 10 caracteres de titulo");
		System.out.println("7)Para consultar actores y directores no USA (SODA)");
		System.out.println("8)Para salir");
	}
}
