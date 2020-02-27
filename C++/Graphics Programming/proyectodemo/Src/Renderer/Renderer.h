#ifndef __Renderer_H
#define __Renderer_H

#include <cstdint> // uint32_t (color) C++11

struct SDL_Window;
struct SDL_Renderer;

/**
 * Módulo Renderer usando SDL.
 */
class Renderer {
public:

	/**
	 * Constante que en realidad actúa como constante de precompilador :-) para
	 * decidir si se usará o no sincronización vertical en los flips.
	 */
	static const bool FLIPT_AT_VSYNC = false;

	/**
	 * Inicializa el renderer con SDL. Asume que SDL está ya inicializado.
	 */
	static bool Init(bool windowed = false, int preferredSizeX = 800, int preferredSizeY = 600);
	static void Release();

	static void Clear(uint32_t argb);
	static void PutPixel(int x, int y, uint32_t argb);

	/**
	 * Muestra el siguiente frame, haciendo visible el back buffer construído.
	 * Dependiendo de si la constante FLIPT_AT_VSYNC es o no cierta, el cambio
	 * se sincronizará con el retrazado vertical, en cuyo caso la llamada será bloqueante.
	 */
	static void Present();

	static int getWidthRes() { return sizeX; }
	static int getHeightRes() { return sizeY; }

private:

	static void setColor(uint32_t argb);
	static SDL_Window *mWindow;
	static SDL_Renderer *mSDLRenderer;
	static int sizeX, sizeY;

}; // class RendererPC

#endif // __Renderer_H
