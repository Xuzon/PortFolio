package practica1;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Types;


public class Conector {
	static Connection conexion;
	public static void main(String[] args){
		if(!cargarDriver())
			return;
		if(!conectar(args[0],args[1]))
			return;
		System.out.println("Conexión a:: "+args[0]+" establecida");
		try{
			interactivo();
		}catch(Exception e){
			System.out.println("Algo ha salido mal en el bucle IO");
		}
		desconectar();
	}
	
	static boolean cargarDriver(){
        try {
        	//Cargo el driver jdbc
            Class.forName("com.mysql.jdbc.Driver").newInstance();
            return true;
        } catch (Exception ex) {
        	ex.printStackTrace();
            return false;
        }
	}
	
	static boolean conectar(String database,String autocommit){
		String usuario = "?user=" + "daniel";
		String password = "&password=" + "0511";
		try {
		    conexion = DriverManager.getConnection("jdbc:mysql://localhost/" + database +usuario + password + "&noAccessToProcedureBodies=true");
		    if(autocommit.equals("false"))
		    	conexion.setAutoCommit(false);
		   return true;
		} catch (SQLException ex) {
		    System.out.println("SQLException: " + ex.getMessage());
		    System.out.println("SQLState: " + ex.getSQLState());
		    System.out.println("VendorError: " + ex.getErrorCode());
		    return false;
		}
	}
	
	static void desconectar(){
		try {
			conexion.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	static void ejecutarSQL(String instruccion){
		try {
			Statement st = conexion.createStatement();
			st.execute(instruccion);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	static void crearTablas(){
			ejecutarSQL("create table if not exists Actores(ID_actor int auto_increment,nombre tinytext,primary key(ID_actor))");
			ejecutarSQL("create table if not exists Directores(ID_Director int auto_increment,nombre tinytext,primary key(ID_director))");
			ejecutarSQL("create table if not exists Peliculas(ID_pelicula int auto_increment,titulo tinytext,primary key(ID_pelicula))");
			ejecutarSQL("alter table Peliculas add ID_Director int");
			ejecutarSQL("alter table Peliculas add foreign key (ID_Director) references Directores(ID_Director)");
			
			ejecutarSQL("create table ActorPeliculas(ID_Pelicula int,ID_Actor int,"+
					"Primary Key(ID_Pelicula,ID_Actor),"+
					"Foreign Key(ID_Pelicula) references Peliculas(ID_Pelicula),"+
					"Foreign Key(ID_Actor) references Actores(ID_Actor))");
			
			ejecutarSQL("alter table Actores add IMDB int(7)");
			ejecutarSQL("alter table Actores add unique (IMDB)");

			ejecutarSQL("alter table Directores add IMDB int(7)");
			ejecutarSQL("alter table Directores add unique (IMDB)");

			ejecutarSQL("alter table Peliculas add IMDB int(7)");
			ejecutarSQL("alter table Peliculas add unique (IMDB)");
			
			ejecutarSQL("alter table ActorPeliculas modify ID_pelicula int not null");
			ejecutarSQL("alter table ActorPeliculas modify ID_actor int not null");
			ejecutarSQL("alter table peliculas modify titulo tinytext not null");
			ejecutarSQL("alter table actores modify nombre tinytext not null");
			ejecutarSQL("alter table directores modify nombre tinytext not null");

			ejecutarSQL("alter table actores add edad int(5)");
			ejecutarSQL("alter table actores add check (edad > 0)");
			ejecutarSQL("alter table actores add check (edad < 120)");

			ejecutarSQL("alter table directores add edad int(5)");
			ejecutarSQL("alter table directores add check (edad > 0)");
			ejecutarSQL("alter table directores add check (edad < 120)");
	}
	
	static void interactivo() throws NumberFormatException, IOException{
		BufferedReader br = new BufferedReader( new InputStreamReader(System.in));
		boolean exit = false;
		while(!exit){
			System.out.println("Bienvenido a JavaSQL by Xuzon");
			System.out.println("1) Introducir datos");
			System.out.println("2) Consultar");
			System.out.println("3) Crear tablas");
			System.out.println("4) Borrar de tabla");
			System.out.println("5) Quit");
			int opcion = Integer.parseInt(br.readLine());
			switch(opcion){
				case 1:
					menuIntroducir(br);
					break;
				case 2: menuConsultas(br);
					break;
				case 3: crearTablas();
					break;
				case 4:
					try{
						System.out.println("Nombre de la tabla");
						String tabla = br.readLine();
						System.out.println("ID a borrar (si no sabe cual borrar introduzca -1 para salir");
						String ID = br.readLine();
						if(ID.equals("-1"))
							break;
						borrarEn(tabla,ID);
					}catch(IOException io){
						io.printStackTrace();
					}
					break;
				case 5:
					try{
						conexion.commit();
					}catch(Exception e){
						e.printStackTrace();
					}
					exit = true;
				break;
			}
		}
	}
	
	static void insertarEn(String tabla,String valores){
		String inicio = "insert into ";
		String valoresFinales = " values(" + valores + ")";
		String sentencia = inicio + tabla + valoresFinales;
		ejecutarSQL(sentencia);
	}
	
	static void menuConsultas(BufferedReader br){
		System.out.println("1) Consultar todos los actores");
		System.out.println("2) Consultar todos los directores");
		System.out.println("3) Consultar todas las peliculas");
		System.out.println("4) Capitalizacion películas");
		System.out.println("5) Llamar a procedimiento contar directores");
		System.out.println("6) Llamar a procedimiento poner en mayusculas");
		int opcion = 0;
		try{
			opcion = Integer.parseInt(br.readLine());
		}catch(Exception e){
			e.printStackTrace();
		}
		switch(opcion){
			case 1:
				consultaDevolverTodosLosActores("actores");
				break;
			case 2:
				consultaDevolverTodosLosActores("directores");
				break;
			case 3:
				consultaDevolverTodasLasPeliculas();
				break;
			case 4:
				capitalizacionTabla("peliculas");
				break;
			case 5:
				ejecutarProcedureContarDirectores();
				break;
			case 6:
				try {
					String s = br.readLine();
					ejecutarProcedurePonerEnMayusculas(s);
				} catch (IOException e) {
					e.printStackTrace();
				}
				break;
		}
		
	}
	
	static void consultaDevolverTodosLosActores(String tabla){
		try{
			ResultSet st = ejecutarQuery("select * from " + tabla);
			while(st.next()){
				String id = "";
				if(tabla.equals("actores"))
					id = "ID: " + st.getInt("ID_actor");
				else
					id = "ID: " + st.getInt("ID_director");
				String nombre = " Nombre: " + st.getString("nombre");
				String imdb = " IMDB:" + st.getInt("IMDB");
				String edad = " Edad: " +st.getInt("edad");
				System.out.print(id);
				System.out.print(nombre);
				System.out.print(imdb);
				System.out.println(edad);
				
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	static void consultaDevolverTodasLasPeliculas(){
		try{
			ResultSet st = ejecutarQuery("select * from peliculas");
			while(st.next()){
				String id = "ID: " + st.getInt("ID_pelicula");
				String nombre = " Titulo: " + st.getString("titulo");
				String imdb = " IMDB:" + st.getInt("IMDB");
				String edad = " ID Director: " +st.getInt("ID_director");
				System.out.print(id);
				System.out.print(nombre);
				System.out.print(imdb);
				System.out.println(edad);
				
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	static ResultSet ejecutarQuery(String query){
		try {
			Statement st = conexion.createStatement();
			return st.executeQuery(query);
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
	}

	static void capitalizacionTabla(String tabla){
		try{
			ResultSet st = ejecutarQueryModificable("select * from peliculas");
			while(st.next()){
				String temp = st.getString("titulo");
				if(temp.length() == 0)
					return;
				String s1 = temp.substring(0 , 1);
				s1 = s1.toUpperCase();
				String s2 = "";
				if(temp.length() > 1)
					s2 = temp.substring(1);
				String resultado = s1.concat(s2);
				st.updateString("titulo", resultado);
				st.updateRow();
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	static ResultSet ejecutarQueryModificable(String query){
		try {
			Statement st = conexion.createStatement(ResultSet.TYPE_FORWARD_ONLY,ResultSet.CONCUR_UPDATABLE);
			return st.executeQuery(query);
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
	}

	static void ejecutarProcedureContarDirectores(){
		String exe = ("{call contarDirectores( ? )}");
		try{
			CallableStatement cstm = conexion.prepareCall(exe);
			cstm.registerOutParameter(1, Types.INTEGER);
			cstm.execute();
			System.out.println("El numero de directores es: " + cstm.getInt(1));
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	static void ejecutarProcedurePonerEnMayusculas(String parametro){
		String exe = ("{call ponerEnMayusculas( ? )}");
		try{
			CallableStatement cstm = conexion.prepareCall(exe);
			cstm.registerOutParameter(1, Types.NVARCHAR);
			cstm.setString(1, parametro);
			cstm.execute();
			System.out.println("Tu string en mayusculas es: " + cstm.getString(1));
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	static void menuIntroducir(BufferedReader br){
		System.out.println("1) Introducir en actores");
		System.out.println("2) Introducir en  películas");
		System.out.println("3) Introducir en directores");
		System.out.println("4) Introducir en relacion Peliculas-Actores");
		int opcion = 0;
		try{
			opcion = Integer.parseInt(br.readLine());
		}catch(Exception e){
			e.printStackTrace();
		}
		try{
			String nom = "";
			String imdb = "";
			String edad = "";
			String ejec = "";
			switch(opcion){
				case 1:
					System.out.println("Introduzca nombre:");
					nom = br.readLine();
					System.out.println("Introduzca imdb:");
					imdb = br.readLine();
					System.out.println("Introduzca edad:");
					edad = br.readLine();
					ejec = "null,'" + nom + "'," + imdb + "," + edad;
					insertarEn("actores",ejec);
					break;
				case 2:
					System.out.println("Introduzca titulo:");
					nom = br.readLine();
					System.out.println("Introduzca id del director:");
					edad = br.readLine();
					System.out.println("Introduzca imdb:");
					imdb = br.readLine();
					ejec = "null,'" + nom + "'," + edad + "," + imdb;
					insertarEn("peliculas",ejec);
					break;
				case 3:
					System.out.println("Introduzca nombre:");
					nom = br.readLine();
					System.out.println("Introduzca imdb:");
					imdb = br.readLine();
					System.out.println("Introduzca edad:");
					edad = br.readLine();
					ejec = "null,'" + nom + "'," + imdb + "," + edad;
					insertarEn("directores",ejec);
					break;
				case 4:
					System.out.println("Introduzca id de la pelicula");
					nom = br.readLine();
					System.out.println("Introduzca id del actor");
					imdb = br.readLine();
					ejec = nom + "," + imdb;
					insertarEn("actorPeliculas",ejec);
					break;
			}
		}catch (IOException io){
			io.printStackTrace();
		}
		
	}
	static void borrarEn(String tabla,String id){
		String idFinal = "";
		if(tabla.equals("peliculas"))
			idFinal = "ID_pelicula = " +id;
		if(tabla.equals("actores"))
			idFinal = "ID_actor = " + id;
		if(tabla.equals("directores"))
			idFinal = "ID_director = " + id;
		if(idFinal.equals("")){
			System.out.println("Tabla no permitida saliendo");
			return;
		}
		String ejec = "delete from " + tabla + " where " + idFinal;
		ejecutarSQL(ejec);
	}
}

