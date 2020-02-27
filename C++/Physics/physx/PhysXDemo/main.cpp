/*----------------------------------------------------------------------------------------------------------------------*/
// Fichero main.cpp
/*----------------------------------------------------------------------------------------------------------------------*/

// Includes
#include "DemoApp.h"
#include "windows.h"

/*----------------------------------------------------------------------------------------------------------------------*/

CDemoApp *g_demoPtr;
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance,  LPSTR lpCmdLine, int nCmdShow ) {
		MSG msg;
		LONGLONG freq, lastTime, currentTime;
		QueryPerformanceFrequency( ( LARGE_INTEGER *) &freq );
		float timeScale = 1.0f / freq;
		QueryPerformanceCounter( ( LARGE_INTEGER * ) &lastTime ); 
		g_demoPtr = new CDemoApp();
		//HCURSOR c = LoadCursorFromFile( L"cursor.cvk" );
		while( !g_demoPtr->IsFinished() ) {
			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
				//SetCursor(c);
			}
			QueryPerformanceCounter( ( LARGE_INTEGER * ) &currentTime ); 
			g_demoPtr->Control( ( currentTime - lastTime ) * timeScale );
			lastTime = currentTime;
			g_demoPtr->Render();
		}
		delete g_demoPtr;
		return static_cast<int>( msg.wParam );
}

/*----------------------------------------------------------------------------------------------------------------------*/

