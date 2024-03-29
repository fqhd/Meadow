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

	void init();
	void createNew(const std::string& name);
	Block getBlock(int _x, int _y, int _z);
	void setBlock(int _x, int _y, int _z, Block _block);
	void destroy();
	std::string getName();
	void updateMeshes();
	void render(Camera& camera, GLuint depthmap, const glm::mat4& lightSpaceMatrix, const glm::vec3& lightPos);

	int load(const std::string& path);
	void save();

	glm::ivec3 blockOffset;

	Chunk chunk;

private:

	// Utility functions
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

	Block* m_data = nullptr;
	std::string m_name;
	GLuint textureArray;
	Shader m_shader;


};