using UnityEngine;
using System.Collections;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Collections.Generic;


public class SaveGame : MonoBehaviour{
	public int numMaxPlanetas=5;
	public Vector3[] posSistemasSolares;
	public Dictionary<vec3,datosDeUnPlaneta> diccionarioPlanetas;
	public datosDeUnPlaneta datosParaCargar;
	public float[] coordenadas;
	public string IDSistemaSolar;
	public tipoEcosistema[] ecosistemasEsteSistemaSolar;
	public string perfil;
	public GameObject planet,planetaMap;
	public float distanciaEntrePlanetas;
	public GameObject playerOnSolarSystem;
	public GameObject Chat;
	public tipoEcosistema ecoPlaneta;
	public static GUIStyle estilo;
	MyPlanetVariables variables;
	private int lastLevelPrefix = 0;
	public Material[] ListaMateriales;
	// Use this for initialization
	void OnLevelWasLoaded(int level) {
		DontDestroyOnLoad (gameObject);
		//**************************INSTANCIO EL CHAT**********************************
		if(Network.isServer && !GameObject.Find("Chat"))
			Network.Instantiate (Chat,new Vector3(0,0,0),transform.rotation,0);
		ecosistemasEsteSistemaSolar = null;
		//**************************INSTANCIO AL JUGADOR**********************************
		if (Application.loadedLevelName.Equals ("SolarSystem")) {
			//variables=Load<MyPlanetVariables>("MyPlanetSave");
			if(Network.isServer || Network.isClient){ //cargo el jugador
				GameObject[] g =GameObject.FindGameObjectsWithTag("Player");
				Network.Instantiate(playerOnSolarSystem,new Vector3(0,10*g.Length,0),transform.rotation,0);
			}
			else Instantiate (playerOnSolarSystem,new Vector3(0,0,0),transform.rotation);
		}
		//**********************************CARGO EL SISTEMA SOLAR (SI NO ESTA EN MEMORIA HAGO UNO NUEVO)**********************************
		if (Application.loadedLevelName.Equals ("SolarSystem") && (!Network.isClient)) { //esto es para cargar el sistema solar
			GameObject g;
			if(Network.isServer){
				g= Network.Instantiate(planet,new Vector3(0,0,5000),transform.rotation,0) as GameObject;
				g.GetComponent<WorldGen>().GetComponent<NetworkView>().RPC("changeEco",RPCMode.AllBuffered,(int)ecoPlaneta);
			}
			else{
				g= Instantiate(planet,new Vector3(0,0,5000),transform.rotation) as GameObject;
				g.GetComponent<WorldGen>().eco=ecoPlaneta;
			}
			prepararPlaneta(g,datosParaCargar);
		}
		if (Application.loadedLevelName.Equals ("Mapa") && !Network.isClient) {
			diccionarioPlanetas=Load<Dictionary<vec3,datosDeUnPlaneta>>("Universe");
			if(diccionarioPlanetas==null)
				diccionarioPlanetas= new Dictionary<vec3, datosDeUnPlaneta>();
			Dictionary<vec3,datosDeUnPlaneta>.KeyCollection llaves = diccionarioPlanetas.Keys;
			foreach (vec3 v in llaves){
				GameObject g;
				if(!Network.isClient && !Network.isServer)
					g=Instantiate(planet,new Vector3(v.x,v.y,v.z),transform.rotation) as GameObject;
				else 
					g=Network.Instantiate(planet,new Vector3(v.x,v.y,v.z),transform.rotation,6) as GameObject;
				datosDeUnPlaneta d;
				diccionarioPlanetas.TryGetValue(v,out d);
				prepararPlaneta(g,d);
			}
		}
	}
	public void prepararPlaneta( GameObject g, datosDeUnPlaneta d){
		Mesh mesh = g.GetComponent<MeshFilter> ().mesh;
		Vector3[] temp= new Vector3[d.vertices.Length];
		for (int i=0; i<temp.Length; i++) {
			temp[i].x=d.vertices[i].x;
			temp[i].y=d.vertices[i].y;
			temp[i].z=d.vertices[i].z;

		}
		mesh.vertices = temp; //le coloco los vertices
		mesh.subMeshCount = d.triangulos.Length;
		for (int i=0; i<d.triangulos.Length; i++) { //le coloco las submeshes
			mesh.SetTriangles(d.triangulos[i],i);
		}
		Material[] lista = new Material[d.materiales.Length];
		for (int i=0; i<lista.Length; i++) { //voy poniendo en la lista
			for(int j=0;j<ListaMateriales.Length;j++){ //recorro la lista de materiales para buscar el material
				if(d.materiales[i].Contains(ListaMateriales[j].name)){ //si encuentro el material
					lista[i]=ListaMateriales[j]; //lo pongo en la lista
					break;
				}
			}
		}
		g.GetComponent<Renderer>().materials = lista;
		WorldGen wg = g.GetComponent<WorldGen> ();
		wg.eco = d.eco;
		wg.datosDeEstePlaneta = d;
		wg.loadFinal ();
	}
	int numPlanetas=0;
	int posPlaneta=0;
	void OnGUI(){
		if (Application.loadedLevelName.Equals ("Mapa")) {
			if(GUI.Button(new Rect(0,Screen.height-Screen.height/4,Screen.width/4,Screen.height/4),"New Planet",estilo)){
				numPlanetas=diccionarioPlanetas.Keys.Count;
				Vector3 temp=WorldGen.randomVector3(numPlanetas*1500,numPlanetas*2500);
				vec3 t = new vec3();
				temp.y=-2000;
				t.x=temp.x;
				t.y=temp.y;
				t.z=temp.z;
				tipoEcosistema ti = GetRandomEnum<tipoEcosistema>();
				GameObject g;
				if(!Network.isClient && !Network.isServer)
					g=Instantiate(planet,temp,transform.rotation) as GameObject;
				else 
					g=Network.Instantiate(planet,temp,transform.rotation,6) as GameObject;
				g.GetComponent<WorldGen>().eco=ti;
				g.GetComponent<WorldGen>().newPlanet();
				Camera.main.transform.position=new Vector3(g.transform.position.x,Camera.main.transform.position.y,g.transform.position.z);
			}
			if(GUI.Button(new Rect(Screen.width/4,Screen.height-Screen.height/4,Screen.width/4,Screen.height/4),"Next Planet",estilo)){
				GameObject[] Planets= GameObject.FindGameObjectsWithTag("Suelo");
				if(Planets.Length==0)
					return;
				if(posPlaneta+1>=Planets.Length)
					posPlaneta=0;
				Camera.main.transform.position=new Vector3(Planets[posPlaneta].transform.position.x,Camera.main.transform.position.y,Planets[posPlaneta].transform.position.z);
				posPlaneta+=1;
			}
		}
	}
	public void añadirPlanetaNuevoAlDiccionario(GameObject g,vec3 v){
		datosDeUnPlaneta d = new datosDeUnPlaneta();
		WorldGen wg = g.GetComponent<WorldGen>();
		d.eco = wg.eco;
		vec3[] temp= new vec3[wg.verts.Length];
		for(int i=0;i<wg.verts.Length;i++){
			temp[i].x=wg.verts[i].x;
			temp[i].y=wg.verts[i].y;
			temp[i].z=wg.verts[i].z;
		}
		d.vertices = temp; //pongo sus vertices
		d.materiales= new string[g.GetComponent<Renderer>().materials.Length];
		for (int i=0; i<d.materiales.Length; i++) //los materiales
			d.materiales[i]=g.GetComponent<Renderer>().materials[i].name;
		d.triangulos= new int[g.GetComponent<MeshFilter>().mesh.subMeshCount][]; //los triangulos
		for (int i=0; i<d.triangulos.Length; i++) 
			d.triangulos[i]=g.GetComponent<MeshFilter>().mesh.GetTriangles(i);
		wg.datosDeEstePlaneta = d;
		diccionarioPlanetas.Add (v,d); //al final lo añado al diccionario
	}
	void Start(){
		DontDestroyOnLoad (gameObject);
		estilo = GameObject.Find ("MultiLoader").gameObject.GetComponent<mainMenu> ().estilo;
	}
	//***************************INTENTO GUARDAR LA POSICION DE LOS SISTEMAS SOLARES Y QUE HAY EN CADA UNO************************************
	public void SaveAllData(){
		if(variables == null)
			variables=new MyPlanetVariables();
		variables.numObjetos=MyPlanet.numObjetos;
		Save <MyPlanetVariables>(variables,"MyPlanetSave");
		Save<Dictionary<vec3,datosDeUnPlaneta>> (diccionarioPlanetas, "Universe");
	}
	public void eraseUniverse(){
		diccionarioPlanetas= new Dictionary<vec3,datosDeUnPlaneta >();
		Save<Dictionary<vec3,datosDeUnPlaneta>> (diccionarioPlanetas, "Universe");
	}
	//***************************INTENTO CARGAR LA POSICION DE LOS SISTEMAS SOLARES Y QUE HAY EN CADA UNO************************************
	public void LoadAllData(){
		variables=Load<MyPlanetVariables>("MyPlanetSave");
		diccionarioPlanetas = Load<Dictionary<vec3,datosDeUnPlaneta>> ("Universe");
	}
	//****************************COJO UN ENUM ALEATORIO*********************************
	public static T GetRandomEnum<T>(){
		System.Array A = System.Enum.GetValues(typeof(T));
		T V = (T)A.GetValue(UnityEngine.Random.Range(0,A.Length));
		return V;
	}
	//****************************GUARDO UNA CLASE EN UN FICHERO CON NOMBRE PROFILE********************************
	public static void Save<T>(T new_save,string Profile){
		string exeFolder = System.IO.Path.GetDirectoryName(Application.dataPath);
		if(!System.IO.Directory.Exists(exeFolder+"/Save"))
			System.IO.Directory.CreateDirectory(exeFolder+"/Save");
		string path=string.Format("{0}/Save/{1}.sav",exeFolder,Profile);	
		FileStream myStream; 
		myStream = new FileStream(path, FileMode.Create); 
		BinaryFormatter binFormatter = new BinaryFormatter(); 
		binFormatter.Serialize(myStream, new_save); 
		myStream.Close();
	}
	//****************************CARGO UNA CLASE DE UN FICHERO DE NOMBRE PROFILE********************************
	public static T Load<T>(string Profile){
		string exeFolder = System.IO.Path.GetDirectoryName(Application.dataPath);
		if(!System.IO.Directory.Exists(exeFolder+"/Save"))
			System.IO.Directory.CreateDirectory(exeFolder+"/Save");
		if(!System.IO.File.Exists(string.Format("{0}/Save/{1}.sav",exeFolder,Profile)))
			return (default(T));
		string path=string.Format("{0}/Save/{1}.sav",exeFolder,Profile);
		BinaryFormatter binFormatter = new BinaryFormatter(); 
		FileStream fs = new FileStream(path, FileMode.Open); 
		return (T)binFormatter.Deserialize(fs);
	}		
	//************************ESTO ES PARA CARGAR ONLINE******************************
	[RPC]
	public IEnumerator LoadLevel(string level){
		lastLevelPrefix++;
		Debug.Log ("Last Level Prefix " + (lastLevelPrefix - 1).ToString () + " New Level Prefix " + lastLevelPrefix);
		// There is no reason to send any more data over the network on the default channel,
		// because we are about to load the level, thus all those objects will get deleted anyway
		//Network.SetSendingEnabled(0, false);    
		
		// We need to stop receiving because first the level must be loaded first.
		// Once the level is loaded, rpc's and other state update attached to objects in the level are allowed to fire
		Network.isMessageQueueRunning = false;
		
		// All network views loaded from a level will get a prefix into their NetworkViewID.
		// This will prevent old updates from clients leaking into a newly created scene.
		Network.SetLevelPrefix(lastLevelPrefix);
		Application.LoadLevel(level);
		yield return 0;
		yield return 0;
		
		// Allow receiving data again
		Network.isMessageQueueRunning = true;
		// Now the level has been loaded and we can start sending out data to clients
		//Network.SetSendingEnabled(0, true);
		
		
		foreach (GameObject go in FindObjectsOfType(typeof(GameObject)))
			go.SendMessage("OnNetworkLoadedLevel", SendMessageOptions.DontRequireReceiver); 
	}
	[System.Serializable]
	public struct datosDeUnPlaneta{
		public vec3[] vertices;
		public int[][] triangulos;
		public tipoEcosistema eco;
		public string[] materiales;
	}
}
