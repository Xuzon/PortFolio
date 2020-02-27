#include "Platform/Platform.h"
#include "Renderer/Renderer.h"


#include "Platform/Clock.h"

#include "Logic/Logic.h"
#include "Logic/Color.h"

#include "Logic/Ball.h"

// Informe de FPSs
#include <iostream>
#include "RenderThread/RenderThread.h"

// Bucle principal de la aplicación. Recibe la hebra de render a utilizar
// para pintar. Asume todo inicializado previamente.
// Devuelve el número de frames/vueltas dadas por el bucle antes de terminar.
long long MainLoop( RenderThread& renderThread) {

	const unsigned int numFrames = 2000000;
	unsigned int pendingFrames = numFrames;
	uint32_t colorFondo = Color::BLUE.toARGB();

	int numFrame = 0;
	CommandBuffer startFrame;
	CommandBuffer endFrame;
	startFrame.type = START_FRAME;
	endFrame.type = END_FRAME;
	// Bucle principal
	while (Platform::Tick() && pendingFrames--) {
		Logic::Tick();


		renderThread.EnqueueAction(startFrame);
		Logic::Render(renderThread);
		renderThread.EnqueueAction(endFrame);
		while (renderThread.PendingFrames() >= 3);

	} // while bucle principal

	return numFrames - (pendingFrames + 1);
}

int main()
{
	RenderThread renderThread;
	renderThread.Start();
	// Inicialización
	Platform::Init();
	/*
	Renderer::Init(true, 768, 432);	 // En ventana
	/*/
	Renderer::Init(false);		// Pantalla completa
	/**/
	Logic::Init();

	Ball *ball;

	// Bola 1
	ball = new Ball(50, 50, 718, 382, Color::RED);
	ball->setPos(200, 216);
	Logic::AddEntity(ball);

	// Bola 2
	ball = new Ball(50, 50, 718, 382, Color::WHITE);
	ball->setPos(568, 216);
	ball->setSpeed(-200, 200);
	Logic::AddEntity(ball);

	Clock chrono;

	// Bucle principal
	chrono.start();
	long long totalFrames = MainLoop(renderThread);
	long long totalMicros = chrono.getMicros();

	double fps = (double)(totalFrames) * 1000000 / totalMicros;
	std::cout << totalFrames << " frames en " << totalMicros / 1000 << " ms (" << fps << " fps)" << std::endl;

	renderThread.Stop();
	// Limpieza
	Logic::Release();
	Renderer::Release();
	Platform::Release();

    return 0;

} // main

