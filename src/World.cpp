#include "World.hpp"
#include <fstream>

bool isUnsignedInteger(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

void World::init(){
	// Try to open the world save file, if it exists, load the world from it, if not, prompt the user for world size and create a new one
	std::ifstream file("world.dat", std::ios::in | std::ios::binary);
	if (!file.good()) {
		std::cout << "Could not find world save file, creating a new one..." << std::endl;
		std::cout << "Enter the world size(1-32): ";
		std::string userInput;
		std::getline(std::cin, userInput);
		if(!isUnsignedInteger(userInput)){
			throw std::runtime_error("The world size must be an unsigned integer(a positive whole number)");
		}
		int worldSize = std::stoi(userInput);
		if(worldSize > 32 || worldSize <= 0) {
			throw std::runtime_error("Invalid range. The world size must be between 1 and 32");
		}
		m_worldSize = worldSize;

		m_data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize * 4];
		memset(m_data, 0, sizeof(Block) * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize * 4);
		for (int i = 0; i < CHUNK_WIDTH * m_worldSize; i++) {
			for (int j = 0; j < CHUNK_WIDTH * m_worldSize; j++) {
				m_data[j * CHUNK_WIDTH * m_worldSize + i] = Block(255, 255, 255, true);
			}
		}
	}else{
		file.read(reinterpret_cast<char*>(&m_worldSize), sizeof(m_worldSize));
		if (!file) {
			throw std::runtime_error("Failed to load world, file format invalid");
		}
		if(m_worldSize <= 0 || m_worldSize > 32) {
			throw std::runtime_error("Failed to load world, world size out of range");
		}

		m_data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize * 4];

		file.read(reinterpret_cast<char*>(m_data), CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize * 4);
		if (!file) {
			throw std::runtime_error("Failed to load world, insufficient world data");
		}

		file.close();
	}

	m_chunks = new Chunk[m_worldSize * m_worldSize * 4];
	for(int i = 0; i < m_worldSize * m_worldSize * 4; i++) {
		m_chunks[i].init();
	}

	m_shader.load("chunk");
	updateMeshes();
}

void World::destroy(){
	for(int i = 0; i < m_worldSize * m_worldSize * 4; i++) {
		m_chunks[i].destroy();
	}
	delete[] m_chunks;
	delete[] m_data;
}

Chunk* World::getChunk(int x, int y, int z) {
	int chunkIndex = (y * m_worldSize * m_worldSize) + (z * m_worldSize) + x;
	return &m_chunks[chunkIndex];
}

void World::render(Camera& camera) {
	m_shader.bind();

	m_shader.loadUniform("projection", camera.getProjectionMatrix());
	m_shader.loadUniform("view", camera.getViewMatrix());

	for(int i = 0; i < m_worldSize * m_worldSize * 4; i++) {
		m_chunks[i].render();
	}

	m_shader.unbind();
}

void World::save() {
    // Open file in binary mode for output
    std::ofstream file("world.dat", std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for saving." << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&m_worldSize), sizeof(m_worldSize));
    if (!file) {
        std::cerr << "Error writing world size." << std::endl;
        return;
    }

    // Step 2: Write the world data
    file.write(reinterpret_cast<const char*>(m_data), CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize * 4 * sizeof(Block));
    if (!file) {
        std::cerr << "Error writing world data." << std::endl;
        return;
    }

    std::cout << "World saved successfully." << std::endl;
}

unsigned int World::getWorldSize() {
	return m_worldSize;
}

void World::updateMeshes(){
	for(int y = 0; y < 4; y++) {
		for(int z = 0; z < m_worldSize; z++) {
			for(int x = 0; x < m_worldSize; x++) {
				int chunkIndex = (y * m_worldSize * m_worldSize) + (z * m_worldSize) + x;
				if(m_chunks[chunkIndex].needsMeshUpdate) {
					generateMesh(x, y, z);
					m_chunks[chunkIndex].pushData(m_vertices.data(), m_vertices.size());
					m_chunks[chunkIndex].needsMeshUpdate = false;
				}
			}
		}
	}
}

void World::generateMesh(int chunkX, int chunkY, int chunkZ){
	m_vertices.clear();
	unsigned int cw = CHUNK_WIDTH;

	for(unsigned int y = 0; y < cw; y++){
		for(unsigned int z = 0; z < cw; z++){
			for(unsigned int x = 0; x < cw; x++){
				int globalX = x + chunkX * CHUNK_WIDTH;
				int globalY = y + chunkY * CHUNK_WIDTH;
				int globalZ = z + chunkZ * CHUNK_WIDTH;

				Block block = getBlock(globalX, globalY, globalZ);

				if(block.visible){
					addBlock(globalX, globalY, globalZ, block.r, block.g, block.b);
				}
			}
		}
	}
}

bool World::isBlockInLocalWorld(int _x, int _y, int _z){
	if(_x < 0 || _x >= CHUNK_WIDTH * m_worldSize || _z < 0 || _z >= CHUNK_WIDTH * m_worldSize || _y < 0 || _y >= CHUNK_WIDTH * 4) return false;
	return true;
}

Block World::getBlock(int _x, int _y, int _z){
	if (!isBlockInLocalWorld(_x, _y, _z)) {
		return Block(0, 0, 0, false);
	}
	
	return m_data[(_y * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize) + (_z * CHUNK_WIDTH * m_worldSize) + _x];
}

void World::setBlock(int x, int y, int z, Block block) {
	if(!isBlockInLocalWorld(x, y, z)){
		return;
	}

	m_data[(y * CHUNK_WIDTH * CHUNK_WIDTH * m_worldSize * m_worldSize) + (z * CHUNK_WIDTH * m_worldSize) + x] = block;

	int chunkX = x / CHUNK_WIDTH;
	int chunkZ = z / CHUNK_WIDTH;
	int chunkY = y / CHUNK_WIDTH;
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			for(int k = -1; k <= 1; k++) {
				int cPosX = chunkX + i;
				int cPosY = chunkY + j;
				int cPosZ = chunkZ + k;
				if(!(cPosX < 0 || cPosX >= m_worldSize || cPosZ < 0 || cPosZ >= m_worldSize || cPosY < 0 || cPosY >= 4)) {
					getChunk(cPosX, cPosY, cPosZ)->needsMeshUpdate = true;
				}
			}
		}
	}
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
		m_vertices.emplace_back(x, y + 1, z, a00*r, a00*g, a00*b);
		m_vertices.emplace_back(x, y + 1, z + 1, a01*r, a01*g, a01*b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11*r, a11*g, a11*b);
		m_vertices.emplace_back(x, y + 1, z, a00*r, a00*g, a00*b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a10 * r, a10 * g, a10 * b);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y + 1, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
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
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y, z + 1, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a11 * r, a11 * g, a11 * b);

	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y, z + 1, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y, z + 1, a01 * r, a01 * g, a01 * b);
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
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b);
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
		m_vertices.emplace_back(x + 1, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x + 1, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x + 1, y, z, a00 * r, a00 * g, a00 * b);
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
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b);
	} else {
		// Generate flipped quad
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y, z, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y, z, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x + 1, y + 1, z, a11 * r, a11 * g, a11 * b);
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
		m_vertices.emplace_back(x, y, z + 1, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x, y, z + 1, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
	} else {
		// Generate a flipped quad
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b);
		m_vertices.emplace_back(x, y, z + 1, a00 * r, a00 * g, a00 * b);
		m_vertices.emplace_back(x + 1, y, z + 1, a10 * r, a10 * g, a10 * b);
		m_vertices.emplace_back(x + 1, y + 1, z + 1, a11 * r, a11 * g, a11 * b);
		m_vertices.emplace_back(x, y + 1, z + 1, a01 * r, a01 * g, a01 * b);
	}
}