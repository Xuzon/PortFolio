#pragma once

#include "Point.h"
#include "Color.h"
#include "Entity.h"
#include "../RenderThread/CommandBuffer.h"
#include <list>
class Snake : public Entity
{

public:
	Snake(Point& startPoint, int startBoxes, Color color);

	virtual bool update();
	virtual void render(RenderThread& renderThread);
private:
	std::list<Point> body;
	Color _color;
};

