
// Includes
#include "DemoApp.h"
#include <winuser.h>
#include "GL/glew.h"
#include "Camera.h"
#include "stdio.h"
#include "math.h"
#include "raw_mouse.h"
#include "PhysicsWorld.h"
#include "Mesh.h"
#include "DebugUtils.h"
#include "CharDisplacer.h" // @TODO 6

// Forwards
LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam );

// externs
extern CDemoApp *g_demoPtr;
CCamera g_camera;

__inline float Randf( float min, float max )
{
	return (float)( ((rand()&32767)*(1.0/32767.0))*(max-min) + min );
}

// Defines
#define SHADOW_MAP_SIZE 1024

/*----------------------------------------------------------------------------------------------------------------------*/

CDemoApp::CDemoApp( void )
{
	Init();

	// Deshabilitar VSync
	typedef BOOL ( APIENTRY *PFNWGLSWAPINTERVALPROC )( int );
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = ( PFNWGLSWAPINTERVALPROC )wglGetProcAddress( "wglSwapIntervalEXT" );
	if( wglSwapIntervalEXT )
	{
		wglSwapIntervalEXT( 1 );
	}

	// Init de las fisicas
	m_physicsWorld = new CPhysicsWorld();

	// Init de la lista de meshes
	ZeroMemory( m_meshList, sizeof( CMesh * ) * MAX_MESHES );
	m_numMeshes = 0;

	// Añadimos la geometria de colision del nivel al motor de fisicas
	m_level = m_physicsWorld->AddLevelToPhysicsWorld();

	// @TODO 6
	m_char = m_physicsWorld->CreateCharDisplacer( CVector3D( 2000.0f, 1000.0f, -2000.0f ) );

	// @TODO TRIGGER
	// Creacion del trigger
	m_meshList[m_numMeshes++] = m_physicsWorld->CreateTrigger( CVector3D( 4000.0f, 500.0f, -4000.0f ), CVector3D( 1000.0f, 500.0f, 1000.0f ) ); // @TODO 7
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDemoApp::~CDemoApp( void )
{
	delete m_physicsWorld;
	delete m_level;
	for( unsigned int i=0; i<m_numMeshes; ++i ) {
		delete m_meshList[i];
	}
	destroy_raw_mouse();
	CDebugUtilManager::Destroy();
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Init( void )
{
	m_timeElapsed = m_lastFPS = 0.0f;
	m_demoFinished = false;
	bool fullScreen = false;
	if( fullScreen ) {
		m_width = 1600;
		m_height = 900;
	}
	else {
		m_width = 800;
		m_height = 600;
	}
	CreateGLWindow( L"Demo de fisicas", m_width, m_height, fullScreen );
	g_camera.Init( m_width, m_height );
	glCullFace( GL_BACK );
	glPointSize( 5.5f );
	glFrontFace( GL_CW );
	//glEnable( GL_CULL_FACE );
	glDisable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	bool ok = CreateBitmapFont( L"Courier New", 1, false, false, false, 12, m_fontId );
	if( !ok ) {
		MessageBox( m_wnd, L"Fallo al crear la fuente de la letra", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}
	ClearEventStruct();
	glDisable( GL_TEXTURE );
	glEnable( GL_COLOR_MATERIAL );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_SRC_ALPHA );
	init_raw_mouse( 1, 0, 0 );

	// Setup de la luz
	glEnable( GL_LIGHTING );
	//glEnable( GL_LIGHT0 );
	m_lightDir = CVector3D( 0.0f, -0.5f, 0.5f );
	m_lightDir.Normalize();
	float lighPos[4] = { m_lightDir.x, m_lightDir.y, m_lightDir.z, 0.0f };
	float lighColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_AMBIENT, lighColor );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lighColor );
	glLightfv( GL_LIGHT0, GL_POSITION, lighPos );
	glLightfv( GL_LIGHT0, GL_SPECULAR, lighColor );
	glShadeModel( GL_SMOOTH );
	glColorMask( 1, 1, 1, 1 );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::CreateNewMesh( const CVector3D &pos, const CVector3D &iniVel )
{
	if( m_numMeshes >= MAX_MESHES )
		return;
	CMesh *mesh = m_physicsWorld->AddRigidBodyDynamic( pos, iniVel );
	m_meshList[m_numMeshes++] = mesh;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::CreateNewMesh( void )
{
	if( m_numMeshes >= MAX_MESHES )
		return;

	CMesh *mesh = m_physicsWorld->AddRigidBodyDynamic( g_camera.GetPos(), g_camera.GetDir() * 2000.0f );
	m_meshList[m_numMeshes++] = mesh;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Control( float timeDiff )
{
	m_physicsWorld->Update( timeDiff );
	g_camera.Update( m_event, timeDiff );
	m_timeElapsed += timeDiff;
	if( m_timeElapsed > 1.0f ){
		m_timeElapsed -= 1.0f;
		m_lastFPS = 1.0f / timeDiff;
	}

	/*if( m_event.mouse.leftButton.down ) {
		CVector3D orig, dir, res;
		g_camera.Transform2Dto3D( m_event.mouse.x, m_event.mouse.y, orig, dir );
		if( m_physicsWorld->RayTest( orig, dir, 9999999.9f, res ) ) {
			CDebugUtilManager::GetInstance()->CreateSphere( res, 100.0f, CVector3D( 1.0f, 0.0f, 0.0f ), 1);
		}
	}*/

	if( m_event.mouse.leftButton.down ) {
		CreateNewMesh();
	}

	// @TODO 6
	m_char->Update( timeDiff, m_event );

	CDebugUtilManager::GetInstance()->Update( timeDiff );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Render( void )
{
	glClearColor( 0.8f, 0.8f, 0.8f, 0.5f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	g_camera.ApplyToOGL();
	RenderScene();
	CDebugUtilManager::GetInstance()->Render();
	DrawText( m_fontId, 0.01f * ( float )m_width, 0.02f * ( float )m_height, 1.0f, 0.0f, 0.0f, "FPS %0.2f", m_lastFPS );
	DrawText( m_fontId, 0.01f * ( float )m_width, 0.04f * ( float )m_height, 1.0f, 0.0f, 0.0f, "CamPos(%0.2f,%0.2f,%0.2f)", g_camera.GetPos().x, g_camera.GetPos().y, g_camera.GetPos().z );
	DrawText( m_fontId, 0.01f * ( float )m_width, 0.06f * ( float )m_height, 1.0f, 0.0f, 0.0f, "CamDir(%0.2f,%0.2f,%0.2f)", g_camera.GetDir().x, g_camera.GetDir().y, g_camera.GetDir().z );
	DrawText( m_fontId, 0.01f * ( float )m_width, 0.08f * ( float )m_height, 1.0f, 0.0f, 0.0f, "Dinamics %i", m_numMeshes );

	//fin de render
	SwapBuffers( m_dc );
	ClearEventStruct();
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::RenderScene( void )
{
	float white[4], black[4], grey[4];
	white[0] = white[1] = white[2] = white[3] = 1.0f;
	black[0] = black[1] = black[2] = black[3] = 0.0f;
	grey[0] = grey[1] = grey[2] = grey[3] = 0.2f;

	// render solido
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//Luz
	float lighPos[4] = { m_lightDir.x, m_lightDir.y, m_lightDir.z, 0.0f };
	glLightfv( GL_LIGHT1, GL_POSITION, lighPos );
	glLightfv( GL_LIGHT1, GL_AMBIENT, grey );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, grey );
	glLightfv( GL_LIGHT1, GL_SPECULAR, black );
	glEnable( GL_LIGHT1 );
	glEnable( GL_LIGHTING );
	m_level->Render();
	for( unsigned int i=0; i<m_numMeshes; ++i ) {
		m_meshList[i]->Render();
	}
	// @TODO 6
	m_char->Render();

	// Render de aristas
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	m_level->Render();
	for( unsigned int i=0; i<m_numMeshes; ++i ) {
		m_meshList[i]->Render();
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Finalize( void )
{
	m_demoFinished = true;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::DrawText( unsigned int fontId, float x, float y, float red, float green, float blue, char *formatString, ... )
{
	glColor4f( red, green, blue, 0.5f );
	char finalString[1024];
	char *args = reinterpret_cast<char *>( &formatString ) + sizeof( formatString );
	vsprintf( finalString, formatString, args );			
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	CMatrix4x4 proj2D;
	proj2D.Identity();
	proj2D._11 =  2.0f / static_cast<float>( m_width );
	proj2D._22 =  2.0f / static_cast<float>( m_height );
	proj2D._33 =  1.0f / ( 20000.0f - 1.0f );
	proj2D._43 = -1.0f * ( 1.0f / ( 20000.0f - 1.0f ) );
	glLoadMatrixf( proj2D.v );
	glMatrixMode( GL_MODELVIEW ); // selecciona la matriz
	glPushMatrix(); // la almacena
	glLoadIdentity(); // la resetea
	glRasterPos2f( x - m_width / 2, ( ( m_height / 2 ) - y ) ); // se posiciona en la coordenada x,y,0 que coincide con la posicion en la ventana x,y, siendo la 0,0 la esquina inferior izquierda
	glPushAttrib( GL_LIST_BIT ); // guarda los valores relativos a las Display List
	glListBase( fontId - 32 ); 
	glCallLists( strlen( finalString ), GL_UNSIGNED_BYTE, finalString );
	glPopAttrib();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix(); // restauramos la matriz de projeccion
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix(); // restauramos la matriz de vista
}

/*----------------------------------------------------------------------------------------------------------------------*/

bool CDemoApp::CreateBitmapFont( LPCWSTR font, int weight, bool italic, bool underline, bool strike, unsigned size, unsigned int &fontId )
{
	int height = -MulDiv( size, GetDeviceCaps( m_dc, LOGPIXELSY ), 72 );
	HFONT hFont = ::CreateFont( height, 0, 0, 0, FW_BOLD, italic, underline, strike,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font );
	if ( hFont == NULL)
		return false;
	fontId = glGenLists( 215 );
	HFONT oldfont = static_cast<HFONT>( SelectObject( m_dc, hFont ) );          
	wglUseFontBitmaps( m_dc, 32, 215, fontId );
	SelectObject( m_dc, oldfont );
	DeleteObject( hFont );									
	return true;
}
/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::FillEventStruct( UINT uMsg, WPARAM	wParam, LPARAM lParam )
{
	switch( uMsg ) {
		case WM_KEYDOWN:
			m_event.key[wParam].down = true;
			break;					
		case WM_KEYUP:
			m_event.key[wParam].up = true;
			break;		
		case WM_MOUSEWHEEL:
			m_event.mouse.wheel = static_cast<short>( HIWORD( wParam ) );
			if( m_event.mouse.oldWheel == -999999 ) {
				m_event.mouse.oldWheel = m_event.mouse.wheel;
			}
			break;
		case WM_MOUSEMOVE:
			m_event.mouse.x = LOWORD( lParam );
			m_event.mouse.y = HIWORD( lParam );	
			if( m_event.mouse.oldy == -999999 ) {
				m_event.mouse.oldy = m_event.mouse.y;
				m_event.mouse.oldx = m_event.mouse.x;
			}
			break;
		case WM_LBUTTONDOWN:
			m_event.mouse.leftButton.down = true;
			return;
			break;
		case WM_RBUTTONDOWN:
			m_event.mouse.rightButton.down = true;
			break;
		case WM_RBUTTONUP:
			m_event.mouse.rightButton.up = true;
			break;
		case WM_LBUTTONUP:
			m_event.mouse.leftButton.up = true;
			break;
		case WM_MBUTTONDOWN:
			m_event.mouse.middleButton.down = true;
			break;
		case WM_MBUTTONUP:
			m_event.mouse.middleButton.up = true;
			break;    
		case WM_INPUT: 
			add_to_raw_mouse_x_and_y((HRAWINPUT)lParam);
			m_event.mouse.deltax = get_raw_mouse_x_delta(0);
			m_event.mouse.deltay = get_raw_mouse_y_delta(0);
			break;
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::InitShadowMapTexture( void )
{
	glGenTextures( 1, &m_shadowMapTexture );
	glBindTexture( GL_TEXTURE_2D, m_shadowMapTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	// Proyeccion de la luiz
	m_lightProjectionMatrix.Identity();
	float _far = 2500.0f;
	float _near = 1.0f;
	float fov = 0.8f;
	float aspect = static_cast<float>( m_height ) / static_cast<float>( m_width );
	//m_lightProjectionMatrix._11 =  2.0f / ( SHADOW_MAP_SIZE * 1.0f );
	//m_lightProjectionMatrix._22 =  2.0f / ( SHADOW_MAP_SIZE * 1.0f );
	//m_lightProjectionMatrix._33 =  1.0f / ( _far - _near );
	//m_lightProjectionMatrix._43 =  _near /( _near - _far );
	//m_lightProjectionMatrix._44 =  1.0f;

	float sinFOV2 = sinf( fov / 2 );
	float cosFOV2 = cosf( fov / 2 );
	float w = aspect * ( cosFOV2 / sinFOV2 );
	float h =   1.0f  * ( cosFOV2 / sinFOV2 );
	float Q = _far / ( _far - _near );
	//memset( &m_lightProjectionMatrix, 0, sizeof( CMatrix4x4 ) );
	m_lightProjectionMatrix._11 = w;
	m_lightProjectionMatrix._22 = h;
	m_lightProjectionMatrix._33 = Q;
	m_lightProjectionMatrix._34 = 1.0f;
	m_lightProjectionMatrix._43 = -Q * _near;


	// Vista de la luz
	CVector3D up( 0.0f, 1.0f, 0.0f );
	CVector3D pos = CVector3D( 0.0f, 0.0f, 200.0f ) - m_lightDir * 2000.0f;
	CVector3D right;
	right.Cross( up, m_lightDir );
	right.Normalize();
	up.Cross( m_lightDir, right );
	up.Normalize();
	m_lightViewMatrix.Identity();
	m_lightViewMatrix._14 = m_lightViewMatrix._21 = m_lightViewMatrix._34 = 0.0f;
	m_lightViewMatrix._44 = 1.0f;
	m_lightViewMatrix._11 = right.x;
	m_lightViewMatrix._21 = right.y;
	m_lightViewMatrix._31 = right.z;
	m_lightViewMatrix._41 = -( right * pos );
	m_lightViewMatrix._12 = up.x;
	m_lightViewMatrix._22 = up.y;
	m_lightViewMatrix._32 = up.z;
	m_lightViewMatrix._42 = -( up * pos );
	m_lightViewMatrix._13 = m_lightDir.x;
	m_lightViewMatrix._23 = m_lightDir.y;
	m_lightViewMatrix._33 = m_lightDir.z;
	m_lightViewMatrix._43 = -( m_lightDir * pos );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::CreateGLWindow( LPCWSTR title, int width, int height, bool fullscreen )
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;		
	DWORD		dwStyle;
	RECT		WindowRect;				
	WindowRect.left = static_cast<long>( 0 );
	WindowRect.right = static_cast<long>( width );	
	WindowRect.top = static_cast<long>( 0 );				
	WindowRect.bottom = static_cast<long>( height );
	m_fullScreen = fullscreen;

	// Obtenemos una instancia para nuestra ventana
	m_instance = GetModuleHandle( NULL );

	// y rellenamos la estructura de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= static_cast<WNDPROC>( WndProc );
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_instance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= L"OpenGL";

	// Intentamos registrar la ventana
	if( !RegisterClass( &wc ) ) {
		MessageBox( NULL, L"Fallo registrando la clase.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;											
	}

	// Startup para el fullscreen si procede
	if( fullscreen ) {
		DEVMODE dmScreenSettings;
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
		dmScreenSettings.dmSize = sizeof( dmScreenSettings );
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Intentamos poner el nuevo modo. NOTA: CDS_FULLSCREEN se encarga de la barra de inicio
		if ( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {

			// Si falla, proponemos iniciar en modo ventana
			if ( MessageBox( NULL, L"El modo a pantalla completa ha fallado. Quieres usar modo ventana?"
				, L"MasterDemo", MB_YESNO | MB_ICONEXCLAMATION ) == IDYES ) {
					fullscreen = false;		
			}

			// Se elige cerrar el programa
			else {
				MessageBox( NULL, L"Abortando programa.", L"ERROR", MB_OK | MB_ICONSTOP );
				return;									
			}
		}
	}

	// Si seguimos en fullscreen
	if ( fullscreen ) {

		// Asignanos los estilos pertinentes
		dwExStyle = WS_EX_APPWINDOW;  
		dwStyle = WS_POPUP;  

		// Ocultamos el puntero del ratón
		//ShowCursor( FALSE );
	}

	// Si no, simplemente asignamos los estilos
	else {
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;
	}

	// Obtenemos la estructura del tamaño de la ventana adaptada a los tamaños que queremos
	AdjustWindowRectEx( &WindowRect, dwStyle, FALSE, dwExStyle );		

	// Creamos la ventana
	if ( !( m_wnd = CreateWindowEx(	dwExStyle,		// Extended Style 
		L"OpenGL",							          // Nombre de la clase de la ventana
		title,								          // Título
		dwStyle |							          // Style
		WS_CLIPSIBLINGS |					      // Style requerido por Windows (...)
		WS_CLIPCHILDREN,					      // Style requerido por Windows (...)
		0, 0,								            // Posición de la ventana
		WindowRect.right-WindowRect.left,	// Anchura de la ventana
		WindowRect.bottom-WindowRect.top,	// Altura de la ventana
		NULL,								// Sin ventana padre
		NULL,								// Sin menú
		m_instance,				  // Instancia
		NULL ) ) ) {				// No le pasamos nada a WM_CREATE

			// Si algo va mal, salimos
			KillGLWindow();		          						
			MessageBox( NULL, L"Error en la creación de la ventana.", L"ERROR", MB_OK | MB_ICONEXCLAMATION ); 
			return;
	}                           

	// Mediante esta estructura indicamos a Windows como queremos que el formato de pixels.
	static	PIXELFORMATDESCRIPTOR pfd =	 {
		sizeof(PIXELFORMATDESCRIPTOR),		// Tamaño de la estructura
		1,											          // Número de versión
		PFD_DRAW_TO_WINDOW |						  // El formato debe soportar ventana
		PFD_SUPPORT_OPENGL |						  // El formato debe soportar OpenGL
		PFD_DOUBLEBUFFER,							    // Debe soportar doble buffer
		PFD_TYPE_RGBA,								    // Formato RGBA
		32,					          // Profundidad del color
		0, 0, 0, 0, 0, 0,							    // Bits de color ignorados
		0,											          // Sin buffer para alpha
		0,											          // Shift Bit Ignored
		0,											          // Sin buffer de acumulación
		0, 0, 0, 0,									      // bits de acumulación ignorados
		24,											          // Z-buffer de 16 bits
		0,											          // Sin stencil buffer
		0,											          // Sin buffer auxiliar
		PFD_MAIN_PLANE,								    // Es la capa principal de pintado
		0,											          // Reservado (...)
		0, 0, 0										        // Mascaras de capa ignoradas (...)
	};

	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	// Obtenemos el Device Context
	if ( !( m_dc = GetDC( m_wnd ) ) ) {
		KillGLWindow();								
		MessageBox(NULL,L"No se puede crear un contexto de dispositivo para OpenGL.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;								// Return FALSE
	}

	// Intentamos obtener un formato de pixel que se adapte a nuestras peticiones
	if (!(PixelFormat=ChoosePixelFormat( m_dc, &pfd ) ) ) {
		KillGLWindow();								
		MessageBox(NULL,L"No se puede encontrar un PixelFormat que se adapte a nuestras peticiones.", 
			L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;								
	}

	// Intentamos hacer efectivo ese pixelformat
	if( !SetPixelFormat( m_dc, PixelFormat, &pfd ) ) {
		KillGLWindow();								
		MessageBox( NULL, L"No podemos asignar el PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;								
	}

	// Intentamos crear un contexto de render
	if ( !( m_rc = wglCreateContext( m_dc ) ) ) {
		KillGLWindow();								
		MessageBox( NULL, L"No se pude crear un  contexto de render para OpenGL.", 
			L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;								
	}

	// Intentamos activar el contexto de render
	if( !wglMakeCurrent( m_dc, m_rc ) ) {
		KillGLWindow();								
		MessageBox( NULL, L"No se puede activar el contexto de render OpenGL.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return;								
	}

	// Mostramos la ventana y le damos el foco
	ShowWindow(m_wnd,SW_SHOW);
	SetForegroundWindow(m_wnd);
	SetFocus(m_wnd);									
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::KillGLWindow( void )
{
	// Si estamos en fullscreen
	if ( m_fullScreen ) {
		ChangeDisplaySettings( NULL, 0 );					// Cambiamos al escritorio
		ShowCursor( TRUE );								        // Enseñamos el puntero del ratón
	}

	// Si tenemos un contexto de render
	if ( m_rc ) {

		// release del contexto de dispositivo y del contexto de render
		if ( !wglMakeCurrent( NULL, NULL ) ) 
			MessageBox( NULL, L"Ha fallado el release del DC y el RC.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		if ( !wglDeleteContext( m_rc ) ) 
			MessageBox( NULL, L"Ha fallado el release del contexto de render.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

		// Lo ponemos a NULL
		m_rc = NULL;										
	}

	// Release del DC DC
	if ( m_dc && !ReleaseDC( m_wnd, m_dc ) ) {
		MessageBox( NULL, L"Ha fallado el release de DC.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		m_dc = NULL;										
	}

	// Destrucción de la ventana
	if ( m_wnd && !DestroyWindow( m_wnd ) ) {
		MessageBox( NULL, L"No se puede destruir la ventana.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		m_wnd = NULL;										
	}

	// Desregistramos la clase de la ventana
	if ( !UnregisterClass( L"OpenGL", m_instance ) ) {
		MessageBox( NULL, L"No se puede desregistrar la clase de la ventana.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		m_instance = NULL;									
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::ClearEventStruct( void )
{
	for( unsigned int i=0; i<256; ++i ){
		m_event.key[i].down = false;
		m_event.key[i].up = false;
	}
	m_event.mouse.leftButton.down = false;
	m_event.mouse.leftButton.up = false;
	m_event.mouse.rightButton.down = false;
	m_event.mouse.rightButton.up = false;
	m_event.mouse.middleButton.down = false;
	m_event.mouse.middleButton.up = false;
	m_event.mouse.deltax = 0;
	m_event.mouse.deltay = 0;

	if( m_event.mouse.oldy != -999999 || m_event.mouse.oldx != -999999 ){
		m_event.mouse.oldy = m_event.mouse.y;
		m_event.mouse.oldx = m_event.mouse.x;
	}
	m_event.mouse.wheel = 0.0f;
}

/*----------------------------------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam )
{
	// Filtramos los mensajes de Windows
	g_demoPtr->FillEventStruct( uMsg, wParam, lParam );

	// Verificamos mensajes de Windows
	switch( uMsg ) {
		case WM_ACTIVATE:
			return 0;				
		case WM_SYSCOMMAND:
			switch( wParam ) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:		
					return 0;							
					break; 
			}
			break;
		case WM_CLOSE:
			PostQuitMessage( 0 );	
			exit( 0 );
			g_demoPtr->Finalize();
			return 0;
		case WM_KEYDOWN:
			break;					
		case WM_KEYUP:
			return 0;				
		case WM_MOUSEMOVE:
			break;
		case WM_SIZE:
			return 0;								
	}

	// Pasamos todos los mensajes no tratados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

/*----------------------------------------------------------------------------------------------------------------------*/
