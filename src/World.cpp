#include "World.hpp"
#include <fstream>

void World::init(){
	m_data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH];
	memset(m_data, 0, sizeof(Block) * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH);
	chunk.init();
	m_shader.load("chunk");
	blockOffset = glm::ivec3(CHUNK_WIDTH*1024);
}

void World::createNew(const std::string& name) {
	m_name = name;
	memset(m_data, 0, sizeof(Block) * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH);
	for (int i = 0; i < CHUNK_WIDTH; i++) {
		for (int j = 0; j < CHUNK_WIDTH; j++) {
			setBlock(i, 0, j, Block(255, 255, 255, true));
		}
	}
}

void World::destroy(){
	chunk.destroy();
	delete[] m_data;
}

void World::render(Camera& camera, GLuint depthmap, const glm::mat4& lightSpaceMatrix, const glm::vec3& lightPos) {
	m_shader.bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	m_shader.loadUniform("lightPos", lightPos);
	m_shader.loadUniform("lightSpaceMatrix", lightSpaceMatrix);
	m_shader.loadUniform("projection", camera.getProjectionMatrix());
	m_shader.loadUniform("view", camera.getViewMatrix());

	chunk.render();

	m_shader.unbind();
}


int World::load(const std::string& name) {
	m_name = name;
	std::string path = "worlds/" + m_name;
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.good()) {
		std::cout << "World: Could not open file: " << path << std::endl;
		return -1;
	}
	for (int i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH; i++) {
		file.read((char*)&m_data[i], sizeof(Block));
	}
	file.close();

	chunk.needsMeshUpdate = true;

	return 0;
}

void World::save() {
	std::string path = "worlds/" + m_name;
	Block* tmp = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH];
	for (int y = 0; y < CHUNK_WIDTH; y++) {
		for (int z = 0; z < CHUNK_WIDTH; z++) {
			for (int x = 0; x < CHUNK_WIDTH; x++) {
				tmp[y * CHUNK_WIDTH * CHUNK_WIDTH + z * CHUNK_WIDTH + x] = getBlock(x, y, z);
			}
		}
	}
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (!file.good()) {
		std::cerr << "could not open " << path << " file for writing" << std::endl;
		return;
	}
	for (int i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH; i++) {
		file.write((char*)&tmp[i], sizeof(Block));
	}
	file.close();
	if(!file.good()) {
      	std::cerr << "Error occurred at writing time!" << std::endl;
		return;
  	}
	delete[] tmp;
}

void World::updateMeshes(){
	if (chunk.needsMeshUpdate) {
		m_vertices.clear();
		generateMesh();
		chunk.pushData(m_vertices.data(), m_vertices.size());
		chunk.needsMeshUpdate = false;
	}
}

void World::generateMesh(){
	unsigned int cw = CHUNK_WIDTH;

	for(unsigned int y = 0; y < cw; y++){
		for(unsigned int z = 0; z < cw; z++){
			for(unsigned int x = 0; x < cw; x++){
				Block block = getBlock(x, y, z);

				if(block.visible){
					addBlock(x, y, z, block.r, block.g, block.b);
				}
			}
		}
	}
}

bool World::isBlockInLocalWorld(int _x, int _y, int _z){
	if(_x < 0 || _x >= CHUNK_WIDTH || _z < 0 || _z >= CHUNK_WIDTH || _y < 0 || _y >= CHUNK_WIDTH) return false;
	return true;
}

Block World::getBlock(int _x, int _y, int _z){
	if (!isBlockInLocalWorld(_x, _y, _z)) {
		return Block(0, 0, 0, false);
	}
	_x = (_x + blockOffset.x) % CHUNK_WIDTH;
	_y = (_y + blockOffset.y) % CHUNK_WIDTH;
	_z = (_z + blockOffset.z) % CHUNK_WIDTH;

	return m_data[(_y * CHUNK_WIDTH * CHUNK_WIDTH) + (_z * CHUNK_WIDTH) + _x];
}

void World::setBlock(int x, int y, int z, Block block) {
	if(!isBlockInLocalWorld(x, y, z)){
		return;
	}

	x = (x + blockOffset.x) % CHUNK_WIDTH;
	y = (y + blockOffset.y) % CHUNK_WIDTH;
	z = (z + blockOffset.z) % CHUNK_WIDTH;

	m_data[(y * CHUNK_WIDTH * CHUNK_WIDTH) + (z * CHUNK_WIDTH) + x] = block;
	chunk.needsMeshUpdate = true;
}

std::string World::getName() {
	return m_name;
}

void World::addBlock(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	addTopFace(x, y, z, r, g, b);
	addBottomFace(x, y, z, r * 0.90, g * 0.90, b * 0.90);
	addLeftFace(x, y, z, r * 0.94, g * 0.94, b * 0.94);
	addRightFace(x, y, z, r * 0.98, g * 0.98, b * 0.98);
	addFrontFace(x, y, z, r * 0.96, g * 0.96, b * 0.96);
	addBackFace(x, y, z, r * 0.92, g * 0.92, b * 0.92);
}

unsigned int calcAO(bool side1, bool side2, bool corner, bool face){
	if(face) return 2;
	if(side1 && side2){
		return 0;
	}
	return 3 - (side1 + side2 + corner);
}

float map(float ao) {
	return 0.6 + ao * 0.4;
}

void World::addTopFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x, y + 1, z);
	if(adjacentBlock.visible) return;

	float a00 = calcAO(getBlock(x, y + 1, z - 1).visible, getBlock(x - 1, y + 1, z).visible, getBlock(x - 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y + 1, z).visible, getBlock(x, y + 1, z + 1).visible, getBlock(x - 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y + 1, z + 1).visible, getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y + 1, z - 1).visible, getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	a00 = map(a00);
	a01 = map(a01);
	a11 = map(a11);
	a10 = map(a10);

	if(a00 + a11 > a01 + a10) {
		// Generate normal quad
		m_vertices.emplace_back(x, y + 1, z, a00*r, a00*g, a00*b, 0);
		m_vertices.emplace_back(x, y + 1, z + 1, a01*r, a01*g, a01*b, 0);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11*r, a11*g, a11*b, 0);
		m_vertices.emplace_back(x, y + 1, z, a00*r, a00*g, a00*b, 0);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 0);
		m_vertices.emplace_back(x + 1, y + 1, z, a10 * r, a10 * g, a10 * b, 0);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y + 1, z, a10 * r, a10 * g, a10 * b, 0);
		m_vertices.emplace_back(x, y + 1, z, a00 * r, a00 * g, a00 * b, 0);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b, 0);
		m_vertices.emplace_back(x + 1, y + 1, z, a10 * r, a10 * g, a10 * b, 0);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b, 0);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 0);
	}
}

void World::addBottomFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x, y - 1, z);
	if(adjacentBlock.visible) return;

	float a00 = calcAO(getBlock(x, y - 1, z - 1).visible, getBlock(x - 1, y - 1, z).visible, getBlock(x - 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y - 1, z).visible, getBlock(x, y - 1, z + 1).visible, getBlock(x - 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y - 1, z - 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y - 1, z + 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	a00 = map(a00);
	a01 = map(a01);
	a11 = map(a11);
	a10 = map(a10);

	if(a00 + a11 > a01 + a10) {
		// Generate normal quad
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 1);
		m_vertices.emplace_back(x + 1, y, z + 1, a11 * r, a11 * g, a11 * b, 1);
		m_vertices.emplace_back(x, y, z + 1, a01 * r, a01 * g, a01 * b, 1);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 1);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b, 1);
		m_vertices.emplace_back(x + 1, y, z + 1, a11 * r, a11 * g, a11 * b, 1);

	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b, 1);
		m_vertices.emplace_back(x, y, z + 1, a01 * r, a01 * g, a01 * b, 1);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 1);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b, 1);
		m_vertices.emplace_back(x + 1, y, z + 1, a11 * r, a11 * g, a11 * b, 1);
		m_vertices.emplace_back(x, y, z + 1, a01 * r, a01 * g, a01 * b, 1);
	}
}

void World::addRightFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x - 1, y, z);
	if(adjacentBlock.visible) return;

	float a00 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x - 1, y - 1, z).visible, getBlock(x - 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x - 1, y + 1, z).visible, getBlock(x - 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x - 1, y, z + 1).visible, getBlock(x - 1, y - 1, z).visible, getBlock(x - 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x - 1, y + 1, z).visible, getBlock(x - 1, y, z + 1).visible, getBlock(x - 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	a00 = map(a00);
	a01 = map(a01);
	a11 = map(a11);
	a10 = map(a10);

	if(a00 + a11 > a01 + a10) {
		// Generate normal quad
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 2);
		m_vertices.emplace_back(x, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 2);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b, 2);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 2);
		m_vertices.emplace_back(x, y, z + 1, a10 * r, a10 * g, a10 * b, 2);
		m_vertices.emplace_back(x, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 2);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x, y, z + 1, a10 * r, a10 * g, a10 * b, 2);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b, 2);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 2);
		m_vertices.emplace_back(x, y, z + 1, a10 * r, a10 * g, a10 * b, 2);
		m_vertices.emplace_back(x, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 2);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b, 2);
	}
}

void World::addLeftFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x + 1, y, z);
	if(adjacentBlock.visible) return;

	float a00 = calcAO(getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	a00 = map(a00);
	a01 = map(a01);
	a11 = map(a11);
	a10 = map(a10);

	if(a00 + a11 > a01 + a10) {
		// Generate normal quad
		m_vertices.emplace_back(x + 1, y, z, a00 * r, a00 * g, a00 * b, 3);
		m_vertices.emplace_back(x + 1, y + 1, z, a01 * r, a01 * g, a01 * b, 3);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 3);
		m_vertices.emplace_back(x + 1, y, z, a00 * r, a00 * g, a00 * b, 3);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 3);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b, 3);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y + 1, z, a01 * r, a01 * g, a01 * b, 3);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 3);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b, 3);
		m_vertices.emplace_back(x + 1, y + 1, z, a01 * r, a01 * g, a01 * b, 3);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b, 3);
		m_vertices.emplace_back(x + 1, y, z, a00 * r, a00 * g, a00 * b, 3);
	}
}

void World::addFrontFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x, y, z - 1);
	if(adjacentBlock.visible) return;

	float a00 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x, y - 1, z - 1).visible, getBlock(x - 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x, y + 1, z - 1).visible, getBlock(x - 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y - 1, z - 1).visible, getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y + 1, z - 1).visible, getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	a00 = map(a00);
	a01 = map(a01);
	a11 = map(a11);
	a10 = map(a10);

	if(a00 + a11 > a01 + a10) {
		// Generate normal quad
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 4);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b, 4);
		m_vertices.emplace_back(x + 1, y + 1, z, a11 * r, a11 * g, a11 * b, 4);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 4);
		m_vertices.emplace_back(x + 1, y + 1, z, a11 * r, a11 * g, a11 * b, 4);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b, 4);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b, 4);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b, 4);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b, 4);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b, 4);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b, 4);
		m_vertices.emplace_back(x + 1, y + 1, z, a11 * r, a11 * g, a11 * b, 4);
	}
}

void World::addBackFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x, y, z + 1);
	if(adjacentBlock.visible) return;

	float a00 = calcAO(getBlock(x - 1, y, z + 1).visible, getBlock(x, y - 1, z + 1).visible, getBlock(x - 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y, z + 1).visible, getBlock(x, y + 1, z + 1).visible, getBlock(x - 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y - 1, z + 1).visible, getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y + 1, z + 1).visible, getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	a00 = map(a00);
	a01 = map(a01);
	a11 = map(a11);
	a10 = map(a10);

	if(a00 + a11 > a01 + a10) {
		// Generate normal quad
		m_vertices.emplace_back(x, y, z + 1, a00 * r, a00 * g, a00 * b, 5);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 5);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b, 5);
		m_vertices.emplace_back(x, y, z + 1, a00 * r, a00 * g, a00 * b, 5);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b, 5);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 5);
	} else {
		// Generate a flipped quad
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b, 5);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b, 5);
		m_vertices.emplace_back(x, y, z + 1, a00 * r, a00 * g, a00 * b, 5);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b, 5);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b, 5);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b, 5);
	}
}