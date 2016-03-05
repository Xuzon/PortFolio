package noticia;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.apache.log4j.varia.NullAppender;

public class Cliente{
	private static boolean salir = false;
	public static void main(String[] args){
		org.apache.log4j.BasicConfigurator.configure(new NullAppender());
		BufferedReader br = new BufferedReader( new InputStreamReader(System.in));
		try{
			NoticiaStub ns = new NoticiaStub();
			//NoticiaStub nsPrueba = new NoticiaStub("http://www.noticiaPrueba.es:8080");
			int opcion = -1;
			String titular;
			String descripcion;
			String url;
			while(!salir){
				System.out.println("Cliente");
				System.out.println("1) Crear noticia, 2) Conseguir noticia, 3) Salir");
				opcion = Integer.parseInt(br.readLine());
				switch(opcion){
					case 1:
						System.out.println("Introduzca titular:\n");
						titular = br.readLine();
						System.out.println("Introduzca descripcion:\n");
						descripcion = br.readLine();
						System.out.println("Introduzca url:\n");
						url = br.readLine();
						setNoticia(ns,titular,descripcion,url);
						break;
					case 2:System.out.println(getNoticia(ns));
						break;
					case 3:
						salir = true;
						break;
				}
			}
			br.close();
		}catch(Exception e){
			e.printStackTrace();
			System.out.println("Hubo un error al crear el stub.. saliendo");
			return;
		}
	}
	
	static void setNoticia(NoticiaStub ns,String titulo,String descripcion,String url){
		NoticiaStub.SetTitular st = new NoticiaStub.SetTitular();
		NoticiaStub.SetDescripcion sd = new NoticiaStub.SetDescripcion();
		NoticiaStub.SetUrl su = new NoticiaStub.SetUrl();
		st.setTitular(titulo);
		sd.setDescripcion(descripcion);
		su.setUrl(url);
		try{
			ns.setTitular(st);
			ns.setDescripcion(sd);
			ns.setUrl(su);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	static String getNoticia(NoticiaStub ns){
		NoticiaStub.GetTitular gt = new NoticiaStub.GetTitular();
		NoticiaStub.GetDescripcion gd = new NoticiaStub.GetDescripcion();
		NoticiaStub.GetUrl gu = new NoticiaStub.GetUrl();
		String titular;
		String descripcion;
		String url;
		try{
			titular = ns.getTitular(gt).get_return();
			descripcion = ns.getDescripcion(gd).get_return();
			url = ns.getUrl(gu).get_return();
			return "Titular: " + titular + " \nDescripción: " + descripcion+" \nURL: " + url;
		}catch(Exception e){
			e.printStackTrace();
			return "ERROR";
		}
	}
}
