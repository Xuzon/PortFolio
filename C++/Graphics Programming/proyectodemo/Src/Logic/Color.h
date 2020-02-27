#ifndef __COLOR_H
#define __COLOR_H

#include <cstdint> // uint32_t (desde C++ 11)

struct Color {
	float r, g, b;

	uint32_t toARGB() const {
		uint32_t argb;
		argb = 255; // alfa
		argb <<= 8;
		argb += (int)(r * 255);
		argb <<= 8;
		argb += (int)(g * 255);
		argb <<= 8;
		argb += (int)(b * 255);
		return argb;

	} // toARGB

	static Color BLACK;
	static Color WHITE;
	static Color YELLOW;
	static Color BLUE;
	static Color RED;
	static Color GREEN;
};

#endif
