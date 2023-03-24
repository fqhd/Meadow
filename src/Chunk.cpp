#include "Chunk.hpp"

Chunk::Chunk() {
	m_vaoID = 0;
	m_vboID = 0;
}

void Chunk::init() {
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(ChunkVertex), 0);
	glVertexAttribIPointer(1, 3, GL_UNSIGNED_BYTE, sizeof(ChunkVertex), (void*)3);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(ChunkVertex), (void*)6);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Chunk::pushData(ChunkVertex* vertices, unsigned int numVertices) {
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ChunkVertex) * numVertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_numVertices = numVertices;
}

unsigned int Chunk::getNumVertices() {
	return m_numVertices;
}

void Chunk::render() {
	glBindVertexArray(m_vaoID);
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	glBindVertexArray(0);
}

void Chunk::destroy() {
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(1, &m_vboID);
}