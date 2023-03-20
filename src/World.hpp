#pragma once

#include "Chunk.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include <cstdint>

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
	void createNew(const std::string& name);
	void render(Camera& _camera);
	Block getBlock(int _x, int _y, int _z);
	void setBlock(int _x, int _y, int _z, Block _block);
	void destroy();
	std::string getName();

	int load(const std::string& path);
	void save();

private:

	// Utility functions
	void updateMeshes();
	void generateMesh();
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

	//We keep vertices so we dont have to reallocate memory every time we want to generate a chunk
	Shader m_shader;

	Chunk m_chunk;
	Block* m_data = nullptr;
	std::string m_name;

};