/*----------------------------------------------------------------------------------------------------------------------*/
// Fichero DemoApp.h
/*----------------------------------------------------------------------------------------------------------------------*/
#ifndef _DEMOAPP_H_
#define _DEMOAPP_H_

// Includes
#include "windows.h"
#include "../Math/Vector3D.h"

// defines
#define MAX_DYNAMICS 1000

// Forwards
class CMap;
class CBall;

// Estructura de evento
struct Button
{
	bool down, up;
};
struct Mouse
{
	Button leftButton, rightButton, middleButton;
	int x, y, oldx, oldy;
	float wheel, oldWheel;
	int deltax, deltay; 
};
struct EVENT {
	Button key[256];
	Mouse mouse;
};

// Definicion de la clase
class CDemoApp
{

public:

	/**
	* Constructor
	*/
	CDemoApp( void );

	/**
	* Destructor
	*/
	~CDemoApp( void );

	// Init
	void Init( void );

	/**
	* Para hacer el control de la aplicación con una estructura de evento externa
	*/
	void Control( float timeDiff );

	/**
	* Render de la aplicación
	*/
	void Render( void );

	// Matamos la demo
	void Finalize( void );

	// Pintar texto...
	void DrawText( unsigned int fontId, float x, float y, float red, float green, float blue, char *formatString, ... );

	// Crea fuente de texto
	bool CreateBitmapFont( LPCWSTR font, int weight, bool italic, bool underline, bool strike, unsigned size, unsigned int &fontId );

	// ¿Hemos finalizado la demo?
	inline bool IsFinished( void ) const { return m_demoFinished; }
	 
	/**
	* Relleno de la estructura de eventos de IO de win
	*/
	void FillEventStruct( UINT uMsg, WPARAM	wParam, LPARAM lParam );

private:

	// Crea una ventana preparada para pintar en OGL
	void CreateGLWindow( LPCWSTR title, int width, int height, bool fullscreen );

	// Destruye la ventana
	void KillGLWindow( void );

	// Limpiado de la estrcutura
	void ClearEventStruct( void );

	// Mapa
	CMap *m_map;
	CBall *m_balls[MAX_DYNAMICS];
	unsigned int m_numBalls;

	// Edicion de lineas en mapa
	bool m_lineEditing;
	CVector3D m_editP0, m_editP1;

	// res de pantalla
	int m_width, m_height;

	// Para la fuente de letra
	unsigned int m_fontId;

	// Variable para el control de los FPS
	float m_lastFPS;

	// Variable para el control de tiempos
	float m_timeElapsed;
	float fixedTimeStep;
	float timeElapsedSinceLastPhysics;
  
	// Booleano que indica si hay que mostrar la ayuda
	bool m_showHelp;

	// Indica si se ha matado la demos
	bool m_demoFinished;

	// Estrcutura de evento actualizada
	EVENT m_event;

	// punteros para contextos de la ventana y dispositivos WIN
	HINSTANCE	m_instance;
	HWND m_wnd;		
	HDC m_dc;
	HGLRC m_rc;
	bool m_fullScreen;
};

#endif _DEMOAPP_H_