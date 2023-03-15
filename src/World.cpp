#include "World.hpp"
#include <iostream>

void World::init(){
	m_data_length = WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH;
	m_data = new Block[m_data_length];
	memset(m_data, 0, sizeof(Block) * m_data_length);

	m_chunks = new Chunk[WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT];
	for(unsigned int y = 0; y < WORLD_HEIGHT; y++){
		for(unsigned int z = 0; z < WORLD_WIDTH; z++){
			for(unsigned int x = 0; x < WORLD_WIDTH; x++){
				getChunk(x, y, z)->init(x * CHUNK_WIDTH, y * CHUNK_WIDTH, z * CHUNK_WIDTH);
			}
		}
	}

	m_shader.load("chunk");
}

void World::createNew(const std::string& name) {
	m_name = name;
	memset(m_data, 0, sizeof(Block) * m_data_length);
	for (int i = 0; i < WORLD_WIDTH * CHUNK_WIDTH; i++) {
		for (int j = 0; j < WORLD_WIDTH * CHUNK_WIDTH; j++) {
			setBlock(i, 0, j, Block(255, 255, 255, true));
		}
	}
}

void World::render(Camera& camera){
	updateMeshes();
	m_shader.bind();

	m_shader.loadUniform("projection", camera.getProjectionMatrix());
	m_shader.loadUniform("view", camera.getViewMatrix());

	unsigned int ww = WORLD_WIDTH;
	unsigned int wl = WORLD_WIDTH;
	unsigned int wh = WORLD_HEIGHT;

	for (int i = 0; i < WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT; i++) {
		if (m_chunks[i].getNumVertices()) {
			m_chunks[i].render();
		}
	}

	m_shader.unbind();
}

void World::destroy(){
	for (int i = 0; i < WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT; i++) {
		m_chunks[i].destroy();
	}
	m_shader.destroy();
	delete[] m_data;
	delete[] m_chunks;
}

int World::load(const std::string& name) {
	m_name = name;
	std::string path = "worlds/" + m_name;
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.good()) {
		std::cout << "World: Could not open file: " << path << std::endl;
		return -1;
	}
	for (int i = 0; i < m_data_length; i++) {
		file.read((char*)&m_data[i], sizeof(Block));
	}
	file.close();

	for (int i = 0; i < WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT; i++) {
		m_chunks[i].needsMeshUpdate = true;
	}

	return 0;
}

void World::save() {
	std::string path = "worlds/" + m_name;
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (!file.good()) {
		std::cerr << "could not open " << path << " file for writing" << std::endl;
		return;
	}
	for (int i = 0; i < m_data_length; i++) {
		file.write((char*)&m_data[i], sizeof(Block));
	}
	file.close();
	if(!file.good()) {
      	std::cerr << "Error occurred at writing time!" << std::endl;
		return;
  	}
}

void World::updateMeshes(){
	for (int i = 0; i < WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT; i++) {
		if (m_chunks[i].needsMeshUpdate) {
			generateMesh(&m_chunks[i]);
			m_chunks[i].pushData(m_vertices.data(), m_vertices.size());
			m_chunks[i].needsMeshUpdate = false;
		}
	}
}

void World::generateMesh(Chunk* chunk){
	m_vertices.clear();
	unsigned int cw = CHUNK_WIDTH;

	for(unsigned int y = 0; y < cw; y++){
		for(unsigned int z = 0; z < cw; z++){
			for(unsigned int x = 0; x < cw; x++){
				Block block = getBlock(chunk->x + x, chunk->y + y, chunk->z + z);

				if(block.visible){
					addBlock(chunk->x + x, chunk->y + y, chunk->z + z, block.r, block.g, block.b);
				}
			}
		}
	}
}

bool World::isBlockInLocalWorld(int _x, int _y, int _z){
	unsigned int maxW = WORLD_WIDTH * CHUNK_WIDTH;
	unsigned int maxL = WORLD_WIDTH * CHUNK_WIDTH;
	unsigned int maxH = WORLD_HEIGHT * CHUNK_WIDTH;

	if(_x < 0 || _x >= maxW || _z < 0 || _z >= maxL || _y < 0 || _y >= maxH) return false;
	return true;
}

Block World::getBlock(int _x, int _y, int _z){
	if(!isBlockInLocalWorld(_x, _y, _z)){
		return Block(0, 0, 0, false);
	}

	unsigned int maxW = WORLD_WIDTH * CHUNK_WIDTH;
	unsigned int maxL = WORLD_WIDTH * CHUNK_WIDTH;

	return m_data[(_y * maxW * maxL) + (_z * maxW) + _x];
}

void World::setBlock(int x, int y, int z, Block block) {
	if(!isBlockInLocalWorld(x, y, z)){
		return;
	}

	unsigned int ww = WORLD_WIDTH;
	unsigned int wl = WORLD_WIDTH;
	unsigned int wh = WORLD_HEIGHT;
	unsigned int cw = CHUNK_WIDTH;

	unsigned int maxW = ww * cw;
	unsigned int maxL = wl * cw;

	// Getting the chunk the block is in
	unsigned int posX = x / cw;
	unsigned int posY = y / cw;
	unsigned int posZ = z / cw;

	// Right now, we have the position of the chunk that the block has been placed in stored in posX, posY, and posZ
	// So we first of all, queue this chunk up for a mesh update
	getChunk(posX, posY, posZ)->needsMeshUpdate = true;

	// Setting the block based on chunk space coords
	m_data[(y * maxW * maxL) + (z * maxW) + x] = block;

	// Next, we check if the placed block has been placed on any edge
	// Update neighboring m_chunks if block is on the edge of the current chunk
	if(x % cw == 0){
		Chunk* chunk = getChunk((posX - 1) % ww, posY, posZ);
		if(chunk) chunk->needsMeshUpdate = true;
	}
	if((x + 1) % cw == 0){
		Chunk* chunk = getChunk((posX + 1) % ww, posY, posZ);
		if(chunk) chunk->needsMeshUpdate = true;
	}
	if(z % cw == 0){
		Chunk* chunk = getChunk(posX, posY, (posZ - 1) % wl);
		if(chunk) chunk->needsMeshUpdate = true;
	}
	if((z + 1) % cw == 0){
		Chunk* chunk = getChunk(posX, posY, (posZ + 1) % wl);
		if(chunk) chunk->needsMeshUpdate = true;
	}
	if(y % cw == 0){
		Chunk* chunk = getChunk(posX, (posY - 1) % wh, posZ);
		if(chunk) chunk->needsMeshUpdate = true;
	}
	if((y + 1) % cw == 0){
		Chunk* chunk = getChunk(posX, (posY + 1) % wh, posZ);
		if(chunk) chunk->needsMeshUpdate = true;
	}
}

void World::addBlock(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	addTopFace(x, y, z, r, g, b);
	addBottomFace(x, y, z, r, g, b);
	addLeftFace(x, y, z, r, g, b);
	addRightFace(x, y, z, r, g, b);
	addFrontFace(x, y, z, r, g, b);
	addBackFace(x, y, z, r, g, b);
}

Chunk* World::getChunk(int _x, int _y, int _z) {
	unsigned int ww = WORLD_WIDTH;
	unsigned int wl = WORLD_WIDTH;
	unsigned int wh = WORLD_HEIGHT;

	if(_y < 0 || _y >= wh || _x < 0 || _x >= ww || _z < 0 || _z >= wl){
		return nullptr;
	}
	return &m_chunks[(_y * ww * wl) + (_z * ww) + _x];
}

unsigned int calcAO(bool side1, bool side2, bool corner, bool face){
	if(face) return 2;
	if(side1 && side2){
		return 0;
	}
	return 3 - (side1 + side2 + corner);
}

bool isBlockTransparent(uint8_t _blockID){
	return _blockID == 7 || !_blockID;
}

void World::addTopFace(GLubyte x, GLubyte y, GLubyte z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = getBlock(x, y + 1, z);
	if(!isBlockTransparent(adjacentBlock.visible)) return;

	float a00 = calcAO(getBlock(x, y + 1, z - 1).visible, getBlock(x - 1, y + 1, z).visible, getBlock(x - 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y + 1, z).visible, getBlock(x, y + 1, z + 1).visible, getBlock(x - 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y + 1, z + 1).visible, getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y + 1, z - 1).visible, getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;

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
	if(!isBlockTransparent(adjacentBlock.visible)) return;

	float a00 = calcAO(getBlock(x, y - 1, z - 1).visible, getBlock(x - 1, y - 1, z).visible, getBlock(x - 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y - 1, z).visible, getBlock(x, y - 1, z + 1).visible, getBlock(x - 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y - 1, z - 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y - 1, z + 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;

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
	if(!isBlockTransparent(adjacentBlock.visible)) return;

	float a00 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x - 1, y - 1, z).visible, getBlock(x - 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x - 1, y + 1, z).visible, getBlock(x - 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x - 1, y, z + 1).visible, getBlock(x - 1, y - 1, z).visible, getBlock(x - 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x - 1, y + 1, z).visible, getBlock(x - 1, y, z + 1).visible, getBlock(x - 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;


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
	if(!isBlockTransparent(adjacentBlock.visible)) return;

	float a00 = calcAO(getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y - 1, z).visible, getBlock(x + 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x + 1, y + 1, z).visible, getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;

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
	if(!isBlockTransparent(adjacentBlock.visible)) return;

	float a00 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x, y - 1, z - 1).visible, getBlock(x - 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y, z - 1).visible, getBlock(x, y + 1, z - 1).visible, getBlock(x - 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y - 1, z - 1).visible, getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y - 1, z - 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y + 1, z - 1).visible, getBlock(x + 1, y, z - 1).visible, getBlock(x + 1, y + 1, z - 1).visible, adjacentBlock.visible) / 3.0f;

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
	if(!isBlockTransparent(adjacentBlock.visible)) return;

	float a00 = calcAO(getBlock(x - 1, y, z + 1).visible, getBlock(x, y - 1, z + 1).visible, getBlock(x - 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a01 = calcAO(getBlock(x - 1, y, z + 1).visible, getBlock(x, y + 1, z + 1).visible, getBlock(x - 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a10 = calcAO(getBlock(x, y - 1, z + 1).visible, getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y - 1, z + 1).visible, adjacentBlock.visible) / 3.0f;
	float a11 = calcAO(getBlock(x, y + 1, z + 1).visible, getBlock(x + 1, y, z + 1).visible, getBlock(x + 1, y + 1, z + 1).visible, adjacentBlock.visible) / 3.0f;

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