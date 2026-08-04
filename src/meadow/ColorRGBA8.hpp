#pragma once
#include "glad.h"

struct ColorRGBA8 {
	ColorRGBA8() {}
	ColorRGBA8(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a = 255) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	GLubyte r = 255;
	GLubyte g = 255;
	GLubyte b = 255;
	GLubyte a = 255;
};