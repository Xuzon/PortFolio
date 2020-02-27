#pragma once

#include <cstdint>
#include <list>
#include "../Logic/Point.h"

enum RenderCommandType
{
	START_FRAME,
	PUT_PIXEL,
	DRAW_BOX,
	DRAW_BOXES,
	END_FRAME
};

struct CommandBuffer
{
	RenderCommandType type;
	
	int x;
	int y;
	uint32_t color;

	int sizeX;
	int sizeY;

	std::list<Point> boxes;
};

