#ifndef __LOGIC_H
#define __LOGIC_H

#include <vector>	// Vector de entidades
#include "../Platform/Clock.h"

class Entity;
class RenderThread;

/**
 * Gestor de la l�gica del "juego.
 *
 * Todos los m�todos son est�ticos, aunque podr�a haberse hecho con un singleton.
 *
 * Tiene los m�todos Init() y Release() para gestionar el ciclo de vida, y el m�todo
 * Tick() que avanza un paso de simulaci�n de la l�gica.
 *
 * Tiene tambi�n el m�todo Render() que recorre toda la "escena" (entidades)
 * y las manda dibujar.
 */

class Logic {
public:

	// Gesti�n de vida
	static bool Init();
	static void Release();

	// Actualizaci�n del juego
	static void Tick();

	// Renderizado
	static void Render(RenderThread& renderThread);

	// -------------------------
	// Acceso al tiempo transcurrido
	// -------------------------

	/**
	 * Devuelve la "hora" del sistema congelada en el inicio del frame en curso.
	 * @return Devuelve la hora en microsegundos.
	 */
	static uint64_t GetTimeMicroSecs() { return _time; }

	/**
	* Devuelve la duraci�n del frame en curso (en microsegundos).
	* @return Devuelve la duraci�n en microsegundos.
	*/
	static uint64_t GetDeltaTimeMicroSecs() { return _deltaTime; }

	/**
	 * No llamar a mitad de un update.
	 * La clase acepta la responsabilidad del borrado.
	 */
	static void AddEntity(Entity *e);

private:

	static std::vector<Entity*> _entities;

	static void DestroyEntities();

	/**
	 * Instante de tiempo en el que ha comenzado el frame.
	 * En microsegundos
	 */
	static uint64_t _time;

	/**
	 * Longitud del frame en microsegundos.
	 */
	static uint64_t _deltaTime;

	/**
	 * Reloj utilizado para medir el tiempo.
	 */
	static Clock _crono;

}; // class Logic

#endif // __LOGIC_H
