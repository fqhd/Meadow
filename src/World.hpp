#pragma once

#include "Chunk.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include <cstdint>

const int WORLD_WIDTH = 4;
const int WORLD_HEIGHT = 4;

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

	void init();
	void render(Camera& _camera);
	Block getBlock(int _x, int _y, int _z);
	void setBlock(int _x, int _y, int _z, Block _block);
	void destroy();

	void loadWorldFromFile(const std::string& path);
	void saveWorldToFile(const std::string& path);

private:

	// Utility functions
	void updateMeshes();
	void generateMesh(Chunk* chunk);
	void addBlock(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	bool isBlockInLocalWorld(int _x, int _y, int _z);

	// Mesh generation functions
	std::vector<ChunkVertex> m_vertices;
	void addTopFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	void addBottomFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	void addRightFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	void addLeftFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	void addFrontFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	void addBackFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b);
	Chunk* getChunk(int x, int y, int z);

	//We keep vertices so we dont have to reallocate memory every time we want to generate a chunk
	Shader m_shader;
	unsigned int m_data_length = 0;

	Chunk* m_chunks = nullptr;
	Block* m_data = nullptr;

};