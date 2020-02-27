// Includes
#include "DemoApp.h"
#include <winuser.h>
#include "../GL/glew.h"
#include "stdio.h"
#include "math.h"
#include "raw_mouse.h"
#include "../Math/Matrix4x4.h"
#include "Map.h"
#include "Ball.h"

// Forwards
LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam );

// externs
extern CDemoApp *g_demoPtr;
extern CPhysics *g_PhysicsPtr;

__inline float Randf( float min, float max )
{
	return (float)( ((rand()&32767)*(1.0/32767.0))*(max-min) + min );
}


/*----------------------------------------------------------------------------------------------------------------------*/

CDemoApp::CDemoApp( void )
{
	Init();
	g_PhysicsPtr = new CPhysics( CVector3D( 0.0f, 0.098f * 2.0f, 0.0f ) );
	m_map = new CMap( m_width, m_height );
	m_numBalls = 0;
	m_balls[m_numBalls++] = new CBall( m_width, m_height, CVector3D( 0.5f, 0.0f, 0.0f ) );
	m_lineEditing = false;

	fixedTimeStep = 1.f / 50.f;
	timeElapsedSinceLastPhysics = 0;
	//VSYNC
	typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
	{
		wglSwapIntervalEXT(0);
	}
}

/*----------------------------------------------------------------------------------------------------------------------*/

CDemoApp::~CDemoApp( void )
{
	destroy_raw_mouse();
	delete m_map;
	for( unsigned int i=0; i<m_numBalls; ++i ) {
		delete m_balls[i];
	}
	delete g_PhysicsPtr;
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Init( void )
{
	m_timeElapsed = m_lastFPS = 0.0f;
	m_demoFinished = false;
	bool fullScreen = false;
	if( fullScreen ) {
		m_width = 1280;
		m_height = 800;
	}
	else {
		m_width = 800;
		m_height = 600;
	}
	CreateGLWindow( L"Fisicas basicas", m_width, m_height, fullScreen );
	
	// Setup de camara
	glCullFace( GL_BACK );
	glPointSize( 50.5f );
	glLineWidth( 10.0f );
	glFrontFace( GL_CW );

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
	glDisable( GL_BLEND );
	init_raw_mouse( 1, 0, 0 );

	// Setup de la luz
	//glEnable( GL_LIGHTING );
	//glShadeModel( GL_SMOOTH );
	//glColorMask( 1, 1, 1, 1 );
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Control( float timeDiff )
{
	m_timeElapsed += timeDiff;
	timeElapsedSinceLastPhysics += timeDiff;
	while (timeElapsedSinceLastPhysics > fixedTimeStep)
	{
		timeElapsedSinceLastPhysics -= fixedTimeStep;
		g_PhysicsPtr->Update(fixedTimeStep);
	}
	if( m_timeElapsed > 1.0f ){
		m_timeElapsed -= 1.0f;
		m_lastFPS = 1.0f / timeDiff;
	}

	if( m_event.mouse.leftButton.down ) {
		m_balls[m_numBalls++] = new CBall( m_width, m_height, CVector3D( (float)m_event.mouse.x / m_width, (float)m_event.mouse.y / m_height, 0.0f ) );
	}


	if (m_event.mouse.rightButton.down)
	{
		m_lineEditing = true;
		m_editP0 = CVector3D((float)m_event.mouse.x / m_width, (float)m_event.mouse.y / m_height, 0.f);
	}

	if (m_lineEditing)
	{
		m_editP1 = CVector3D((float)m_event.mouse.x / m_width, (float)m_event.mouse.y / m_height, 0.f);
	}

	if (m_event.mouse.rightButton.up)
	{
		m_lineEditing = false;
		m_map->AddSegment(m_editP0, m_editP1);
	}
	//	@TODO_BP 5
}

/*----------------------------------------------------------------------------------------------------------------------*/

void CDemoApp::Render( void )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	DrawText( m_fontId, 0.01f * ( float )m_width, 0.02f * ( float )m_height, 1.0f, 0.0f, 0.0f, "FPS %0.2f", m_lastFPS );

	glPolygonMode(GL_FRONT, GL_LINE);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D(0.0, m_width, m_height, 0.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	m_map->Render();

	for( unsigned int i=0; i<m_numBalls; ++i ) {
		m_balls[i]->Render();
	}

	if( m_lineEditing ) {
		//glLineWidth( 5.0f );
		glBegin( GL_LINES );
			glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
			glVertex3f( m_editP0.x * m_width, m_editP0.y * m_height, m_editP0.z );
			glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
			glVertex3f( m_editP1.x * m_width, m_editP1.y * m_height, m_editP1.z );
		glEnd();
	}

	//fin de render
	SwapBuffers( m_dc );
	ClearEventStruct();
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
				, L"PhysXDemo", MB_YESNO | MB_ICONEXCLAMATION ) == IDYES ) {
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
