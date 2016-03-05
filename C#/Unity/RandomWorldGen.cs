 using UnityEngine;
using System.Collections;
using System.Threading;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
public class WorldGen : MonoBehaviour {
	private Thread hiloCalculadorMalla;
	protected Mesh mesh;
	public Vector3[] verts;
	public SaveGame.datosDeUnPlaneta datosDeEstePlaneta;
	Vector3[] initVerts;
	Vector3[] normals;
	Vector3[] verciesMontaña;
	float distanciaAguaCentro;
	bool[] indiceVerticesMontaña;
	bool[] indiceVerticesAgua;
	int[] triangulosOriginales;
	int[] montañaTriangulos;
	int[] aguaTriangulos;
	int[] waterTriang;
	public GameObject oceano2k;
	public GameObject oceanoInverso;
	public tipoEcosistema eco;
	public int verticesCordilleras,verticesOceanos,oceanos,cordilleras,arboles,prados,variacionNormal,nivelSuavizado,nidos;
	public float minRandom, maxRandom;
	public Material materialAgua;
	public Material[] ListaDeMateriales;
	public bool hayAgua,hayCordilleras;
	private bool mostrarDatos;
	private bool clicked;
	private string ecoString;
//***************************************MATERIALES***********************************
	public Material[] mountainMaterials;
	public Material[] swampMaterials;
	public Material[] grassMaterials;
	public Material[] snowMaterials;
	public Material[] sandMaterials;
	public Material[] fireMaterials;
	public Material[] volcanoMaterials;
	public Material[] rustMaterials;
	public Material[] mineralMaterials;
//*********************************************************************************
//*****************************************ARBOLES*******************************
	public GameObject[] swampTrees;
	public GameObject[] grassTrees;
	public GameObject[] snowTrees;
	public GameObject[] sandTrees;
	public GameObject[] volcanoTrees;
	public GameObject[] rustTrees;
	public GameObject[] mineralTrees;
	public GameObject[] oceanTrees;
	public GameObject[] arbolesAUsar;
//******************************************************************************
	//*****************************************NIDOS*******************************
	public GameObject[] swampNidos;
	public GameObject[] grassNidos;
	public GameObject[] snowNidos;
	public GameObject[] sandNidos;
	public GameObject[] volcanoNidos;
	public GameObject[] rustNidos;
	public GameObject[] mineralNidos;
	public GameObject[] oceanNidos	;
	public GameObject[] nidosAUsar;
	//******************************************************************************
	private Vector3 WorldPosition;
	public GameObject Prado;
	private bool datosEnviados;
	private System.Random random;
	public bool workDone;
	protected static int[][] triangulosCadaVertice,doblesCadaVertice;
	protected static bool noDeboGuardar,soyUnServer;
	protected static bool cargando;
	public GUIStyle estilo;

	public bool hiloDone = false;
	protected variablesIguales vari;
	public void newPlanet(){
		WorldPosition = transform.position;
		cargando=true;
		random = new System.Random ();
		foreach(Transform tr in transform){
			if(tr.name.Equals("Atmosphere"))
				tr.localPosition=Vector3.zero;
		}
		if(Network.isClient)
			return;
		if(Network.isServer)
			soyUnServer=true;
		mesh = GetComponent<MeshFilter>().mesh;
		if (variacionNormal == 0) //con variacion Normal voy a dividir por lo tanto nunca puede ser 0
			variacionNormal = 5;
		verts = mesh.vertices;
		initVerts = mesh.vertices;
		triangulosOriginales = mesh.triangles;
		normals = mesh.normals;
		indiceVerticesMontaña = new bool[verts.Length];
		indiceVerticesAgua=new bool[verts.Length];
		changePlanetForEco ();
		if(doblesCadaVertice==null)
			vari = SaveGame.Load<variablesIguales> ("necesaryData");
		if (vari!= null && vari.doblesCadaVertice != null) {
			noDeboGuardar=true;
			doblesCadaVertice=vari.doblesCadaVertice;
			triangulosCadaVertice=vari.triangulosCadaVertice;
		}
		estilo = GameObject.Find ("guardar").GetComponent<TextureEditor> ().estiloBotones;
		hiloCalculadorMalla = new Thread (crearTerreno);
		hiloCalculadorMalla.Start ();
	}
	//******************************************************************FUNCION QUE SI YA ESTA CARGADO QUE NO HAGA NADA*********************
	public void loadFinal(){
		changePlanetForEco ();
		if(hayAgua)
			createWater ();
		enviarDatos ();

		if (!Application.loadedLevelName.Equals ("SolarSystem")) {
			Debug.Log ("Me las piro");
			cargando=false;
			return;
		}
		try{
			gameObject.AddComponent<MeshCollider> ();
			//gameObject.GetComponent<MeshCollider>().smoothSphereCollisions=true;
		}catch(UnityException e){
			Debug.Log (e.Data);
		}
		finally{
			distribuirSobrePlaneta(accesorios,numAccesorios);
			distribuirSobrePlaneta(arbolesAUsar,arboles);
			GameObject[] pradoss= new GameObject[1];
			pradoss[0]=Prado;
			distribuirSobrePlaneta(pradoss,prados);
			if(!Network.isServer && !Network.isClient) //por ahora como los enemigos en multiplayer hacen cosas raras
				distribuirSobrePlaneta(nidosAUsar,nidos);//instancio los nidos solo en single player
			cargando=false;
		}
	}
	void Update(){
		//**************************SI EL HILO YA ACABO SU TRABAJO EMPIEZAN LAS POLLAS*********************************
		if (workDone) {
			mesh.vertices=verts;
			separateMeshes ();
			if(hayAgua)
				createWater ();
			workDone=false;
			if(vari==null){
				vari=new variablesIguales(triangulosCadaVertice,doblesCadaVertice);
			}
			if(!noDeboGuardar)
				SaveGame.Save<variablesIguales> (vari,"necesaryData");
			if(Application.loadedLevelName.Equals("Menu")){
				cargando=false;
				return;
			}
			vec3 vec;
			vec.x=transform.position.x;
			vec.y=transform.position.y;
			vec.z=transform.position.z;
			if(Network.isServer || (!Network.isServer && !Network.isClient)) //si estoy single player o soy el server
				GameObject.Find("guardar").GetComponent<SaveGame>().añadirPlanetaNuevoAlDiccionario(gameObject,vec); //guardo el planeta en yololandia
			enviarDatos();
			if(!Application.loadedLevelName.Equals("SolarSystem")){
				cargando=false;
				return;
			}
			try{
				gameObject.AddComponent<MeshCollider> ();
				//gameObject.GetComponent<MeshCollider>().smoothSphereCollisions=true;
			}catch(UnityException e){
				Debug.Log (e.Data);
			}
			finally{
				distribuirSobrePlaneta(accesorios,numAccesorios);
				distribuirSobrePlaneta(arbolesAUsar,arboles);
				GameObject[] pradoss= new GameObject[1];
				pradoss[1]=Prado;
				distribuirSobrePlaneta(pradoss,prados);
				if(!Network.isServer && !Network.isClient) //por ahora como los enemigos en multiplayer hacen cosas raras
					distribuirSobrePlaneta(nidosAUsar,nidos);//instancio los nidos solo en single player
				cargando=false;
			}
		}
	
	}
	//*************************CREO EL PLANETA ES DECIR DIOS**********************************************
	void crearTerreno(){ //esto son los calculos del hilo
		cargando=true;
		if(doblesCadaVertice==null){
			doblesCadaVertice= new int[verts.Length][];
			distribuirDobles ();
		}

		if(triangulosCadaVertice==null){
			triangulosCadaVertice= new int[verts.Length][];
			distribuirTriangulos ();
		}
		if(hayCordilleras){
			for(int i=0;i<cordilleras;i++) //creo cordilleras segun cuantas hayas puesto
				crearCordillera (verticesCordilleras);
		}
		if(hayAgua){
			for(int i=0;i<oceanos;i++) //creo oceanos segun cuantos hayas puesto
				crearOceanos (verticesOceanos);
		}
		variedad (); //cambio un poco las alturas y tal del resto de los vertices para que no sea completamente redondo lo que queda
		for(int i=0;i<nivelSuavizado;i++)
			basicSmooth();
		getTrianglesMountainAndWater ();
		hiloDone = true;
		workDone = true;
	}
	//*************************************DISTRIBUYO EN CADA VERTICE TODOS SUS DOBLES EN UN ARRAY BIDIMENSIONAL************************************
	void distribuirDobles(){
		int[] tmp;
		for(int i=0;i<verts.Length;i++) //relleno los dobles
			doblesCadaVertice[i]=new int[0];
		for (int i=0; i<verts.Length; i++) {
			for(int k=0;k<verts.Length;k++){
				if(verts[i]==verts[k] && k!=i){
					tmp=doblesCadaVertice[i];
					doblesCadaVertice[i]=new int[doblesCadaVertice[i].Length+1];
					tmp.CopyTo(doblesCadaVertice[i],0);
					doblesCadaVertice[i][doblesCadaVertice[i].Length-1]=k;
				}
			}
		}
	}
	//*************************************DISTRIBUYO EN CADA VERTICE TODOS SUS TRIANGULOS EN UN ARRAY BIDIMENSIONAL************************************
	void distribuirTriangulos(){
		int[] tmp;
		int arLenght;
		for (int i=0; i<verts.Length; i++) 
			triangulosCadaVertice[i]=new int[0];
		
		
		for (int i=0; i<triangulosOriginales.Length; i+=3) {
			for(int k=0;k<3;k++){
				tmp=triangulosCadaVertice[triangulosOriginales[i+k]];
				arLenght=triangulosCadaVertice[triangulosOriginales[i+k]].Length+3;
				triangulosCadaVertice[triangulosOriginales[i+k]]=new int[arLenght];
				tmp.CopyTo(triangulosCadaVertice[triangulosOriginales[i+k]],0);
				triangulosCadaVertice[triangulosOriginales[i+k]][arLenght-3]=triangulosOriginales[i];
				triangulosCadaVertice[triangulosOriginales[i+k]][arLenght-2]=triangulosOriginales[i+1];
				triangulosCadaVertice[triangulosOriginales[i+k]][arLenght-1]=triangulosOriginales[i+2];
			}
		}
	}
	//****************************CREO LAS MONTAÑAS***********************************************
	void crearCordillera(int numOfVertex){ //el funcionamiento es como el de los oceanos pero 
		int contador = numOfVertex; //en vez de quitarle altura se la añado
		int verticeAlAzar=random.Next(0,verts.Length);
		int anteriorVerticeAlAzar=verticeAlAzar;
		int[] dobles=new int[0];
		while (contador>0) {
			if(verticeAlAzar>=verts.Length)
				verticeAlAzar=0;
			
			float multiply = (float)random.NextDouble()*maxRandom;
			if(indiceVerticesMontaña[verticeAlAzar]==true){
				verticeAlAzar=gerNearbyVerts(anteriorVerticeAlAzar,dobles,0);
				contador--;
				continue;
			}
			verts[verticeAlAzar] += normals[verticeAlAzar] * multiply;
			indiceVerticesMontaña[verticeAlAzar]=true;
			dobles=doblesCadaVertice[verticeAlAzar];
			foreach(int i in dobles){
				verts[i]=verts[verticeAlAzar];
				indiceVerticesMontaña[i]=true;
			}
			anteriorVerticeAlAzar=verticeAlAzar;
			verticeAlAzar=gerNearbyVerts(verticeAlAzar,dobles,0); //escojo aleatoriamente un vertice de su alrededor
			contador--;
		}
	}
	//**************************CREO LAS PROFUNDIDADES OCEANICAS********************************
	void crearOceanos(int numOfVertex){
		int contador = numOfVertex; //esto es para saber cuantas veces voy a iterar
		int verticeAlAzar=random.Next(0,verts.Length); //consigo un punto aleatorio del planeta
		int[] dobles=new int[0];
		while(indiceVerticesMontaña[verticeAlAzar]==true) //si ya es una montaña sigo buscando puntos aleatorios del planeta
			verticeAlAzar=random.Next(0,verts.Length);
		
		while (contador>0) { //iteraciones
			if(verticeAlAzar>=verts.Length) //si por algun fallo el vertice es mayor que la longitud de verts lo vuelvo 0 pero no deberia pasar
				verticeAlAzar=0;
			
			float multiply = (float)random.NextDouble()*maxRandom; //como devuelve de 0 a 1 es mucho asi q lo multiplico x el num max asi devuelve como maximo num max
			if(indiceVerticesAgua[verticeAlAzar]==true || indiceVerticesMontaña[verticeAlAzar]==true){ //si es agua o montaña busco otro vertice cercano
				verticeAlAzar=gerNearbyVerts(verticeAlAzar,dobles,1);
				contador--;
				continue;
			}
			verts[verticeAlAzar] -= normals[verticeAlAzar] * multiply; //le quito altura (para hacer oceano y tal)
			indiceVerticesAgua[verticeAlAzar]=true; //digo que este vertice es agua
			dobles=doblesCadaVertice[verticeAlAzar];
			foreach(int i in dobles){
				verts[i]=verts[verticeAlAzar];
				indiceVerticesAgua[i]=true;
			}
			verticeAlAzar=gerNearbyVerts(verticeAlAzar,dobles,1); //escojo aleatoriamente un vertice de su alrededor
			contador--;
		}
	}

	//***********************LE DOY UNA PEQUEÑA VARIACION AL TERRENO NO MONTAÑOSO NI ACUOSO*************************************************
	void variedad(){
		int[] dobles;
		for (int i=0; i<verts.Length; i++) { //recorro todos los vertices
			if(!indiceVerticesAgua[i] && !indiceVerticesMontaña[i] && verts[i]==initVerts[i]){ //si el vertice no esta caracterizado como montaña o agua y no se ha tocado ya
				float multiply = (float)random.NextDouble()*maxRandom/variacionNormal;
				verts[i] += normals[i] * multiply; //le añado una altura
				dobles=doblesCadaVertice[i];
				foreach(int k in dobles)
					verts[k]=verts[i];
			}
		}
	}

	//**********************************HAGO UN BASIC SMOOTH ES DECIR UNA MEDIA DE LOS PUNTOS CON LOS CERCANOS************************************
	void basicSmooth(){
		Vector3 media;
		int numMedia;
		for (int i=0; i<verts.Length; i++) {
			numMedia=1;
			media=verts[i];
			foreach (int k in triangulosCadaVertice[i]){
				if(k!=i){
					numMedia++;
					media+=verts[k];
				}
			}
			verts[i]=media/numMedia;
			for(int k=0;k<doblesCadaVertice[i].Length;k++) //hago su misma altura en los dobles
				verts[doblesCadaVertice[i][k]]=verts[i];
		}
	}


	//******************LO USO PARA DISTRIBUIR ARBOLES, ACCESORIOS NIDOS TODO SOBRE EL PLANETA*********************
	void distribuirSobrePlaneta(GameObject[] g,int max){
		GameObject go=null;
		GetComponent<SphereCollider> ().enabled = false;
		for(int i =0; i< max;i++){
			Vector3 v = transform.position+randomVector3(minSpawn,maxSpawn);
			if(Network.isServer)
				go=Network.Instantiate(g[Random.Range(0,g.Length)],v,transform.rotation,6) as GameObject;
			else{
				if(!Network.isServer && !Network.isClient)
					go=Instantiate(g[Random.Range(0,g.Length)],v,transform.rotation) as GameObject;
			}
			if(go!=null){
				backToPlanet(go);
				if(go==null)
					continue;
				go.transform.parent=transform;
				if(Network.isServer)
					go.GetComponent<PlanetAccesory>().GetComponent<NetworkView>().RPC("changePlanetById",RPCMode.AllBuffered,GetComponent<NetworkView>().viewID);
				else
					go.GetComponent<PlanetAccesory>().Planet=gameObject;
			}
		}
		GetComponent<SphereCollider> ().enabled = true;
	}
	//****************************FUNCION AUXILIAR PARA CALCULAR UN VECTOR3 ALEATORIO***********************************
	public static Vector3 randomVector3(float min,float max){
		float x=Random.Range(min,max);
		float y=Random.Range(min,max);
		float z=Random.Range(min,max);
		if(Random.Range(0,2)==0)
			x=(-x);
		if(Random.Range(0,2)==0)
			y=(-y);
		if(Random.Range(0,2)==0)
			z=(-z);
		return new Vector3(x,y,z);
	}
	//************************************ATRAIGO CUALQUIER GAMEOBJECT A LA SUPERFICIE DEL PLANETA***********************
	public void backToPlanet(GameObject g){
		Vector3 worldUp = g.transform.position - transform.position; //lo roto de tal manera como la gravedad
		g.transform.rotation = Quaternion.FromToRotation(g.transform.up, worldUp) * g.transform.rotation;
		int layer= 1<<9;
		RaycastHit hit;
		Physics.Linecast(g.transform.position,transform.position,out hit);
		if (hit.collider==null || hit.collider.tag.Equals("Ocean")){
			Destroy (g);
			return;
		}
		Physics.Linecast (g.transform.position,transform.position,out hit,layer); //invoco un rayo desde el bicho hacia el planeta
		if (hit.collider != null) {
			if(hit.collider.tag.Equals("Suelo")){ //si encuentro suelo
				g.transform.position=(hit.point+g.transform.up/4); //lo bajo al nivel del suelo
			}
		}else {//significa que esta en el interior del planeta asi que no voy a gastar memoria con este mamon
			Destroy (g);
			return;
		}
	}
	//*****************************A PARTIR DE LOS INDICES DE LOS VERTICES MONTAÑOSOS SACO LOS TRIANGULOS***********************************
	void getTrianglesMountainAndWater(){
		int[] tmp;
		int longTemp = 0;
		montañaTriangulos = new int[0];
		aguaTriangulos = new int[0];
		for(int i=0;i<indiceVerticesMontaña.Length;i++){ //miro cuales son los vertices de la montaña
			if(indiceVerticesMontaña[i]==true){ //si encuentro un vertice de montaña
				longTemp=montañaTriangulos.Length;
				tmp=montañaTriangulos;
				montañaTriangulos=new int[longTemp+triangulosCadaVertice[i].Length];
				tmp.CopyTo(montañaTriangulos,0);
				triangulosCadaVertice[i].CopyTo(montañaTriangulos,longTemp);
			}
		}
		for(int i=0;i<indiceVerticesAgua.Length;i++){ //lo mismo pero con el agua
			if(indiceVerticesAgua[i]==true){
				longTemp=aguaTriangulos.Length;
				tmp=aguaTriangulos;
				aguaTriangulos=new int[longTemp+triangulosCadaVertice[i].Length];
				tmp.CopyTo(aguaTriangulos,0);
				triangulosCadaVertice[i].CopyTo(aguaTriangulos,longTemp);
			}
		}
		
	}
	//*********************************CREO UNA MESH DE AGUA****************************************
	void createWater(){ //con esto voy a crear la mesh del agua
		GameObject ocean;
		ocean = Instantiate (oceano2k,transform.position,transform.rotation) as GameObject;
		ocean.transform.parent = gameObject.transform;
		if(eco!= tipoEcosistema.Ocean)
			ocean.transform.localScale = new Vector3 (1, 1, 1);
		else
			ocean.transform.localScale= new Vector3(1.04f,1.04f,1.04f);
		ocean.name = gameObject.name + "Ocean";
		ocean.tag="Ocean";
		Mesh oceanMesh=ocean.GetComponent<MeshFilter>().mesh;
		ocean.GetComponent<MeshFilter> ().GetComponent<Renderer>().material=materialAgua;
		ocean.AddComponent<WaterSimple> ();
		ocean.AddComponent<OceanScript> ();
		ocean.GetComponent<OceanScript> ().oceanoInverso = oceanoInverso;
		ocean.GetComponent<SphereCollider> ().isTrigger = true;

	}

	//*************************ME DEVUELVE UN VERTICE ADYACENTE A OTRO (REFIRIEDOME A ELLOS COMO INDICES DE LA MATRIZ ORIGINAL)**************************************
	int gerNearbyVerts(int verticeRef,int[] verticesDobles,int modo){ //devuelvo un vertice aleatorio vecino
		int[] nearbyTriangles = new int[0];
		int[] temp1=new int[0],temp2=new int[0];
		nearbyTriangles = triangulosCadaVertice [verticeRef];
		//****************************AQUI COJO TODOS LOS TRIANGULOS INCLUIDOS LOS DOBLES Y LOS METO EN UN ARRAY****************
		foreach (int i in verticesDobles) {
			temp1=triangulosCadaVertice[i];
			temp2= new int[nearbyTriangles.Length+temp1.Length];
			nearbyTriangles.CopyTo(temp2,0);
			temp1.CopyTo(temp2,nearbyTriangles.Length);
			nearbyTriangles=temp2;
		}
		temp1= temp2 = new int[0];
		//*******************************BUSCO EN ESE ARRAY ANTERIOR CUAL DE ELLOS NO ES UNA MONTAÑA Y LOS VOY INTRODUCIENDO EN OTRO ARRAY****************
		if(modo==0){ //si estoy con las montañas
			for (int i=0; i<nearbyTriangles.Length; i++) {
				if(!indiceVerticesMontaña[nearbyTriangles[i]]){ //busco cuales de los vertices de al lado no son montañas
					temp2=temp1; //al descubrirlo lo añado a un array temporal
					temp1=new int[temp1.Length+1];
					temp2.CopyTo(temp1,0);
					temp1[temp1.Length-1]= nearbyTriangles[i];
				}
			}
			//**************************DEVUELVO UNO DE ESE NUEVO ARRAY*****************************
			if(temp1.Length>0) //si consegui al menos un punto cercano que no es una montaña devuelvo un punto aleatorio
				return temp1[random.Next(0,temp1.Length)];
		}

		//*******************************BUSCO EN ESE ARRAY ANTERIOR CUAL DE ELLOS NO ES UNA MONTAÑA NI OCEANO Y LOS VOY INTRODUCIENDO EN OTRO ARRAY****************
		if(modo==1){ //si estoy con oceano
			for (int i=0; i<nearbyTriangles.Length; i++) { //lo mismo pero tienen que no ser montañas ni oceanos
				if(!indiceVerticesMontaña[nearbyTriangles[i]] && !indiceVerticesAgua[nearbyTriangles[i]]){
					temp2=temp1;
					temp1=new int[temp1.Length+1];
					temp2.CopyTo(temp1,0);
					temp1[temp1.Length-1]= nearbyTriangles[i];
				}
			}
			//*************************DEVUELVO UNO DE ESE NUEVO ARRAY********************************
			if(temp1.Length>0)
				return temp1[random.Next(0,temp1.Length)];
		}
		return random.Next (0,verts.Length); //esto es por si todo falla y devuelve un 0 no deberia pasar
	}

	//*******************ESTO ES PARA INSTANCIAR ACCESORIOS***********************************
	public int numAccesorios=0;
	public GameObject[] accesorios;
	public float minSpawn,maxSpawn;
	//**********************ESTO CAMBIA LAS CARACTERISTICAS SEGUN EL TIPO DE PLANETA**************************
	void changePlanetForEco(){
		int temp;
		switch (eco) {
		case tipoEcosistema.Rust:
			ecoString= "Rust";
			hayCordilleras=true;
			hayAgua=false;
			oceanos=0;
			cordilleras=4;
			temp=Random.Range(0,rustMaterials.Length);
			ListaDeMateriales[0]=rustMaterials[temp];
			ListaDeMateriales[1]=rustMaterials[temp];
			arboles=200;
			arbolesAUsar=rustTrees;
			nidosAUsar=rustNidos;
			break;
		case tipoEcosistema.Mineral:
			ecoString="Mineral";
			hayCordilleras=true;
			hayAgua=false;
			oceanos=0;
			cordilleras=5;
			temp=Random.Range(0,rustMaterials.Length);
			ListaDeMateriales[0]=mineralMaterials[temp];
			temp=Random.Range(0,rustMaterials.Length);
			ListaDeMateriales[1]=mineralMaterials[temp];
			arboles=200;
			arbolesAUsar=mineralTrees;
			nidosAUsar=mineralNidos;
			break;
		case tipoEcosistema.Swamp:
			ecoString="Swamp";
			hayCordilleras=false;
			hayAgua=true;
			oceanos=8;
			verticesOceanos=200;
			cordilleras=0;
			temp=Random.Range(0,swampMaterials.Length);
			ListaDeMateriales[0]=swampMaterials[temp];
			ListaDeMateriales[1]=swampMaterials[temp];
			arboles=200;
			arbolesAUsar=swampTrees;
			nidosAUsar=swampNidos;
			break;
		case tipoEcosistema.Desert:
			ecoString="Desert";
			hayCordilleras=false;
			hayAgua=false;
			oceanos=0;
			cordilleras=0;
			temp=Random.Range(0,sandMaterials.Length);
			ListaDeMateriales[0]=sandMaterials[temp];
			ListaDeMateriales[1]=sandMaterials[temp];
			arboles=200;
			arbolesAUsar=sandTrees;
			nidosAUsar=sandNidos;
			break;
		case tipoEcosistema.Fire:
			ecoString="Fire";
			hayCordilleras=true;
			hayAgua=true;
			oceanos=2;
			cordilleras=4;
			temp=Random.Range(0,volcanoMaterials.Length);
			ListaDeMateriales[0]=volcanoMaterials[temp];
			temp=Random.Range(0,mountainMaterials.Length);
			ListaDeMateriales[1]=mountainMaterials[temp];
			temp=Random.Range(0,fireMaterials.Length);
			ListaDeMateriales[2]=fireMaterials[temp];
			materialAgua=fireMaterials[temp];
			arboles=200;
			arbolesAUsar=volcanoTrees;
			nidosAUsar=volcanoNidos;
			break;
		case tipoEcosistema.Jungle:
			ecoString="Normal";
			hayCordilleras=true;
			hayAgua=true;
			oceanos=2;
			cordilleras=3;
			temp=Random.Range(0,grassMaterials.Length);
			ListaDeMateriales[0]=grassMaterials[temp];
			temp=Random.Range (0,mountainMaterials.Length);
			ListaDeMateriales[1]=mountainMaterials[temp];
			temp=Random.Range (0,grassMaterials.Length);
			ListaDeMateriales[2]=grassMaterials[temp];
			arbolesAUsar=grassTrees;
			nidosAUsar=grassNidos;
			prados=300;
			arboles=2000;
			break;
		case tipoEcosistema.Snow:
			ecoString="Snow";
			hayCordilleras=true;
			hayAgua=false;
			oceanos=0;
			cordilleras=4;
			temp=Random.Range(0,snowMaterials.Length);
			ListaDeMateriales[0]=snowMaterials[temp];
			ListaDeMateriales[1]=snowMaterials[temp];
			arboles=300;
			arbolesAUsar=snowTrees;
			nidosAUsar=snowNidos;
			break;
		case tipoEcosistema.Ocean:
			ecoString="Ocean";
			hayCordilleras=true;
			hayAgua=true;
			cordilleras=15;
			verticesCordilleras=80;
			oceanos=3;
			verticesOceanos=100;
			arboles=80;
			arbolesAUsar=oceanTrees;
			nidosAUsar=oceanNidos;
			nidos=0;
			break;
		}
	}
	void enviarDatos(){
		if(!datosEnviados && Network.isServer){ //si soy server hago que el planeta se vea igual en todas partes
			BinaryFormatter binFormatter = new BinaryFormatter ();
			byte[] b;
			MemoryStream m = new MemoryStream();
			binFormatter.Serialize(m,datosDeEstePlaneta);
			b=m.ToArray();
			GetComponent<NetworkView>().RPC("changePlanet",RPCMode.OthersBuffered,b);
			datosEnviados=true;
		}
	}
	
	//***************************CREA LAS 3 SUBMHESES ES DECIR, EL SUELO NORMAL, LAS MONTAÑAS, LO DE DEBAJO DEL AGUA **************************
	void separateMeshes(){
		mesh.subMeshCount = 3;
		if(aguaTriangulos!=null && aguaTriangulos.Length>0)
			mesh.SetTriangles (aguaTriangulos, 2);
		if(montañaTriangulos!=null && montañaTriangulos.Length!=0)
			mesh.SetTriangles (montañaTriangulos, 1);
		GetComponent<MeshFilter> ().GetComponent<Renderer>().materials = ListaDeMateriales;
	}
	

	void OnTriggerEnter(Collider col){
		if(col.tag.Equals("Aircraft")){
			col.gameObject.GetComponent<BasicAricraft>().planet=gameObject;

		}
		if (col.tag.Equals ("Player")) {
			col.gameObject.GetComponent<PJMovement>().planet=gameObject;
		}
		if (col.tag.Equals ("Spawner")) {
			col.gameObject.GetComponent<Spawneador>().planet=gameObject;
		}
	}
	void OnTriggerExit(Collider col){
		if(col.tag.Equals("Aircraft")){
			col.gameObject.GetComponent<BasicAricraft>().planet=null;

		}
		if(col.tag.Equals("Spawner")){
			col.gameObject.GetComponent<Spawneador>().planet=null;
			Destroy (col.gameObject.GetComponent<Spawneador>().particulaActiva);
		}
	}
	//********************************PASO EL ECOSISTEMA QUE ME LLEGA AL PLANETA*************
	[RPC]
	void changeEco(int t){
		Debug.Log ("voy a cambiar a " + (tipoEcosistema)t);
		eco= (tipoEcosistema) t;
		changePlanetForEco ();
	}
	//*****************************PASO LOS DATOS QUE ME LLEGA DE INTERNET AL PLANETA**********************
	[RPC]
	void changePlanet(byte[] b){
		if(mesh==null)
			mesh = GetComponent<MeshFilter>().mesh;
		BinaryFormatter binFormatter = new BinaryFormatter ();
		MemoryStream m = new MemoryStream (b);
		SaveGame.datosDeUnPlaneta p = (SaveGame.datosDeUnPlaneta)binFormatter.Deserialize (m);
		GameObject.Find ("guardar").GetComponent<SaveGame> ().prepararPlaneta (gameObject, p);
		if(Application.loadedLevelName.Equals("SolarSystem"))
			gameObject.AddComponent<MeshCollider> ();
		cargando = false;
	}
	void escogido(){
		SaveGame.datosDeUnPlaneta d= new SaveGame.datosDeUnPlaneta();
		vec3 vec;
		vec.x = transform.position.x;
		vec.y = transform.position.y;
		vec.z = transform.position.z;
		GameObject.Find ("guardar").GetComponent<SaveGame> ().diccionarioPlanetas.TryGetValue (vec, out d);
		GameObject.Find ("guardar").GetComponent<SaveGame> ().datosParaCargar = d;
		GameObject.Find ("guardar").GetComponent<SaveGame>().ecoPlaneta = eco;
		if (!Network.isServer && !Network.isClient) {
			SaveGame.Save(GameObject.Find("guardar").GetComponent<SaveGame>().diccionarioPlanetas,"Universe");
			Application.LoadLevel("SolarSystem");
		}else{
			if(Network.isServer){
				GameObject.Find("guardar").GetComponent<NetworkView>().RPC("LoadLevel",RPCMode.AllBuffered,"SolarSystem");
			}
		}
		//aqui cargamos el siguiente nivel
	}
	void OnMouseEnter(){
		if(Application.loadedLevelName.Equals("Mapa"))
			mostrarDatos=true;
	}
	void OnMouseExit(){
		if(Application.loadedLevelName.Equals("Mapa"))
			mostrarDatos=false;
	}
	void OnMouseOver(){
		if (Application.loadedLevelName.Equals("Mapa") && Input.GetMouseButton (0)) {
			clicked=true;
		}
	}
	//**********************************COSAS PARA MOSTRAR SU CUADRADO EN LA GUI***************************
	public GUIStyle estiloCarga;
	private Vector3 pos;
	void OnGUI(){
		/*if(Application.loadedLevelName.Equals("SolarSystem") && cargando){
			GUI.Box (new Rect(Screen.width/2-Screen.width/4,Screen.height/2-Screen.height/4,Screen.width/2,Screen.height/2),"Loading..",estiloCarga);
		}*/
		if(mostrarDatos && !clicked){
			pos=Camera.main.WorldToScreenPoint (transform.position);
			pos.y=Screen.height-pos.y;
			GUI.Box(new Rect(pos.x,pos.y,Screen.width/5,Screen.height/5),"Eco: "+ecoString,SaveGame.estilo);
		}
		if (clicked) {
			if(GUI.Button(new Rect(Screen.width/2-Screen.width/4,Screen.height/2,Screen.width/4,Screen.height/4),"TRAVEL",SaveGame.estilo))
				escogido();
			if(GUI.Button(new Rect(Screen.width/2,Screen.height/2,Screen.width/4,Screen.height/4),"CANCEL",SaveGame.estilo))
				clicked=false;
		}
	}
	//*************************************************************************************************************
}
//**************************************CLASE PARA GUARDAR EL HIPER CALCULO*******************************
	[System.Serializable]
	public class variablesIguales{
		public int[][] triangulosCadaVertice,doblesCadaVertice;
		public variablesIguales(){
		}
		public variablesIguales(int[][] t,int[][] d){
			triangulosCadaVertice = t;
			doblesCadaVertice = d;
		}
}


public enum tipoEcosistema{Jungle,Snow,Desert,Fire,Rust,Swamp,Ocean,Mineral};//