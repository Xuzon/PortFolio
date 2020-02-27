#ifndef __Platform_H
#define __Platform_H

/**
	Funciones básicas de la plataforma.

	Todas son estáticas; también podría haber sido un singleton.
*/
class Platform {
public:

	static bool Init();
	static void Release();
	static bool Tick();
};

#endif // __Platform_H
