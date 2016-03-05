import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import javax.xml.parsers.*;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;
import javax.xml.xpath.XPathConstants;
import java.net.*;
import org.w3c.dom.*;
import org.xml.sax.*;
import java.util.*;
import org.xml.sax.helpers.DefaultHandler;

class xml_error_handler extends DefaultHandler{
	Sint85P2 ser;
	Boolean trigger = false;
	String error = "";
	public xml_error_handler(){}
	public xml_error_handler(Sint85P2 ser){
		ser = ser;
	}
	@Override
	public void warning(SAXParseException spe){
		ser.error += "Warning: " + spe.toString()+ "</br></br>";
	}
	@Override
	public void error(SAXParseException spe){
		trigger = true;
                error = "Error: " + spe.toString()+ "</br></br>";
	}
	@Override
	public void fatalError(SAXParseException spe){
		trigger = true;
                error += "FatalError: " + spe.toString()+ "</br></br>";
	}
}

class entRes implements EntityResolver{
	ServletConfig contMio;
	Sint85P2 s;
	public entRes(ServletConfig config){
		contMio = config;
	}

	public entRes(ServletConfig config,Sint85P2 p){
		s = p;
		contMio = config;
	}
	public entRes(){}
	@Override
 	public InputSource resolveEntity(String publicId, String systemId) throws SAXException, IOException {
		 if (systemId.contains("iml.dtd")){ // Nombre del DTD
			return new InputSource(new FileReader(contMio.getServletContext().getRealPath("iml.dtd"))); // Ruta hacia el DTD desde el contexto
 		 }else{
			if(s != null)
				s.error += "NO SE ENCUENTRA EL DTD </br>";
		 	return null;
		}
 	}
}

public class Sint85P2 extends HttpServlet{
	int tipoConsulta = 0;
	HashMap<String, Document> lista;
	String error = "";
	XPath xpath;
	String expresionArtista ="Interprete/Nombre/NombreG | Interprete/Nombre/NombreC";
	String url = "http://clave.det.uvigo.es:8080/~sintprof/15-16/p2/";
	@Override
	public void init(ServletConfig config){
		lista = new HashMap<String, Document>();
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		try{
			factory.setValidating(true);
			xpath = XPathFactory.newInstance().newXPath();
			DocumentBuilder builder = factory.newDocumentBuilder();
			xml_error_handler xeh = new xml_error_handler(this);
			builder.setErrorHandler(xeh);
			builder.setEntityResolver(new entRes(config,this));
			ArrayList <String> listaPorLeer = new ArrayList<String>();
			listaPorLeer.add("sabina.xml");//establezco el primero a leer
			for(int i = 0; i < listaPorLeer.size() ; i ++){//voy recorriendo la lista por leer
				String file = null;
				if(listaPorLeer.get(i).startsWith("http://"))
					file = listaPorLeer.get(i);
				else
					file = (url + listaPorLeer.get(i));
				if(!existe(file)){
					error += "NOT FOUND: " + file + "</br>";
					continue;
				}
				Document dom = parsear(builder,file);//leo el documento a arbol dom
				if(xeh.trigger){
					error += xeh.error;
					xeh.trigger = false;
					continue;
				}
				lista.put(dom.getElementsByTagName("Id").item(0).getFirstChild().getNodeValue(),dom);//lo meto en el hash
				NodeList nl = dom.getElementsByTagName("IML");//cojo sus IMLs
				for(int j = 0; j < nl.getLength() ; j ++){//busco en eses IMLs
					if(!listaPorLeer.contains(nl.item(j).getFirstChild().getNodeValue()))//si no está ya como por leer
						listaPorLeer.add(nl.item(j).getFirstChild().getNodeValue());//lo añado en por leer
				}
			}
		}catch(Exception e){
			//error = e.toString();
		}
	}
	boolean existe(String file){
		try{
			HttpURLConnection.setFollowRedirects(false);
			HttpURLConnection con = (HttpURLConnection) new URL(file).openConnection();
			con.setRequestMethod("HEAD");
			return (con.getResponseCode() == HttpURLConnection.HTTP_OK);
		}catch(Exception e){
			error += "NOT FOUND: " + file;
			return false;
		}
	}

	public Document parsear(DocumentBuilder builder, String file){
		try{
			Document dom = builder.parse(file);
			return dom;
		}catch(Exception e){
			return null;
		}
	}
	public void doGet(HttpServletRequest req, HttpServletResponse res) throws IOException, ServletException{
		res.setContentType("text.html");
		res.setCharacterEncoding("ISO-8859-1");
		req.setCharacterEncoding("ISO-8859-1");
		PrintWriter out = res.getWriter();
		cabecera(out);
		out.println("<body>");
		out.println("<div id='TODO'>");
		out.println("<h1>Music All</h1>");
		out.println("<hr />");
		if(req.getParameter("fase") == null)
			paginaPrincipal(out,req);
		else
			decidirPagina(out,req);
		out.println("</div>");
		pieDePagina(out);
		out.println("</body>");
		out.println("</html");
	}

	void paginaPrincipal(PrintWriter o,HttpServletRequest req){
		o.println("<p style='color:red'>"+error+"</p>");
		o.println("<a href = '?fase=1'>Consultar por artista/album</a>");
		o.println("</br>");
		o.println("<a href = '?fase=2'>Consulta por año/album/estilo</a>");
	}
	void decidirPagina(PrintWriter out,HttpServletRequest req){
		String s = req.getParameter("fase");
		if(s.equals("1")){
			tipoConsulta = 1;
			consulta1(out,req);
		}
                if(s.equals("2")){
			tipoConsulta = 2;
                        consulta2(out,req);
		}
	}

	void consulta1(PrintWriter out,HttpServletRequest req){
		if(req.getParameter("artista") == null){
			out.println("<h3>Escoge artista</h3>");
			out.println("</br>");
			out.println("<ul>");
			ArrayList<String> nombresArtistas = new ArrayList<String>();
			String hrefArtista = "href = '" + req.getRequestURL() + "?fase=1&artista=";
			for(String key : lista.keySet()){
				Document dom = lista.get(key);
				try{
					nombresArtistas.add(xpath.compile(expresionArtista).evaluate(dom));
				}
				catch(Exception e){
					e.toString();
				}
                	}
			Collections.sort(nombresArtistas);
			for(String artista : nombresArtistas){
                        	out.println("<li><a " + hrefArtista + artista + "'> "+artista+"</a></li>");
				out.println("</br>");
			}
			if(nombresArtistas.size() > 0){
				out.println("<li><a " + hrefArtista + "Todos'>Todos los artistas </a></li>");
				out.println("</br>");
			}
			out.println("<ul>");
			imprimirVolver(out,req);
		}else
			consultaAlbum(out,req);
	}
	/*************************Funcion de imprimir las canciones en la consulta 1
	******************************************************************************/
	void consulta1Canciones(PrintWriter out, HttpServletRequest req){
		imprimirParametros(req,out);
		ArrayList<String> al = new ArrayList<String>();
		for(String key : lista.keySet()){//voy sacando todas las key
			Document dom;//declaracion para multiuso segun if
			if(!req.getParameter("artista").equals("Todos"))//si no es todos
				dom = getArtista(req.getParameter("artista"));//lo saco
			else//si es todos
				dom = lista.get(key);//voy sacando
			String a = req.getParameter("album");//saco el album
			a = a.trim();
			String expr = "";//expresion para multi uso
			if(!a.equals("Todos"))//si no es todos los albumes
				expr = "//Album[NombreA = '"+a+"']/Cancion";//expresion de busco album en concreto sus canciones
			else
				expr = "//Album/Cancion";//expresion de cualquier album busco canciones
                	try{
                		NodeList nl = (NodeList)xpath.evaluate(expr,dom,XPathConstants.NODESET);//saco los nombres de canciones
				for(int i = 0;i < nl.getLength();i++){//las voy añadiendo
                        	        String nombre = xpath.evaluate("NombreT",nl.item(i));
                        	        String duracion = " (Duracion "+xpath.evaluate("Duracion",nl.item(i));
					String descripcion = ") " + xpath.evaluate("text()[normalize-space()]",nl.item(i));
					String s = nombre + duracion + descripcion;
					        if(!al.contains(s))
                        	                        al.add(s);
                        	}
                	}catch(Exception e){}
			if(!req.getParameter("artista").equals("Todos"))//parche, si no es todos solo ejecuto el bucle
				break; //una vez
		}
		Collections.sort(al);
                for(String s : al){
                        out.println("<li>" + s+"</li>");
                	out.println("</br>");
                }
		imprimirVolver(out,req);
	}

	Document getArtista(String art){
		Document dom = null;
                for(String key : lista.keySet()){//rebusco por los doms
                        dom = lista.get(key);
                        try{
                                String arti = xpath.compile(expresionArtista).evaluate(dom);
                                if(arti.equals(art))//si es el que busco
                                        break;
                        }
                        catch(Exception e){}
		}
		return dom;
	}
	void consulta2(PrintWriter out,HttpServletRequest req){
		if(req.getParameter("year") != null)
			consultaAlbum(out, req);
		else{
                        out.println("<h3>Escoge año</h3>");
                        out.println("</br>");
			out.println("<ul>");
			String href = "href = '" + req.getRequestURL() +"?"+ req.getQueryString() + "&year=";
			ArrayList<String> anos = getStringOfAllAlbums("//Album/Año",null);
			Collections.sort(anos);
			for(String si : anos){
                        	out.println("<li><a " + href + si + "'>"+si+"</a></li>");
				out.println("</br>");
			}
			if(anos.size() > 0)
				out.println("<li><a " + href + "Todos'>Todos los años</a></li>");
			out.println("</ul>");
			imprimirVolver(out,req);
		}
	}

	ArrayList<String> getStringOfAllAlbums(String expr,Document dom1){//si le envio un dom me busca solo la expresion de ese dom
		ArrayList<String> al = new ArrayList<String>();
		if(dom1 == null){
			for(String key : lista.keySet()){//busco en todos los doms
                		Document dom = lista.get(key);
                		try{
                			NodeList nl = (NodeList)xpath.compile(expr).evaluate(dom,XPathConstants.NODESET);
                			for(int i = 0;i < nl.getLength();i++){
                				String s = nl.item(i).getFirstChild().getNodeValue();
						if(!al.contains(s))
							al.add(s);
					}
                		}catch(Exception e){}
                	}
		}else{
			try{
                	NodeList nl = (NodeList)xpath.compile(expr).evaluate(dom1,XPathConstants.NODESET);
                	for(int i = 0;i < nl.getLength();i++){
                        	String s = nl.item(i).getFirstChild().getNodeValue();
                                	if(!al.contains(s))
                                        	al.add(s);
                	}
			}catch(Exception e){}
		}
		return al;
	}
        void consultaAlbum(PrintWriter out,HttpServletRequest req){
                if(req.getParameter("album") != null){
			if(tipoConsulta == 2)
				consulta2Estilo(out,req);
			if(tipoConsulta == 1)
				consulta1Canciones(out,req);
		}
                else{
			imprimirParametros(req,out);
                        out.println("<h3>Escoge album</h3>");
                        out.println("</br>");
			if(tipoConsulta == 2)
				imprimirAlbumes(out,req,req.getParameter("year"),"NO");
                        if(tipoConsulta == 1)
                                imprimirAlbumes(out,req,"NO",req.getParameter("artista"));
			imprimirVolver(out,req);
                }
        }
        void consulta2Estilo(PrintWriter out,HttpServletRequest req){
                if(req.getParameter("estilo") != null)
                        consulta2canciones(out,req);
                else{
			imprimirParametros(req,out);
                        out.println("<h3>Escoge estilo</h3>");
        	        out.println("</br>");
                        String href = "href = '" + req.getRequestURL() +"?"+ req.getQueryString() + "&estilo=";
			out.println("<ul>");
			ArrayList<String> estilos = new ArrayList<String>();
			if(!req.getParameter("album").equals("Todos")){//si no es todos los albumes
				Node album = getAlbum(req.getParameter("album"));//cojo el album que me indica
				if(album != null)
					estilos = getEstilos(album);//y sus estilos
			}else{
				estilos = getAllStylesOfYearAlbum(req.getParameter("year"));
			}
			Collections.sort(estilos);
                        for(String es : estilos){
                                out.println("<li><a " + href + es + "'>"+es+"</a></li>");
				out.println("</br>");
                        }
			if(estilos.size() > 0)
				out.println("<li><a " + href+ "Todos'>Todos los estilos </a></li>");
			out.println("</ul>");
			imprimirVolver(out,req);
                }
        }

	ArrayList<String> getAllStylesOfYearAlbum(String ano){
		ArrayList<String> as = new ArrayList<String>();
		String expr = "";
		if(!ano.equals("Todos"))
			expr = "//Album[Año ='"+ ano + "']";//con esto cojo todos los albumes del año que me mandan
		else
			expr = "//Album";
		for(String key : lista.keySet()){//recorro todos los documents
			Document dom = lista.get(key);
			try{
				NodeList nl = (NodeList)xpath.compile(expr).evaluate(dom,XPathConstants.NODESET);
				for(int i = 0; i < nl.getLength();i++){//recorro todos los albumes
					NodeList nl2 = nl.item(i).getChildNodes();
					for(int j = 0; j < nl2.getLength();j++){//recorro todas las canciones
						if(!(nl2.item(j) instanceof Element))
							continue;
		                	        Element el = (Element) nl2.item(j);//si lo es lo downcasteo
                			        String s = el.getAttribute("estilo");//consigo su atributo estilo
                        			if(s != null && !s.equals("")){//si no es null o ""
                                			if(!as.contains(s))//si aun no está, lo introduzco
                                	        		as.add(s);
                        			}
					}
				}
			}catch(Exception e){}
		}
		return as;
	}
        int countAllStylesOfYearAlbum(String year,String album,String style){
		int contador = 0;
                String expr = "";
                if(!year.equals("Todos"))//si no es todos los años
                        expr = "//Album[Año ='"+ year + "']";//con esto cojo todos los albumes del año que me mandan
                else
                        expr = "//Album";
		if(!album.equals("Todos"))//si es un album en concreto
			expr += "[NombreA ='"+album+"']";

                for(String key : lista.keySet()){//recorro todos los documents
                        Document dom = lista.get(key);
                        try{
                                NodeList nl = (NodeList)xpath.compile(expr).evaluate(dom,XPathConstants.NODESET);
                                for(int i = 0; i < nl.getLength();i++){//recorro todos los albumes
                                        NodeList nl2 = nl.item(i).getChildNodes();
                                        for(int j = 0; j < nl2.getLength();j++){//recorro todas las canciones
                                                if(!(nl2.item(j) instanceof Element))
                                                        continue;
                                                Element el = (Element) nl2.item(j);//si lo es lo downcasteo
                                                String s = el.getAttribute("estilo");//consigo su atributo estilo
                                                if(s != null && (s.equals(style) || (!s.equals("") && style.equals("Todos"))))//si es el estilo que busco o busco todos
                                                	contador++;
                                        }
                                }
                        }catch(Exception e){}
                }
                return contador;
        }

	Node getAlbum(String album){//busco un album en concreto entre todos los documents a partir de su nombre
		String expr = "//Album[NombreA = '"+album+"']";//expresion de xpath de busco solo el album q tenga nombreA como album
		try{
			for(String key: lista.keySet()){//lo busco
				Document dom = lista.get(key);
				NodeList nl = (NodeList) xpath.compile(expr).evaluate(dom,XPathConstants.NODESET);
				if(nl != null && nl.getLength() != 0)//si encuentro algo que me devuelva un valor
					return nl.item(0);//va a ser el
			}
		}catch(Exception e){}
		return null;
	}
	ArrayList<String> getEstilos(Node album){//a partir de un nodo album consigo todos los estilos
		ArrayList<String> al = new ArrayList<String>();
		NodeList nl = album.getChildNodes();
		for(int i = 0; i < nl.getLength();i++){
			if(!(nl.item(i) instanceof Element))//si no es un element
				continue;//paso
			Element el = (Element) nl.item(i);//si lo es lo downcasteo
			String s = el.getAttribute("estilo");//consigo su atributo estilo
			if(s != null && !s.equals("")){//si no es null o ""
				if(!al.contains(s))//si aun no está, lo introduzco
					al.add(s);
			}
		}
		return al;
	}
	void consulta2canciones(PrintWriter out, HttpServletRequest req){
		imprimirParametros(req,out);
		int cuenta = -1;
		if(!req.getParameter("album").equals("Todos")){//el caso de que el album esté determinado
			Node album = getAlbum(req.getParameter("album"));
			String estilo = req.getParameter("estilo");
			cuenta = contarEstilo(estilo,album);
		}else
			cuenta = countAllStylesOfYearAlbum(req.getParameter("year"),req.getParameter("album"),req.getParameter("estilo"));
		if(cuenta == 1)
			out.println("Hay "+cuenta+" cancion");
		if(cuenta != 1 && cuenta != 0)
			out.println("Hay "+cuenta+" canciones");
		if(cuenta == 0)
			out.println("No hay ninguna canción del estilo elegido");//imposible que pase
		out.println("</br>");
		imprimirVolver(out,req);
	}
	int contarEstilo(String estilo,Node album){
		int contador = 0;
		NodeList nl = album.getChildNodes();
                for(int i = 0; i < nl.getLength();i++){
                        if(!(nl.item(i) instanceof Element))//si no es un element
                                continue;//paso
                        Element el = (Element) nl.item(i);//si lo es lo downcasteo
                        String s = el.getAttribute("estilo");//consigo su atributo estilo
                        if(s != null && (s.equals(estilo) || (!s.equals("") && estilo.equals("Todos"))))//si no es null y vale el estilo que le mando
				contador++;//aumento en uno el contador
                }
		return contador;//para debuggear
	}

	void cabecera (PrintWriter o) throws IOException{
		o.println("<html>");
		o.println("<head>");
		o.println("<meta charset='ISO-8859-1'>");
		o.println("<title>IM2 </title>");
		o.println("<link rel='stylesheet' href='iml.css'>");
		o.println("</head>");
	}
	void pieDePagina(PrintWriter out){
		out.println("<div id='PieDePagina'>");
		out.println("Carlos Daniel Garrido Suárez -- SINT85 -- 2015");
		out.println("</div>");
	}
	void imprimirVolver(PrintWriter out, HttpServletRequest req){
		StringBuffer linkBasico = req.getRequestURL();
		String QueryString = req.getQueryString();
		String[] temp = QueryString.split("&");
		String dirFinal = linkBasico.toString() + "?";
		for(int i = 0; i < (temp.length - 1);i++){
			if(i != 0)
				dirFinal +="&";
			dirFinal +=temp[i];
		}
		out.println("<a href = '"+dirFinal + "'>Pulsa aquí para volver</a>");
                out.println("<a href = '"+linkBasico.toString() + "'>Pulsa aquí para reiniciar</a>");
	}
/***************es para imprimir albumes segun año o segun artista DONE
******************************************************************/
	void imprimirAlbumes(PrintWriter out, HttpServletRequest req,String ano,String art){
		ArrayList<String> albumes = new ArrayList<String>();
		ArrayList<String> nombreAlbumes = new ArrayList<String>();
		String expresionAlbumes = "//Album/NombreA";
                String href = "href = '" + req.getRequestURL() +"?"+ req.getQueryString() + "&album=";

		if(ano.equals("NO")){//imprimo albumes por artistas
			ArrayList<Document> documentos = new ArrayList<Document>();
                        for(String key : lista.keySet()){//rebusco por los doms
                                Document dom = lista.get(key);
                                try{
					expresionAlbumes = "//Album";
                                        String arti = xpath.compile(expresionArtista).evaluate(dom);//cojo nombre de interprete
					if(arti.equals(art) || art.equals("Todos")){//si es el que busco o busco todos
						documentos.add(dom);//lo añado a un arraylist donde imprimo los albumes
                                	}
				}
                                catch(Exception e){
                                        e.toString();
                                }
                        }
			for(Document dom : documentos){//en los documentos elegidos (1 salvo caso de todos)
                                try{
                                        NodeList nl = (NodeList)xpath.compile(expresionAlbumes).evaluate(dom,XPathConstants.NODESET);//añado el nombre de album a imprimir
					for(int i = 0; i < nl.getLength();i++){
						String temp = xpath.evaluate("Año",nl.item(i)) + ",# " + xpath.evaluate("NombreA",nl.item(i));
						nombreAlbumes.add(temp);
					}
				}
                                catch(Exception e){
                                        e.toString();
                                }

			}
			Collections.sort(nombreAlbumes);
			ArrayList<String> alT = new ArrayList<String>();
			for(String s : nombreAlbumes)
				alT.add(s.split(",#")[1]);
			nombreAlbumes = alT;
		}
		if(art.equals("NO")){//imprimo albumes por ano
			if(!ano.equals("Todos"))
				expresionAlbumes = "//Album[Año = "+ ano + "]/NombreA";
			nombreAlbumes = getStringOfAllAlbums(expresionAlbumes,null);
		}
                out.println("<ul>");
                for(String si : nombreAlbumes){
                        out.println("<li><a " + href + si + "'>"+si+"</a></li>");
                        out.println("<br>");
                }
		if(nombreAlbumes.size() > 0)
	                out.println("<li><a " + href+ "Todos'>Todos los albumes </a></li>");
                out.println("</ul>");
	}
	void imprimirParametros(HttpServletRequest req, PrintWriter out){
		String tipoConsulta = req.getParameter("fase");
		String artista = req.getParameter("artista");
		String album = req.getParameter("album");
		String estilo = req.getParameter("estilo");
		String year = req.getParameter("year");
		String finalString = "";
		if(tipoConsulta.equals("1")){
			if(artista != null)
				finalString += "Artista: " + artista + " ";
			if(album != null)
				finalString += "Album: " + album + " ";
		}else{
                        if(year != null)
                                finalString += "Año: " + year + " ";
                        if(album != null)
                                finalString += "Album: " + album + " ";
                        if(estilo != null)
                                finalString += "Estilo: " + estilo + " ";
		}
		out.println("<h3>" + finalString + "</h3>");
	}
}
