#include "Renderer.h"

#include <SDL.h>

#include <iostream>

SDL_Window *Renderer::mWindow = nullptr;

SDL_Renderer *Renderer::mSDLRenderer = nullptr;

int Renderer::sizeX = 0;
int Renderer::sizeY = 0;

bool Renderer::Init(bool windowed, int preferredSizeX, int preferredSizeY) {

	if (mWindow || mSDLRenderer)
		// Ya inicializado
		return false;

	// Creación de la ventana
	Uint32 windowFlags = windowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN;

	if (!windowed) {
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		preferredSizeX = DM.w;
		preferredSizeY = DM.h;
	}

	mWindow = SDL_CreateWindow("Demo", 100, 100, preferredSizeX, preferredSizeY, windowFlags);
	if (mWindow == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Y del renderer
	Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
	if (FLIPT_AT_VSYNC) rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, rendererFlags);
	if (mSDLRenderer == nullptr) {
		SDL_DestroyWindow(mWindow);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Cacheamos la resolución
	sizeX = preferredSizeX;
	sizeY = preferredSizeY;

	return true;
}


void Renderer::Release() {

	if (mSDLRenderer) {
		SDL_DestroyRenderer(mSDLRenderer);
		mSDLRenderer = nullptr;
	}
	if (mWindow) {
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}
}


void Renderer::Clear(uint32_t argb) {
	setColor(argb);
	SDL_RenderClear(mSDLRenderer);
}

void Renderer::PutPixel(int x, int y, uint32_t argb) {

	setColor(argb);
	SDL_RenderDrawPoint(mSDLRenderer, x, y);
}

void Renderer::Present() {
	SDL_RenderPresent(mSDLRenderer);
}

void Renderer::setColor(uint32_t argb) {

	SDL_SetRenderDrawColor(mSDLRenderer,
	                           (argb >> 16) & 0xFF,  // r
	                           (argb >> 8) & 0xFF,   // g
	                            argb & 0xFF,         // b
	                           (argb >> 24) & 0xFF); // a

}
