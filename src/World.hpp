#pragma once

#include "Chunk.hpp"
#include "Camera.hpp"
#include <cstdint>
#include "Shader.hpp"

struct Block {
	Block(){}
	Block(GLubyte _r, GLubyte _g, GLubyte _b, bool _visible) {
		r = _r;
		g = _g;
		b = _b;
		visible = _visible;
	}
	GLubyte r;
	GLubyte g;
	GLubyte b;
	bool visible;
};

class World {
public:

	void init(Block* data, unsigned int worldSize);
	Block getBlock(int _x, int _y, int _z);
	void setBlock(int _x, int _y, int _z, Block _block);
	void destroy();
	unsigned int getWorldSize();
	void updateMeshes();
	void render(Camera& camera);

	void save();

	Chunk* getChunk(int x, int y, int z);

private:

	// Utility functions
	void generateMesh(int chunkX, int chunkY, int chunkZ);
	void addBlock(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);
	bool isBlockInLocalWorld(int _x, int _y, int _z) const;

	// Mesh generation functions
	std::vector<ChunkVertex> m_vertices;
	void addTopFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);
	void addBottomFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);
	void addRightFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);
	void addLeftFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);
	void addFrontFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);
	void addBackFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b);

	Block* m_data = nullptr;
	Chunk* m_chunks = nullptr;
	Shader m_shader;
	unsigned int m_worldSize;


};