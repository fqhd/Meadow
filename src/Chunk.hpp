#pragma once

#include <vector>
#include <cstddef>
#include <iostream>
#include "glad.h"

const int CHUNK_WIDTH = 16;

struct ChunkVertex {
	ChunkVertex(GLubyte _x, GLubyte _y, GLubyte _z, GLubyte _r, GLubyte _g, GLubyte _b) {
		x = _x;
		y = _y;
		z = _z;
		r = _r;
		g = _g;
		b = _b;
	}
	GLubyte x, y, z, r, g, b;
};

class Chunk {
public:

	Chunk();
	void init();

	// Utility functions
	void render();
	void pushData(ChunkVertex* vertices, unsigned int numVertices);
	unsigned int getNumVertices();
	void destroy();

	// Public variables
	bool needsMeshUpdate = true;

private:

	// Opengl Variables
	GLuint m_vaoID = 0;
	GLuint m_vboID = 0;
	GLuint m_numVertices = 0;

};
