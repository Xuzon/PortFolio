#include "Ball.h"
#include "Logic.h"
#include "../RenderThread/CommandBuffer.h"
#include "../RenderThread/RenderThread.h"

#include <algorithm> // swap

Ball::Ball(int x1, int y1, int x2, int y2, Color color) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _color(color) {
	if (_x1 > _x2) std::swap(_x1, _x2);
	if (_y1 > _y2) std::swap(_y1, _y2);

	_x = (_x1 + _x2) / 2;
	_y = (_y1 + _y2) / 2;

	_medioAncho = _medioAlto = 20 / 2;
	_medioAlto *= 30;

	_speedX = _speedY = 300;
}

/**
* Destructor. Quita la entidad del mundo de colisión.
*/
Ball::~Ball() {

}

void Ball::setPos(int x, int y) {

	_x = x;
	_y = y;

	_x = std::max(_x, _x1 + _medioAncho);
	_x = std::min(_x, _x2 - _medioAncho);
	_y = std::max(_y, _y1 + _medioAlto);
	_y = std::min(_y, _y2 - _medioAlto);

}

bool Ball::update() {
	float deltaTime = Logic::GetDeltaTimeMicroSecs() / 1000000.0f;

	float incrX = _speedX * 0.01;// deltaTime;
	float incrY = _speedY * 0.01;// deltaTime;

	_x += (int)incrX;
	_y += (int)incrY;

	if (_x < _x1) { _x = _x1; _speedX *= -1; }
	if (_x > _x2) { _x = _x2; _speedX *= -1; }
	if (_y < _y1) { _y = _y1; _speedY *= -1; }
	if (_y > _y2) { _y = _y2; _speedY *= -1; }

	return true;
}

void Ball::render(RenderThread& renderThread) {

	CommandBuffer buffer;
	buffer.type = DRAW_BOX;
	buffer.color = _color.toARGB();
	buffer.x = _x;
	buffer.y = _y;
	buffer.sizeX = _medioAncho;
	buffer.sizeY = _medioAlto;
	renderThread.EnqueueAction(buffer);
} // render
