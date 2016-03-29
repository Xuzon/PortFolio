package practica6;

import com.db4o.Db4o;
import com.db4o.ObjectContainer;
import com.db4o.config.Configuration;
@SuppressWarnings("deprecation")
public class Conexion {
	String path = "baseDeDatos.db4o";
	ObjectContainer db;

	public Conexion(){
		Configuration config = Db4o.configure();
		db = Db4o.openFile(config,this.path);
		Menu.db = db;
	}
	
	public Conexion(String path){
		this.path = path;
		Configuration config = Db4o.configure();
		db = Db4o.openFile(config,this.path);
		Menu.db = db;
	}
}
