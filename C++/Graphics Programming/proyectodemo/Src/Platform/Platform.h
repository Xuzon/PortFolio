#ifndef __Platform_H
#define __Platform_H

/**
	Funciones b�sicas de la plataforma.

	Todas son est�ticas; tambi�n podr�a haber sido un singleton.
*/
class Platform {
public:

	static bool Init();
	static void Release();
	static bool Tick();
};

#endif // __Platform_H
