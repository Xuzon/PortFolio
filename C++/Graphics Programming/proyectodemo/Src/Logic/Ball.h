#ifndef __WALL_H
#define __WALL_H

#include "Entity.h"
#include "Color.h"

class RenderThread;
class Ball : public Entity {
public:

	/**
	 * Constructor. Define el rango de movimiento y el color de la bola.
	 * La coloca en el centro y con tamaño 20x20.
	 * @param color Color del muro
	 */
	Ball(int x1, int y1, int x2, int y2, Color color);

	/**
	 * Destructor. Quita la entidad del mundo de colisión.
	 */
	~Ball();

	// En píxeles por segundo
	void setSpeed(int speedX, int speedY) {
		_speedX = speedX; _speedY = speedY;
	}

	void setPos(int x, int y);

	virtual bool update();
	virtual void render(RenderThread& renderThread);

private:

	int _x1, _y1, _x2, _y2;

	int _x, _y;
	int _speedX, _speedY; // píxeles por segundo
	int _medioAncho, _medioAlto;
	Color _color;

}; // class Wall

#endif // __WALL_H