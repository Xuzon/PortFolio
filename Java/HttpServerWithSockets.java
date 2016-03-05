//Carlos Daniel Garrido Suárez
//Bruno Nogareda Da Cruz

package skynet;

import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.zip.DeflaterInputStream;

class Terminator implements Runnable{
	Socket objetivo;
	InputStreamReader canalRecibirDatos;
	OutputStream canalMandarDatos;
	private Thread me;
	String lineaentrada,conexionAlive,GET,HTTPversion;
	String cabeceraRespuesta="";
	String DefaultDir;
	String modificadaDesde;
	String[] codificaciones;
	List<String> DatosEntrada = new ArrayList<String>();
	long miliSegundosUltimaConexion;
	int tiempoMaximoConexion=20000;
	String nameServer="Skynet Server/1.0";
	Date fechaTemporal;
	boolean codifDeflate;
	boolean persistente;
	int numeroIteraciones;
	SimpleDateFormat formatearFecha= new SimpleDateFormat("EE, dd MMM yyyy HH:mm:ss zzz",Locale.ROOT); //pongo el formato
	
	Terminator(Socket inicializacion, String i, String dir){
		objetivo=inicializacion;
		me=new Thread (this,i);
		me.start();
		DefaultDir=dir;
	}
	Terminator(){
		
	}
	public void run(){
		fechaTemporal=new Date();
		destruir();
		System.out.println("Conexión cerrada");
		return;
	}
	void destruir(){
		try {
			
		while(true){
				if(miliSegundosUltimaConexion==0)
					miliSegundosUltimaConexion=fechaTemporal.getTime();
			
				if(canalRecibirDatos==null)
					canalRecibirDatos=new InputStreamReader(objetivo.getInputStream());//si no hay entrada sale de destruir por el catch
			
				BufferedReader bf = new BufferedReader (canalRecibirDatos);
			
			
			//para resetear la cabecera de entrada
				DatosEntrada.clear();
				codifDeflate=false;
				codificaciones=null;
				conexionAlive= null;
				modificadaDesde=null;
				GET=null;
				
				while(bf!=null){
			
					fechaTemporal=new Date(); //para comprobar la fecha actual
					if((fechaTemporal.getTime()-miliSegundosUltimaConexion)>tiempoMaximoConexion){ //para el tiempo limite
							canalRecibirDatos.close();
							objetivo.close();
					}
				
					if(bf.ready()){ //cuando haya una frase para leer
						lineaentrada=bf.readLine();
						DatosEntrada.add(lineaentrada);
						if(lineaentrada.equals(""))
							break;
			 		}
				}
				
				if(DatosEntrada.get(0).equalsIgnoreCase("")) continue;
			
				String[] temporalAdivinar;
			
				//Leemos la cabecera y la añadimos a nuestras variables.
				for(int i=0;i<DatosEntrada.size();i++){
					temporalAdivinar=DatosEntrada.get(i).split(" ");
					if(i==0)	
						GET=DatosEntrada.get(i);
					
					if(temporalAdivinar[0].startsWith("if-Modified-Since")){
						modificadaDesde=DatosEntrada.get(i).split(":", 2)[1];
						modificadaDesde=modificadaDesde.split(" ", 2)[1]; //hago esto para quitarle el espacio del principio
					}
					
					if(temporalAdivinar[0].equals("Connection:"))
						conexionAlive=DatosEntrada.get(i);
					
					if(temporalAdivinar[0].equals("Accept-Encoding:"))
						codificaciones=temporalAdivinar[1].split(",");
					
				}
			
				
					if(codificaciones != null){
						for(String s : codificaciones){
							if(s.equals("deflate"))//compruebo si acepta deflate
								codifDeflate=true;
						}
					}
				String[] primeraFraseSplit=null;
				if(GET!=null) //si la orden en vez de ser get es otra cosa GET ser�a null por lo que dar�a una exception
					primeraFraseSplit=GET.split(" "); //separo la primera linea por palabras
								
				
				if(primeraFraseSplit==null || (!primeraFraseSplit[0].equals("GET") && !primeraFraseSplit[0].equals("PUT") && !primeraFraseSplit[0].equals("DELETE") && !primeraFraseSplit[0].equals("HEAD") && !primeraFraseSplit[0].equals("POST"))){
					matarASarahConnor("/commandError2.html","HTTP/1.0",objetivo);
					canalRecibirDatos.close();
					objetivo.close();
					break;	
					}
				else if(!primeraFraseSplit[0].equals("GET")) {
					matarASarahConnor("/commandError.html","HTTP/1.0",objetivo);
					canalRecibirDatos.close();
					objetivo.close();
					break;
				}

				//Compruebo si es persistente
				if((primeraFraseSplit[2].equals("HTTP/1.1") && (conexionAlive!=null && conexionAlive.equals("Connection: close"))) || ((conexionAlive==null || !conexionAlive.equals(("Connection: keep-alive"))) && primeraFraseSplit[2].equals("HTTP/1.0")))
					persistente=false;
				else
					persistente=true;
				
				if(primeraFraseSplit[2].equals("HTTP/1.1") || primeraFraseSplit[2].equals("HTTP/1.0")) // si es version http1.0 o 1.1
					matarASarahConnor(primeraFraseSplit[1], primeraFraseSplit[2],objetivo); //funcion para mandar el fichero
				else {//si no error http no
					matarASarahConnor("/versionError.html","HTTP/1.0",objetivo);
					canalRecibirDatos.close();
					objetivo.close();
					break;
				}
			
				
				if(!persistente){
					canalRecibirDatos.close();
					objetivo.close();
					break;
				}else{
					fechaTemporal= new Date();
					miliSegundosUltimaConexion=fechaTemporal.getTime();
				}
			}
		} catch (Exception e) {
			e.getMessage();
		}
		
	
	}
	public  void matarASarahConnor(String fichero, String versionHttp, Socket objetivo){
		try{
			//Definimos las variables con las cadenas de html de los distintos error.
			//En algunos casos si encuentra el archivo de error, utilizará este, en caso contrario usará la cadena de html.
			String htmlError1="<html><head>\n<title>ERROR 501 Not Implemented</title>\n</head>\n<body>\n<div align='left'><h1>ERROR 501 &nbsp;&nbsp;Not Implemented</h1><p>Orden no implementada, pruebe con otra<p></div>\n<hr>\n<address>Skynet Server/1.0</address>\n</body></html>";
			String htmlError2="<html><head>\n<title>ERROR 400 (Bad Request)</title>\n</head>\n<body>\n<div align='left'><h1>ERROR 400 &nbsp;&nbsp;Bad Request</h1><p>Orden no conocida, pruebe con otra<p></div>\n<hr>\n<address>Skynet Server/1.0</address>\n</body></html>";
			String htmlError3="<html><head><title>ERROR 505</title></head>\n<body>\n<div align='left'><h1>ERROR 505 &nbsp;&nbsp;HTTP Version Not Supported</h1><p>Por ahora no soportamos tu versión Http<p></div>\n<hr>\n<address>Skynet Server/1.0</address>\n</body></html>";
			String htmlError4="<html><head><title>ERROR 404 Not Found</title></head>\n<body>\n<div align='left'><h1>ERROR 404 &nbsp;&nbsp;Not Found</h1><p>El archivo que estas buscando no existe.<p></div>\n<hr>\n<address>Skynet Server/1.0</address>\n</body></html>";
			String htmlError5="<html><head><title>ERROR 403 Forbidden</title></head>\n<body>\n<div align='left'><h1>Error 403 &nbsp;&nbsp;Forbidden</h1><p>Los humanos no tienen permisos para acceder aqui.<p></div>\n<hr>\n<address>Skynet Server/1.0</address>\n</body></html>";			
			String file="";
			if(fichero.equals("/")) file="/index.html";
			else file=fichero;
			File archivo = new File (DefaultDir+file);
			FileInputStream archivoEnvio;
			if(canalMandarDatos==null)
			canalMandarDatos = objetivo.getOutputStream();
			PrintStream salida = new PrintStream(canalMandarDatos);
			
			//Realizamos diferentes acciones dependiendo del error.
			 if (fichero.equals("/commandError.html")) //error de instrucción no implementada
			  {
				 cabeceraRespuesta=versionHttp+" 501 Not Implemented";
				 persistente=false;
				 cabeceraRespuesta+=cabecerasimple();
				 cabeceraRespuesta+="\nContent-Length: "+htmlError1.length()+"\nContent-Type: text/html";
				 salida.println(cabeceraRespuesta);
				 salida.println();
				 salida.print(htmlError1);
				 salida.print("\n\n");
				 objetivo.close();
				 return;
			  }
			 
			 if (fichero.equals("/commandError2.html")) //error de instrucción no conocida.
			  {
				 cabeceraRespuesta=versionHttp+" 400 Bad Request";
				 persistente=false;
				 cabeceraRespuesta+=cabecerasimple();
				 cabeceraRespuesta+="\nContent-Length: "+htmlError2.length()+"\nContent-Type: text/html";
				 salida.println(cabeceraRespuesta);
				 salida.println();
				 salida.print(htmlError2);
				 salida.print("\n\n");
				 objetivo.close();
				 return;
			  }
			  
			 if (fichero.equals("/versionError.html")) //error de no version apropiada
			  {
				 cabeceraRespuesta=versionHttp+" 505 HTTP Version Not Supported";
				 persistente=false;
				 cabeceraRespuesta+=cabecerasimple();
				 cabeceraRespuesta+="\nContent-Length: "+htmlError3.length()+"\nContent-Type: text/html";
				 salida.println(cabeceraRespuesta);
				 salida.println();
				 salida.print(htmlError3);
				 salida.print("\n\n");
				 objetivo.close();
				 return;
			  }
			
			
			if(!archivo.exists()) //aqui empieza el abrir el fichero comprobar que existe etc...
			 {
				cabeceraRespuesta= versionHttp+" 404 Not Found";
				archivo=new File(DefaultDir+"/Not found.html");
				persistente=false;
				if(!archivo.exists())
				 {
					cabeceraRespuesta+=cabecerasimple();
					cabeceraRespuesta+="\nContent-Length: "+htmlError4.length()+"\nContent-Type: text/html";
					salida.println(cabeceraRespuesta);
					salida.println();
					salida.print(htmlError4);
					salida.print("\n\n");
					objetivo.close();
					return;
				 }
				else archivoEnvio = new FileInputStream(archivo);
			 }
			else
			 {
				if(!archivo.canRead() || archivo.isDirectory())
				 {
					cabeceraRespuesta= versionHttp+" 403 Forbidden";
					archivo=new File(DefaultDir+"/Forbidden.html");
					persistente=false;
					if(!archivo.exists())
					 {
						cabeceraRespuesta+=cabecerasimple();
						cabeceraRespuesta+="\nContent-Length: "+htmlError5.length()+"\nContent-Type: text/html";
						salida.println(cabeceraRespuesta);
						salida.println();
						salida.print(htmlError5);
						salida.print("\n\n");
						objetivo.close();
						return;
					 }
					else archivoEnvio = new FileInputStream(archivo);
				 }
				else
				 {
					archivoEnvio = new FileInputStream(archivo);
					cabeceraRespuesta= versionHttp+" 200 OK";
				 }
			 }
		
			 if(modificadaDesde!=null){ //si me manda la cabecera de que lo tiene guardado en la cach�
				Date Date1=formatearFecha.parse(modificadaDesde);
			 	Date Date2=formatearFecha.parse(formatearFecha.format(archivo.lastModified()));
			 	if(Date1.after(Date2)){ //si la fecha que me indica es mayor que la del fichero
				 	cabeceraRespuesta="HTTP 304 Not Modified"; //le devuelvo el HTTP 304
				 	persistente=false;
				 	cabeceraRespuesta+=cabecerasimple();
				 	salida.println(cabeceraRespuesta);
				 	salida.println();
				 	archivoEnvio.close();
				 	return;
			 	}
			 }
			 
			 formatearFecha.setTimeZone(TimeZone.getTimeZone("GMT"));//hago que la zona horaria a mostrar sea GMT
			 cabeceraRespuesta+=cabecerasimple();//añado una cabezera simple a la respuesta

			 
			 if((!archivo.exists() || !archivo.canRead())){ //solo si alguien borra algo que no debe del directorio es mera comprobacion
				 salida.println("Error inesperado");
				 archivoEnvio.close();	
				 return;
			 }
			 
			 cabeceraRespuesta+="\nLast-Modified: "+formatearFecha.format(archivo.lastModified());//formateo la fecha y la pongo en la cabecera
			 cabeceraRespuesta+="\nContent-Type: "+Files.probeContentType(archivo.toPath()); //el tipo mime
			 

			 if(!(Files.probeContentType(archivo.toPath()).startsWith("text") && codifDeflate)){ //si no comprimo hago lo de siempre
				cabeceraRespuesta+="\nContent-Length: "+archivo.length(); //la longitud
			 	salida.println(cabeceraRespuesta); //la imprimo por el canal de salida
			 	salida.println();
			 	
				byte[] buffer = new byte[1024]; 
				int bytes;
			 	
			 	while((bytes = archivoEnvio.read(buffer)) != -1 ){
				 canalMandarDatos.write(buffer, 0, bytes);
			 	}
			 }else { //comprimimos con deflate
			 	cabeceraRespuesta+="\nContent-Encoding: deflate"; //indico al navegador que est� codificado en deflate 
			 	cabeceraRespuesta+="\nTransfer-Encoding: chunked"; 
			 	salida.println(cabeceraRespuesta); //la imprimo por el canal de salida
			 	salida.println(); //para separar la cabecera http del objeto
			 	DeflaterInputStream din = new DeflaterInputStream(archivoEnvio);
				codify(din,salida,canalMandarDatos); //envio el archivo deflated
			 }
			 archivoEnvio.close();
		}catch (Exception e){
		}
	}
	
	//Comprimimos y enviamos los chunks al mismo tiempo
	void codify(DeflaterInputStream din,PrintStream ps,OutputStream ous){
		byte[] bytesBuffer = new byte[100];
		try {
			int tam;
			while((tam=din.read(bytesBuffer))!= -1){
				ps.print(Integer.toHexString(tam));
				ps.print("\n");
				ous.write(bytesBuffer,0,tam);
				bytesBuffer= new byte[100];
				ps.print("\n");
			}
			ps.print("0");
			ps.print("\n");
			ps.println();
			
		} catch (IOException e) {
			System.out.println("Catch de escritura deflate");
			e.printStackTrace();
		}
		
	}
	
	public String cabecerasimple()//Generamos una cabecera simple
	 { 
		 String datos;
		 Date Fecha_actual= new Date();
		 datos= "\nDate: "+formatearFecha.format(Fecha_actual);
		 datos+="\nServer: "+ nameServer;
		 if(persistente)
		  {
			 datos+="\nConnection: keep-alive";
			 datos+="\nKeep-Alive: timeout="+tiempoMaximoConexion;
		  }
		 else
			 datos+="\nConnection: close";
		 return datos;
	 }
}

public class ServidoHttp51 {
	
	static ServerSocket puertoActivo;
	static Socket conexion;
	static int numThreads;
	static int port;
	static int defaultport=10051;
	static String dir;
	static String defaultdir="www";
	
	public static void main(String[] Args){
		if(Args.length==0)
		 {
			port=defaultport;
			dir=defaultdir;
		 }
		else
		 {
			if(Args.length==1)
			 {
				port=Integer.parseInt(Args[0]);
				dir=defaultdir;
			 }
			else 
			 {
				port= Integer.parseInt(Args[0]);
				dir=Args[1];
			 }
		 }
		
		try{
			puertoActivo=new ServerSocket(port);
				while(true)
				 {
					if((conexion=puertoActivo.accept())!=null){
						System.out.println("Nuevo terminator "+numThreads);
						new Terminator(conexion,"Terminator numero: "+numThreads, dir);
						numThreads++;
					}
				 }
		}
		catch (Exception e){
			System.out.println("EL PUERTO EST� OCUPADO");
			System.exit(0);
		}
	}
}