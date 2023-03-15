#pragma once

#include <GLAD/glad.h>
#include <vector>
#include <glm/glm.hpp>

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

struct GUIVertex {
	GUIVertex() {
		x = 0.0f;
		y = 0.0f;
		u = 0.0f;
		v = 0.0f;
	}
	GUIVertex(float _x, float _y, float _u, float _v, const ColorRGBA8& _color) {
		x = _x;
		y = _y;
		u = _u;
		v = _v;
		color = _color;
	}
	float x;
	float y;
	float u;
	float v;
	ColorRGBA8 color;
};


class SpriteBatch {
public:

	void init(GLuint textureID);
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, const ColorRGBA8& color); // Adds sprite to draw call
	void batch(); // Batches all the sprites together(sends them to the GPU under one vao)
	void render(); // Renders the sprites
	void destroy();

private:

	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_textureID;
	GLuint m_vertexCount;

	std::vector<GUIVertex> m_vertices;
};
