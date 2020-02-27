#ifndef __CLOCK_H
#define __CLOCK_H

#include <chrono>

class Clock {

public:

	/**
	* Constructor.
	*/
	Clock() : _startTime(std::chrono::system_clock::time_point::min()) {
	}

	/**
	* Borra el punto inicial, dejando en "parado" al temporizador.
	*/
	void clear() {
		_startTime = std::chrono::system_clock::time_point::min();
	} // clear

    /**
	 * Devuelve cierto si se ha lanzado la medición de tiempo (se ha llamado previamente
	 * a start()).
	 */
	bool isStarted() const {
		return (_startTime.time_since_epoch() != std::chrono::system_clock::duration(0));
	}

	/**
	* Anota el instante de tiempo actual, como punto de inicio para la medición de tiempo
	* transcurrido.
	*/
	void start() {
		_startTime = std::chrono::system_clock::now();
	}

	/**
	* Devuelve el tiempo transcurrido desde la última llamada a start() en microsegundos
	* o 0 si no se ha llamado nunca.
	*/
	unsigned long getMicros() const {
		if (isStarted()) {
			std::chrono::system_clock::duration diff;
			diff = std::chrono::system_clock::now() - _startTime;
			return (unsigned)(std::chrono::duration_cast<std::chrono::microseconds>(diff).count());
		}
		return 0;
	}

private:

	std::chrono::system_clock::time_point _startTime;

}; // class Clock

#endif // __CLOCK_H
