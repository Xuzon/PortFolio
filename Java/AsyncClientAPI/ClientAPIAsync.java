package noticia;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.axiom.om.OMAbstractFactory;
import org.apache.axiom.om.OMElement;
import org.apache.axiom.om.OMFactory;
import org.apache.axiom.om.OMNamespace;
import org.apache.axis2.AxisFault;
import org.apache.axis2.addressing.EndpointReference;
import org.apache.axis2.client.Options;
import org.apache.axis2.client.ServiceClient;
import org.apache.log4j.varia.NullAppender;

public class ClientAPIAsync {

	public static void main(String[] args) throws IOException{
		org.apache.log4j.BasicConfigurator.configure(new NullAppender());
		BufferedReader br = new BufferedReader( new InputStreamReader(System.in));
		ServiceClient sc = new ServiceClient();
		Options opts = new Options(); //create options
		opts.setTo(new EndpointReference( "http://localhost:8080/axis2/services/Noticia")); //setting target EPR
		sc.setOptions(opts);//set options
		boolean salir = false;
		int opcion = -1;
		String titular;
		String descripcion;
		String url;
		while(!salir){
			System.out.println("ClientAPI Async");
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
					setNoticia(sc,titular,descripcion,url,opts);
					break;
				case 2:System.out.println(getNoticia(sc,opts));
					break;
				case 3:
					salir = true;
					break;
			}
		}
		br.close();
	}
	
	static void setNoticia(ServiceClient sc,String titulo,String descripcion,String url,Options opts){
		try{
			opts.setAction("urn:SetTitular");
			sc.setOptions(opts);//set options
			sc.sendRobust(setTitular(titulo));
			opts.setAction("urn:SetDescripcion");
			sc.setOptions(opts);//set options
			sc.sendRobust(setDescripcion(descripcion));
			opts.setAction("urn:SetUrl");
			sc.setOptions(opts);//set options
			sc.sendRobust(setUrl(url));
		}catch(AxisFault af){
			af.printStackTrace();
		}
	}
	
	static String getNoticia(ServiceClient sc,Options opts){
		try{
			String titular = "";
			String descripcion = "";
			String url = "";
			
			MyCallback mcT = new MyCallback();
			MyCallback mcD = new MyCallback();
			MyCallback mcU = new MyCallback();
			
			opts.setAction("urn:GetTitular");
			sc.setOptions(opts);//set options
			sc.sendReceiveNonBlocking(getTitular(), mcT);
			while(mcT.espera)
				Thread.sleep(100);
			titular = mcT.elemento;
			
			opts.setAction("urn:GetDescripcion");
			sc.setOptions(opts);//set options
			sc.sendReceiveNonBlocking(getDescripcion(),mcD);
			while(mcD.espera)
				Thread.sleep(100);
			descripcion = mcD.elemento;
			
			opts.setAction("urn:GetUrl");
			sc.setOptions(opts);//set options
			sc.sendReceiveNonBlocking(getUrl(), mcU);
			while(mcU.espera)
				Thread.sleep(100);
			url = mcU.elemento;
			return "Titular: " + titular + " \nDescripción: " + descripcion+" \nURL: " + url;
		}catch(Exception e){
			e.printStackTrace();
			return "ERROR";
		}
	}
	
	public static OMElement setTitular(String valorSetTitular) {
		OMFactory fac = OMAbstractFactory.getOMFactory();
		OMNamespace omNs = fac.createOMNamespace( "http://Noticia", "ns1");
		OMElement method = fac.createOMElement("setTitular", omNs);
		OMElement value = fac.createOMElement("titular", omNs);
		value.setText(valorSetTitular);
		method.addChild(value);
		return method;
	}
	
	public static OMElement getTitular() {
		OMFactory fac = OMAbstractFactory.getOMFactory();
		OMNamespace omNs = fac.createOMNamespace( "http://Noticia", "ns1");
		OMElement method = fac.createOMElement("getTitular", omNs);
		return method;
	}
	
	public static OMElement setDescripcion(String valorSetDescripcion) {
		OMFactory fac = OMAbstractFactory.getOMFactory();
		OMNamespace omNs = fac.createOMNamespace( "http://Noticia", "ns1");
		OMElement method = fac.createOMElement("setDescripcion", omNs);
		OMElement value = fac.createOMElement("descripcion", omNs);
		value.setText(valorSetDescripcion);
		method.addChild(value);
		return method;
	}
	
	public static OMElement getDescripcion() {
		OMFactory fac = OMAbstractFactory.getOMFactory();
		OMNamespace omNs = fac.createOMNamespace( "http://Noticia", "ns1");
		OMElement method = fac.createOMElement("getDescripcion", omNs);
		return method;
	}
	
	public static OMElement setUrl(String valorSetUrl) {
		OMFactory fac = OMAbstractFactory.getOMFactory();
		OMNamespace omNs = fac.createOMNamespace( "http://Noticia", "ns1");
		OMElement method = fac.createOMElement("setUrl", omNs);
		OMElement value = fac.createOMElement("url", omNs);
		value.setText(valorSetUrl);
		method.addChild(value);
		return method;
	}
	
	public static OMElement getUrl() {
		OMFactory fac = OMAbstractFactory.getOMFactory();
		OMNamespace omNs = fac.createOMNamespace( "http://Noticia", "ns1");
		OMElement method = fac.createOMElement("getUrl", omNs);
		return method;
	}
}
