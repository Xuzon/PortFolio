#include "Snake.h"
#include "../RenderThread/RenderThread.h"

Snake::Snake(Point& startPoint, int startBoxes, Color color)
{

}

bool Snake::update() 
{
	//float deltaTime = Logic::GetDeltaTimeMicroSecs() / 1000000.0f;

	//float incrX = _speedX * 0.01;// deltaTime;
	//float incrY = _speedY * 0.01;// deltaTime;

	//_x += (int)incrX;
	//_y += (int)incrY;

	//if (_x < _x1) { _x = _x1; _speedX *= -1; }
	//if (_x > _x2) { _x = _x2; _speedX *= -1; }
	//if (_y < _y1) { _y = _y1; _speedY *= -1; }
	//if (_y > _y2) { _y = _y2; _speedY *= -1; }

	return true;
}

void Snake::render(RenderThread& renderThread) 
{

	CommandBuffer buffer;
	buffer.type = DRAW_BOXES;
	buffer.color = _color.toARGB();
	buffer.boxes = body;
	renderThread.EnqueueAction(buffer);
} // render
