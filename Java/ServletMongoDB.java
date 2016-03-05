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
import com.mongodb.*;

public class Sint85P3 extends HttpServlet{
	int tipoConsulta = 0;
	String error = "";
	DBCollection coll;
	ArrayList<String> documentosErroneos = new ArrayList<String>();
	@Override
	public void init(ServletConfig config){
		try{
			MongoClient mongoClient = new MongoClient ("localhost", 27017);
			DB db = mongoClient.getDB("sint85");
			coll = db.getCollection("interpretes");
			DBCursor cursor = coll.find();
			while(cursor.hasNext()){
				DBObject documento = (DBObject) cursor.next();
				if(!comprobacionDocumento(documento)){
					documentosErroneos.add(documento + "");
					error += " en fichero (" + documento +")</br></br>";
				}
			}
		}catch(Exception e){
			error += e.toString();
		}
	}
	Boolean comprobacionDocumento(DBObject dom){
		try{
		DBObject nombre = (DBObject) dom.get("Nombre");
		if(nombre == null){
			error += "Interprete sin nombre";
			return false;
		}
		String temporal = nombre.get("NombreC") + "";//consigo NombreC
                if(temporal.equals("null"))//si es null tendrá nombre G
                	temporal = nombre.get("NombreG") +"";//consigo NombreG
                if(temporal.equals("null") || temporal.equals("")){//si aun así sigue siendo null hay un error poque no hay $
	  		error += "Interprete sin un nombreC o nombreG";
		        return false;
		}
		if(nombre.get("Id") == null || nombre.get("Id").equals("")){
			error += "SIN ID";
			return false;
		}
		if(dom.get("Nacionalidad") == null || dom.get("Nacionalidad").equals("")){
			error += "SIN NACION";
			return false;
		}
		Object o = dom.get("Album");

		if(o instanceof BasicDBList){
			BasicDBList albumes = (BasicDBList) o;
			for(Object ob : albumes){
				DBObject album = (DBObject) ob;
				if(album.get("NombreA") == null || album.get("NombreA").equals("")){
					error += "Album no tiene NombreA";
					return false;
				}
				if(album.get("Tipo") == null || album.get("Tipo").equals("")){
					error += "No tiene tipo";
					return false;
				}
				if(album.get("Año") == null || album.get("Año").equals("")){
					error += "No tiene año";
					return false;
				}
				Object listaCanciones = album.get("Cancion");
				if(listaCanciones instanceof BasicDBList){
					BasicDBList canciones = (BasicDBList) listaCanciones;
					for(Object obc : canciones){
						DBObject cancion = (DBObject) obc;
						if(cancion.get("Estilo") == null || cancion.get("Estilo").equals("")){
							error += "no tiene estilo";
							return false;
						}
						if(cancion.get("NombreT") == null || cancion.get("NombreT").equals("")){
							error += "No tiene TombreT";
							return false;
						}
						if(cancion.get("Duracion") == null || cancion.get("Duracion").equals("")){
							error += "No tiene duración";
							return false;
						}
						if(cancion.get("Version") != null){
							DBObject version = (DBObject)cancion.get("Version");
							if(version.get("Nombre") == null || version.get("Nombre").equals("")){
								error += "La version no tiene nombre";
								return false;
							}
						}
					}
				}else{
					 DBObject cancion = (DBObject) listaCanciones;
                                         if(cancion.get("Estilo") == null || cancion.get("Estilo").equals("")){
                                         	error += "no tiene estilo";
                                                return false;
                                         }
                                         if(cancion.get("NombreT") == null || cancion.get("NombreT").equals("")){
                                                error += "No tiene NombreT";
                                                return false;
                                         }
                                         if(cancion.get("Duracion") == null || cancion.get("Duracion").equals("")){
                                         	error += "No tiene duración";
                                                return false;
                                         }
                                         if(cancion.get("Version") != null){
                                                DBObject version = (DBObject)cancion.get("Version");
                                                if(version.get("Nombre") == null || version.get("Nombre").equals("")){
                                               		error += "La version no tiene nombre";
                                         	        return false;
                                                }
                                         }
				}
			}
		}else{
			DBObject album = (DBObject) o;
			if(album.get("NombreA") == null || album.get("NombreA").equals("")){
				error += "Album no tiene NombreA";
				return false;
			}
			if(album.get("Tipo") == null || album.get("Tipo").equals("")){
				error += "No tiene tipo";
				return false;
			}
			if(album.get("Año") == null || album.get("Año").equals("")){
				error += "No tiene año";
				return false;
			}
			Object listaCanciones = album.get("Cancion");
			if(listaCanciones instanceof BasicDBList){
				BasicDBList canciones = (BasicDBList) listaCanciones;
				for(Object obc : canciones){
					DBObject cancion = (DBObject) obc;
					if(cancion.get("Estilo") == null || cancion.get("Estilo").equals("")){
						error += "no tiene estilo";
						return false;
					}
					if(cancion.get("NombreT") == null || cancion.get("NombreT").equals("")){
						error += "No tiene NombreT";
						return false;
					}
                                        if(cancion.get("Duracion") == null || cancion.get("Duracion").equals("")){
                                                error += "No tiene duración";
                                                return false;
                                        }

					if(cancion.get("Version") != null){
						DBObject version = (DBObject)cancion.get("Version");
						if(version.get("Nombre") == null || version.get("Nombre").equals("")){
							error += "La version no tiene nombre";
							return false;
						}
					}
				}
			}else{
				 DBObject cancion = (DBObject) listaCanciones;
                                 if(cancion.get("Estilo") == null || cancion.get("Estilo").equals("")){
                                     	error += "no tiene estilo";
                                        return false;
                                 }
                                 if(cancion.get("NombreT") == null || cancion.get("NombreT").equals("")){
                                        error += "No tiene NombreT";
                                        return false;
                                 }
                                 if(cancion.get("Duracion") == null || cancion.get("Duracion").equals("")){
                                         error += "No tiene duración";
                                         return false;
                                 }

                                 if(cancion.get("Version") != null){
                                        DBObject version = (DBObject)cancion.get("Version");
                                        if(version.get("Nombre") == null || version.get("Nombre").equals("")){
                                        	error += "La version no tiene nombre";
                                                return false;
                                         }
                                }
			}
		}
		return true;
		}catch(Exception e){
			error += e.toString();
			return false;
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
			DBCursor cursor = coll.find();
			String hrefArtista = "href = '" + req.getRequestURL() + "?fase=1&artista=";;
			while(cursor.hasNext()){//recorro todos los documentos
				DBObject documento = cursor.next();
				if(documentosErroneos.contains(documento + ""))//si el documento tiene un error paso de el
					continue;
				DBObject nombre = (DBObject) documento.get("Nombre");
				String temporal = nombre.get("NombreC") + "";//consigo NombreC
				if(temporal.equals("null"))//si es null tendrá nombre G
					temporal = nombre.get("NombreG") +"";//consigo NombreG
				nombresArtistas.add(temporal);
			}
			cursor.close();
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
		String art = req.getParameter("artista");
		String album = req.getParameter("album");
		ArrayList<String> al = new ArrayList<String>();
		DBCursor cursor = coll.find();
		while(cursor.hasNext()){
			DBObject documento = cursor.next();
			if(documentosErroneos.contains(documento + ""))//si el documento tiene un error paso de el
					continue;
                        DBObject nombre = (DBObject) documento.get("Nombre");
                        String temporal = nombre.get("NombreC") + "";//consigo NombreC
                        if(temporal.equals("null"))//si es null tendrá nombre G
                                temporal = nombre.get("NombreG") +"";//consigo NombreG
			if(temporal.equals(art) || art.equals("Todos")){
				if(documento.get("Album") instanceof BasicDBList){
					BasicDBList albumes = (BasicDBList) documento.get("Album");
					for(Object o : albumes){
						DBObject alb = (DBObject) o;
						String temp2 = alb.get("NombreA") + "";
						if(temp2.equals(album) || (!temp2.equals("null") && album.equals("Todos")))
							al = addCancionesAlbum(al,alb);
					}
				}else{
 					DBObject alb = (DBObject) documento.get("Album");
                                        String temp2 = alb.get("NombreA") + "";
                                        if(temp2.equals(album) || (!temp2.equals("null") && album.equals("Todos")))
                                	        al = addCancionesAlbum(al,alb);
				}

			}
		}
		Collections.sort(al);
                for(String s : al){
                        out.println("<li>" + s+"</li>");
                	out.println("</br>");
                }
		imprimirVolver(out,req);
	}
	ArrayList<String> addCancionesAlbum (ArrayList<String> al,DBObject album){
		if(album.get("Cancion") instanceof BasicDBList){
			BasicDBList canciones = (BasicDBList) album.get("Cancion");
			for(Object o : canciones){
				DBObject dbo = (DBObject) o;
				String nombre = dbo.get("NombreT") + "";
				String duracion = " (Duracion: " + dbo.get("Duracion") + " min) ";
				String descripcion = dbo.get("Descripcion") + "";
				if(descripcion.equals("null"))
					al.add(nombre + duracion);
				else
					al.add(nombre + duracion + descripcion);
			}
		}else{
			 DBObject dbo = (DBObject) album.get("Cancion");
                         String nombre = dbo.get("NombreT") + "";
                         String duracion = " (Duracion: " + dbo.get("Duracion") + " min) ";
                         String descripcion = dbo.get("Descripcion") + "";
                         if(descripcion.equals("null"))
                          	 al.add(nombre + duracion);
                         else
                                 al.add(nombre + duracion + descripcion);
		}
		return al;
	}
	void consulta2(PrintWriter out,HttpServletRequest req){
		if(req.getParameter("year") != null)
			consultaAlbum(out, req);
		else{
                        out.println("<h3>Escoge año</h3>");
                        out.println("</br>");
			out.println("<ul>");
			String href = "href = '" + req.getRequestURL() +"?"+ req.getQueryString() + "&year=";
			ArrayList<String> anos = getAllYears();
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
	ArrayList<String> getAllYears(){
		ArrayList<String> al = new ArrayList<String>();
		DBCursor cursor = coll.find();
		while(cursor.hasNext()){
			DBObject documento = cursor.next();
			if(documentosErroneos.contains(documento + ""))//si el documento tiene un error paso de el
					continue;
			if(documento.get("Album") instanceof BasicDBList){
				BasicDBList lista = (BasicDBList) documento.get("Album");
				for(Object o : lista){
					DBObject dbo = (DBObject) o;
					String temporal = dbo.get("Año") + "";
					if(!temporal.equals("null") && !al.contains(temporal))
						al.add(temporal);
				}
			}else{
				 DBObject dbo = (DBObject) documento.get("Album");
                                 String temporal = dbo.get("Año") + "";
                                 if(!temporal.equals("null") && !al.contains(temporal))
        	                         al.add(temporal);
			}
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
			estilos = getEstilos(req);
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

        int contarEstilo(String year,String album,String style){
		int contador = 0;
		DBCursor cursor = coll.find();
		while(cursor.hasNext()){
			DBObject dom = cursor.next();
			if(documentosErroneos.contains(dom + ""))
				continue;
			Object alb = dom.get("Album");
			if(alb == null)
				continue;
			if(alb instanceof BasicDBList){//si es una lista de albumes
				BasicDBList albumes = (BasicDBList) alb;
				for(Object o : albumes){//recorro album a album
					BasicDBObject albumTemporal = (BasicDBObject) o;//cojo sus años y nombre
					String nombreA =albumTemporal.get("NombreA") + "";
					String yearTemporal = albumTemporal.get("Año") + "";
					if(!(nombreA.equals(album) || album.equals("Todos")))//si no es el nombre que busco y no busco todos
						continue;//voy al siguiente album
					if(!(yearTemporal.equals(year) || year.equals("Todos")))//si pertenece al año que busco o no busco de cualquier año
						continue;//voy al siguiente album
					Object listaCanciones = albumTemporal.get("Cancion");//consigo lista de canciones
					if(listaCanciones == null)//si no hay canciones
						continue;//siguiente album
					if(listaCanciones instanceof BasicDBList){//si es una lista de canciones
						BasicDBList canciones = (BasicDBList) listaCanciones;
						for(Object ob : canciones){//recorro las canciones
							BasicDBObject cancion = (BasicDBObject) ob;//cojo sus datos
							String estilo = cancion.get("Estilo") + "";
							if(!estilo.equals("null") && (estilo.equals(style) || style.equals("Todos")))//si coincide el estilo con el que busco o busco todos
								contador++;//sumo 1
						}
					}else{//si es solo una cancion
						DBObject cancion = (DBObject) listaCanciones;
						String estilo = cancion.get("Estilo") + "";
						if(!estilo.equals("null") && (estilo.equals(style) || style.equals("Todos")))
							contador++;
					}
				}
			}else{//si es solo un album
				 BasicDBObject albumTemporal = (BasicDBObject) alb;//cojo sus años y nombre
                                 String nombreA =albumTemporal.get("NombreA") + "";
                                 String yearTemporal = albumTemporal.get("Año") + "";
                                 if(!(nombreA.equals(album) || album.equals("Todos")))//si no es el nombre que $
                                	 continue;//voy al siguiente album
                                 if(!(yearTemporal.equals(year) || year.equals("Todos")))//si pertenece al año $
                                         continue;//voy al siguiente album
                                 Object listaCanciones = albumTemporal.get("Cancion");//consigo lista de canciones
                                 if(listaCanciones == null)//si no hay canciones
                                         continue;//siguiente album
                                 if(listaCanciones instanceof BasicDBList){//si es una lista de canciones
                                         BasicDBList canciones = (BasicDBList) listaCanciones;
                                         for(Object ob : canciones){//recorro las canciones
                                 	        BasicDBObject cancion = (BasicDBObject) ob;//cojo sus datos
                                                String estilo = cancion.get("Estilo") + "";
                                                if(!estilo.equals("null") && (estilo.equals(style) || style.equals("Todos")))//si coincide el estilo con el que busco o busco todos
                                        	        contador++;//sumo 1
                                        }
                                 }else{//si es solo una cancion
                                 	DBObject cancion = (DBObject) listaCanciones;
                                        String estilo = cancion.get("Estilo") + "";
                                        if(!estilo.equals("null") && (estilo.equals(style) || style.equals("Todos")))
                                        	contador++;
                                }
			}
		}
                return contador;
        }

	ArrayList<String> getEstilos(HttpServletRequest req){//a partir de un nodo album consigo todos los estilos
		ArrayList<String> al = new ArrayList<String>();
		String album = req.getParameter("album");
		String year = req.getParameter("year");
		DBCursor cursor = coll.find();
		while(cursor.hasNext()){
			DBObject dom = cursor.next();
			if(documentosErroneos.contains(dom + ""))
				continue;
			Object alb = dom.get("Album");
			if(alb == null)
				continue;
			if(alb instanceof BasicDBList){//si es una lista de albumes
				BasicDBList albumes = (BasicDBList) alb;
				for(Object o : albumes){//recorro album a album
					BasicDBObject albumTemporal = (BasicDBObject) o;//cojo sus años y nombre
					String nombreA =albumTemporal.get("NombreA") + "";
					String yearTemporal = albumTemporal.get("Año") + "";
					if(!(nombreA.equals(album) || album.equals("Todos")))//si no es el nombre que busco y no busco todos
						continue;//voy al siguiente album
					if(!(yearTemporal.equals(year) || year.equals("Todos")))//si pertenece al año que busco o no busco de cualquier año
						continue;//voy al siguiente album
					Object listaCanciones = albumTemporal.get("Cancion");//consigo lista de canciones
					if(listaCanciones == null)//si no hay canciones
						continue;//siguiente album
					if(listaCanciones instanceof BasicDBList){//si es una lista de canciones
						BasicDBList canciones = (BasicDBList) listaCanciones;
						for(Object ob : canciones){//recorro las canciones
							BasicDBObject cancion = (BasicDBObject) ob;//cojo sus datos
							String estilo = cancion.get("Estilo") + "";
							if(!estilo.equals("null") && !al.contains(estilo))//si aun no estaba su estilo en la lista
								al.add(estilo);//lo añado
						}
					}else{//si es solo una cancion
						DBObject cancion = (DBObject) listaCanciones;
						String estilo = cancion.get("Estilo") + "";
						if(!estilo.equals("null") && al.contains(estilo))
							al.add(estilo);
					}
				}
			}else{//si es solo un album
				 BasicDBObject albumTemporal = (BasicDBObject) alb;//cojo sus años y nombre
                                 String nombreA =albumTemporal.get("NombreA") + "";
                                 String yearTemporal = albumTemporal.get("Año") + "";
                                 if(!(nombreA.equals(album) || album.equals("Todos")))//si no es el nombre que $
                                	 continue;//voy al siguiente album
                                 if(!(yearTemporal.equals(year) || year.equals("Todos")))//si pertenece al año $
                                         continue;//voy al siguiente album
                                 Object listaCanciones = albumTemporal.get("Cancion");//consigo lista de canciones
                                 if(listaCanciones == null)//si no hay canciones
                                         continue;//siguiente album
                                 if(listaCanciones instanceof BasicDBList){//si es una lista de canciones
                                         BasicDBList canciones = (BasicDBList) listaCanciones;
                                         for(Object ob : canciones){//recorro las canciones
                                 	        BasicDBObject cancion = (BasicDBObject) ob;//cojo sus datos
                                                String estilo = cancion.get("Estilo") + "";
                                                if(!estilo.equals("null") && !al.contains(estilo))//si aun no $
                                        	        al.add(estilo);//lo añado
                                        }
                                 }else{//si es solo una cancion
                                 	DBObject cancion = (DBObject) listaCanciones;
                                        String estilo = cancion.get("Estilo") + "";
                                        if(!estilo.equals("null") && al.contains(estilo))
                                        	al.add(estilo);
                                }
			}
		}
		Collections.sort(al);
		return al;
	}
	void consulta2canciones(PrintWriter out, HttpServletRequest req){
		imprimirParametros(req,out);
		int cuenta = -1;
		cuenta = contarEstilo(req.getParameter("year"),req.getParameter("album"),req.getParameter("estilo"));
		if(cuenta == 1)
			out.println("Hay "+cuenta+" cancion");
		if(cuenta != 1 && cuenta != 0)
			out.println("Hay "+cuenta+" canciones");
		if(cuenta == 0)
			out.println("No hay ninguna canción del estilo elegido");//imposible que pase
		out.println("</br>");
		imprimirVolver(out,req);
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
		ArrayList<String> nombreAlbumes = new ArrayList<String>();
                String href = "href = '" + req.getRequestURL() +"?"+ req.getQueryString() + "&album=";
		if(ano.equals("NO"))//imprimo albumes por artistas
			nombreAlbumes = devolverAlbumesByArt(art);
		if(art.equals("NO"))//imprimo albumes por ano
			nombreAlbumes = devolverAlbumesByYear(ano);
                out.println("<ul>");
                for(String si : nombreAlbumes){
                        out.println("<li><a " + href + si + "'>"+si+"</a></li>");
                        out.println("<br>");
                }
		if(nombreAlbumes.size() > 0)
	                out.println("<li><a " + href+ "Todos'>Todos los albumes </a></li>");
                out.println("</ul>");
	}
/********************************FUNCION QUE DEVUELVE LOS ALBUMES DE UN ARTISTA
		*******************EN CONCRETO O TODOS SEGUN SE LE PASE********/
	ArrayList<String> devolverAlbumesByArt(String art){
			ArrayList<String> nombreAlbumes = new ArrayList<String>();
			DBCursor cursor = coll.find();
                        while(cursor.hasNext()){
                                DBObject documento = cursor.next();
				if(documentosErroneos.contains(documento + ""))
					continue;
                                DBObject nombre = (DBObject)documento.get("Nombre");
                                if(nombre == null){
				        continue;
				}
                                String temporal = nombre.get("NombreC") + "";
                                if(temporal.equals("null"))
                                        temporal = nombre.get("NombreG") + "";
                                if(temporal.equals("null"))
                                        continue;
                                if(art.equals("Todos") || art.equals(temporal)){
					if(documento.get("Album") == null){//por si no tiene albumes
						continue;
					}
					if(documento.get("Album") instanceof BasicDBList){//por si es una lista de albumes
                                	        BasicDBList albumes = (BasicDBList) documento.get("Album");
                                	        for(Object ob : albumes){
                                	                BasicDBObject dbo = (BasicDBObject) ob;
                                	                String año = dbo.get("Año") + "";
                                	                String nombreA = dbo.get("NombreA") + "";
                                	                nombreAlbumes.add(año + ",#" + nombreA);
                                	        }
					}else{//por si es un album
						BasicDBObject dbo = (BasicDBObject) documento.get("Album");
						String año = dbo.get("Año") + "";
                                                String nombreA = dbo.get("NombreA") + "";
                                                nombreAlbumes.add(año + ",#" + nombreA);
					}
                                }
                        }
                        Collections.sort(nombreAlbumes);
                        ArrayList<String> alT = new ArrayList<String>();
                        for(String s : nombreAlbumes)
                                alT.add(s.split(",#")[1]);
                        return alT;
	}

	//******************FUNCION QUE DEVUELVE TODOS LOS ALBUMES
	//**********************DE UN AÑO O TODOS
	ArrayList<String> devolverAlbumesByYear(String year){
		 ArrayList<String> nombreAlbumes = new ArrayList<String>();
                 DBCursor cursor = coll.find();
                 while(cursor.hasNext()){
                 	DBObject documento = cursor.next();
			if(documentosErroneos.contains(documento + ""))
				continue;
			if(documento.get("Album") == null)
				continue;
                        if(documento.get("Album") instanceof BasicDBList){
				BasicDBList albumes = (BasicDBList) documento.get("Album");
                	        for(Object ob : albumes){
                	        	BasicDBObject dbo = (BasicDBObject) ob;
                	                String año = dbo.get("Año") + "";
					if(!año.equals(year) && !year.equals("Todos"))//si no es el año escogido
						continue;//o es todos el año escogido paso al siguiente album
                	                String nombreA = dbo.get("NombreA") + "";
                	                nombreAlbumes.add(año + ",#" + nombreA);
                	        }
			}else{
				BasicDBObject dbo = (BasicDBObject) documento.get("Album");
                                String año = dbo.get("Año") + "";
                                if(!año.equals(year) && !year.equals("Todos"))//si no es el $
                                	continue;//o es todos el año escogido paso al siguie$
                                String nombreA = dbo.get("NombreA") + "";
                                nombreAlbumes.add(año + ",#" + nombreA);
			}
                }
                Collections.sort(nombreAlbumes);
                ArrayList<String> alT = new ArrayList<String>();
                for(String s : nombreAlbumes)
                	alT.add(s.split(",#")[1]);
                return alT;
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
