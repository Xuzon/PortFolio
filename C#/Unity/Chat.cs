using UnityEngine;
using System.Collections;

public class Chat : MonoBehaviour {
	private bool activarChat;
	private float pantallaY=Screen.height;
	private float altura=Screen.height/10;
	private float ancho = Screen.width/5;
	public Texture2D cajaBackground,escribirBackground;
	public string Texto,textoEnviar;
	public GUIStyle estiloCaja,estiloEscribir;
	private int numEntradas;
	// Use this for initialization
	void Start () {
		DontDestroyOnLoad (gameObject);
		/*estiloCaja = "box";
		estiloEscribir = "box";
		estiloCaja.wordWrap = true;
		estiloEscribir.wordWrap = true;
		estiloCaja.normal.background = cajaBackground;
		estiloEscribir.normal.background = escribirBackground;
		*/
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	public Vector2 scrollPosition= Vector2.zero;
	void OnGUI(){
		if (activarChat) {
			scrollPosition=GUI.BeginScrollView(new Rect(0,pantallaY-4*altura,ancho,3*altura),scrollPosition,new Rect(0,0,ancho,pantallaY));
			//GUI.Box(new Rect(0,pantallaY-4*altura,ancho,3*altura),Texto,estiloCaja);
			GUI.Box(new Rect(0,0,ancho,pantallaY),Texto,estiloCaja);
			GUI.EndScrollView();
			textoEnviar=GUI.TextField (new Rect(0,pantallaY-altura,ancho,altura),textoEnviar,estiloEscribir);
			if(GUI.Button(new Rect(ancho,pantallaY-altura,ancho/3,altura),"Send")){
				GetComponent<NetworkView>().RPC ("sendText",RPCMode.AllBuffered,textoEnviar);
				textoEnviar="";
			}
			if(GUI.Button(new Rect(0,pantallaY-5*altura,ancho,altura),"Quitar Chat"))
				activarChat=false;
		} else {
			if(GUI.Button(new Rect(0,pantallaY-altura,ancho,altura),"Mostrar Chat"))
				activarChat=true;
		}
	}
	[RPC]
	void sendText(string newWord){
		if (numEntradas >= 10) { //si lo almacenado tiene mas de 50 caraceteres sale el nuevo 
			Texto = newWord;
			numEntradas=0;
		}
		else {
			Texto=Texto+"\n"+newWord;
			numEntradas+=1;
		}
	}
}
