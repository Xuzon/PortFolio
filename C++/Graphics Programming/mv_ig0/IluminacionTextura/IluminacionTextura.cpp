/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include "glaux.h"		// Header File For The Glaux Library
#include "glut.h"		// Header File For The Glaux Library
#include <math.h>
#include <stdio.h>
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


//Estado de la aplicación y de la ventana
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
int		ancho, alto;		// Ancho y alto del area cliente

//Estado del Interfaz de Usuario
int		Mx=0;				//Posición de último click de Mouse
int		My=0;
bool	LClick=FALSE;		//Estado de los Botones del Mouse
bool	RClick=FALSE;
bool	keys[256];			//Estado del teclado

//Estado de la escena. Traslaciones y Rotaciones de objeto y cámara
float x=0.0;				//Posicion del objeto
float y=0.0;
float z=0.0;
float Rx, Ry;				//Rotación del objeto
float Cz = 1000.0;			//Distancia de la cámara
float RCx, RCy;				//Rotación de la Cámara

//LUCES
GLfloat pos0[] = { 1000.0,1000.0,1000.0,1.0 };
GLfloat pos1[] = { 200.0,0.0,0.0,1.0 };
GLfloat ambientLight[] = { 0.1,0.1,0.1,1.0 };
GLfloat ambiente0[] = { 0,0,0,1.0 };
GLfloat difuso0[] = { 0.8,0.0,0.0,1.0 };
GLfloat difuso1[] = { 0.0,0.8,0.0,1.0 };
GLfloat especular0[] = { 1.0,1.0,1.0,1.0 };
//MATERIAL
GLfloat ambientMaterial[] = { 0.5,0.2,0.0,1.0 };
GLfloat difusoMaterial[] = { 1.0,0.4,0.0,1.0 };
GLfloat especMaterial[] = { 1.0,1.0,1.0,1.0 };
GLfloat shininess = 50.0;
bool luz0, luz1;
GLuint	texture[1];

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

//FUNCIONES AUXILIARES
GLuint LoadBMPTexture(const char * imagepath)	// Load Bitmaps And Convert To Textures
{

	// Lectura de información del encabezado del archivo
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Información RGB
	unsigned char * data;

	// Apertura del archivo
	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }
	//Leer cabecera
	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Lectura de los enteros desde el arreglo de bytes
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Algunos archivos BMP tienen un mal formato, así que adivinamos la información que falta
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : un byte por cada componente Rojo (Red), Verde (Green) y Azul(Blue)
	if (dataPos == 0)      dataPos = 54; // El encabezado del BMP está hecho de ésta manera

	// Se crea un buffer
	data = new unsigned char[imageSize];

	// Leemos la información del archivo y la ponemos en el buffer
	fread(data, 1, imageSize, file);

	//Todo está en memoria ahora, así que podemos cerrar el archivo
	fclose(file);

	// Se Crea una textura OpenGL
	GLuint textureID;
	glGenTextures(1, &textureID);

	// Se "Ata" la nueva textura : Todas las futuras funciones de texturas van a modificar esta textura
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Se le pasa la imagen a OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	//glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	float colorenv[4] = { 0.5,0.5,0.5,1.0 };
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, colorenv);

	return textureID;
	//	if (TextureImage[0]=LoadBMP("Data/NeHe.bmp"))

}



void DibEjes(bool Mundo)
{   glEnable(GL_LINE_STIPPLE);
	glLineWidth(2);
	glBegin(GL_LINES);								// Drawing Using Triangles
		if (Mundo)
			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		else
			glColor3f(0.0f,1.0f,1.0f);
		glVertex3f( 0.0f, 0.0f, 0.0f);					// Top
		glVertex3f(150.0f,0.0f, 0.0f);					// Bottom Left
		if (Mundo)
			glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Red
		else
			glColor3f(1.0f,0.0f,1.0f);
		glVertex3f( 0.0f, 0.0f, 0.0f);					// Top
		glVertex3f(0.0f,150.0f, 0.0f);					// Bottom Left
		if (Mundo)
			glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Red
		else
			glColor3f(1.0f,1.0f,0.0f);
		glVertex3f( 0.0f, 0.0f, 0.0f);					// Top
		glVertex3f(0.0f,0.0f, 150.0f);					// Bottom Left
	glEnd();											// Finished Drawing The Triangle
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(1);
}

void DibMalla(float tamaño)
{	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);	
	 glColor3f(0.3f,0.3f,0.3f);					
	 for (int i=-10;i<=10;i++)
	 {
		glVertex3f( -tamaño*10.0, 0.0f, (float)i*tamaño);
		glVertex3f(tamaño*10.0,0.0f, (float)i*tamaño);	
		glVertex3f((float)i*tamaño,0.0f, -tamaño*10.0);	
		glVertex3f((float)i*tamaño,0.0f, tamaño*10.0);	
	 }
	glEnd();
}
void TeclaModo()
{//if (keys[VK_UP]) z=z-10;
 //if (keys[VK_DOWN]) z=z+10;
 if (keys[VK_UP]) {
	 z = z - cos(-Ry * 3.1416 / 180.0) * 10;
	 x = x + sin(-Ry * 3.1416 / 180.0) * 10;
 }
 if (keys[VK_DOWN]) {
	 z = z + cos(-Ry * 3.1416 / 180.0) * 10;
	 x = x - sin(-Ry * 3.1416 / 180.0) * 10;
 }
 if (keys[VK_LEFT]) x=x-10;
 if (keys[VK_RIGHT]) x=x+10;
 if (keys[VK_PRIOR]) y=y+10;
 if (keys[VK_NEXT]) y=y-10;
 if (keys[VK_HOME]) Cz=Cz-10;
 if (keys[VK_END]) Cz=Cz+10;
 if (keys['1']) 
	 luz1 = !luz1;
 if (keys['0']) 
	 luz0 = !luz0;
}

//FUNCIONES OGL
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{	if (height==0){	height=1;}
    ancho=width;alto=height;
	//glViewport(0, 0, ancho, alto);
	
}
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glLineStipple(1, 0xFF08);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix
    glOrtho(-1000.0,1000.0,-1000.0,1000.0,1000.0,-1000.0);
    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();
	glViewport(0,0,ancho,alto);	

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difuso0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambiente0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difuso1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, especular0);
	//glLightfv (GL_LIGHT0, GL_POSITION, pos0);
	//glEnable(GL_LIGHT0);
	//glLightfv (GL_LIGHT0, GL_POSITION, pos1);
	//glEnable(GL_LIGHT1);
	luz0 = true;
	luz1 = true;
	return TRUE;	
	// Initialization Went OK
}
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{ glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
  //VP Sup.Izq.
  glViewport(0, alto/2, ancho/2, alto/2);
  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();									// Reset The Projection Matrix
  //glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, 1000.0, -1000.0);
  gluPerspective(80.0, (float)ancho/(float)alto, 100.0, 2000.0);
  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
  //Cámara
  glLoadIdentity();
  //gluLookAt(100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 0.0, -Cz);//Cz
  glRotatef(RCx, 1.0, 0.0, 0.0);
  glRotatef(RCy, 0.0, 1.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, pos0);
  //Mundo
  DibMalla(100);
  DibEjes(false);
  //Objeto
  glTranslatef(x, y, z);
  glRotatef(Rx, 1.0, 0.0, 0.0);
  glRotatef(Ry, 0.0, 1.0, 0.0);
  
  glLightfv(GL_LIGHT1, GL_POSITION, pos1);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 100.0, 0.0);
  DibEjes(true);
  //glColor3f(1.0f, 1.0f, 1.0f); 
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, difusoMaterial);
  glMaterialfv(GL_FRONT, GL_SPECULAR, especMaterial);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  if (luz0) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
  if (luz1) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
  glutSolidTeapot(100);

  //VP Sup.Der
  glViewport(ancho/2, alto/2, ancho/2, alto/2);
  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();									// Reset The Projection Matrix
  glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, 1000.0, -1000.0);
  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
  glLoadIdentity();
  gluLookAt(0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
  DibMalla(100);
  DibEjes(false);
  glTranslatef(x, y, z);
  glRotatef(Rx, 1.0, 0.0, 0.0);
  glRotatef(Ry, 0.0, 1.0, 0.0);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 100.0, 0.0);
  DibEjes(true);
  glColor3f(1.0f, 1.0f, 1.0f);
  glutWireTeapot(100);

  //VP Inf.
  glViewport(0,0, ancho, alto/2);
  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();									// Reset The Projection Matrix
  //glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, 1000.0, -1000.0);
  gluPerspective(80.0, (float)ancho / (float)alto, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
  glLoadIdentity();
  glTranslatef(0.0, -200.0, -200.0);
   glRotatef(-Ry, 0.0, 1.0, 0.0);
   glRotatef(-Rx, 1.0, 0.0, 0.0);
   glTranslatef(-x, -y, -z);
    DibMalla(100);
    DibEjes(false);
   glTranslatef(x, y, z);
   glRotatef(Rx, 1.0, 0.0, 0.0);
   glRotatef(Ry, 0.0, 1.0, 0.0);
  //Dibujo Objeto 
   glRotatef(90.0, 0.0, 1.0, 0.0);
   glTranslatef(0.0, 100.0, 0.0);
    DibEjes(true);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glutWireTeapot(100);

  return TRUE;										// Keep Going
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}
//FUNCIONES WINDOWS
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			TeclaModo();
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		case WM_LBUTTONDOWN:
		{
			Mx = LOWORD(lParam);
			My = HIWORD(lParam);
			LClick = TRUE;
			return 0;
		}
		case WM_LBUTTONUP:
		{
			LClick = FALSE;
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			Mx = LOWORD(lParam);
			My = HIWORD(lParam);
			RClick = TRUE;
			return 0;
		}
		case WM_RBUTTONUP:
		{
			RClick = FALSE;
			return 0;
		}
		case WM_MOUSEMOVE:
		{   int nMx,nMy;
			nMx = LOWORD(lParam);
			nMy = HIWORD(lParam);
			if (LClick)
			{	 Rx= Rx+(float)(nMy-My);
				 Ry= Ry+(float)(nMx-Mx);
			     Mx=nMx;My=nMy;
				 if (Rx>=360.0f)	Rx -= 360.0f;
				 if (Rx<=-360.0f)	Rx += 360.0f;
				 if (Ry>=360.0f)	Ry -= 360.0f;
				 if (Ry<=-360.0f)	Ry += 360.0f;
			}
			if (RClick)
			{	 RCx=RCx+(float)(nMy-My);
				 RCy=RCy+(float)(nMx-Mx);
				 Mx=nMx;My=nMy;
				 if (RCx>=360.0f)	RCx -= 360.0f;
				 if (RCx<=-360.0f)	RCx += 360.0f;
				 if (RCy>=360.0f)	RCy -= 360.0f;
				 if (RCy<=-360.0f)	RCy += 360.0f;
			}
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	//{
		fullscreen=FALSE;							// Windowed Mode
	//}

	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's Color Tutorial",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's Color Tutorial",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
