#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "PauseMenu.hpp"
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "raytracing/rtweekend.h"
#include "raytracing/bvh.h"
#include "raytracing/camera.h"
#include "raytracing/constant_medium.h"
#include "raytracing/hittable.h"
#include "raytracing/hittable_list.h"
#include "raytracing/material.h"
#include "raytracing/quad.h"
#include "raytracing/sphere.h"
#include "raytracing/texture.h"

World* world_ptr;
hittable_list* rtworld_ptr;

static bool isUnsignedInteger(const std::string& str) {
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}


void addTopFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = world_ptr->getBlock(x, y + 1, z);
	if(adjacentBlock.visible) return;

	auto material = make_shared<lambertian>(color(r/255.0, g/255.0, b/255.0));
	auto face = make_shared<quad>(point3(x, y+1, z), vec3(0, 0, 1), vec3(1, 0, 0), material);
	rtworld_ptr->add(face);
}

void addBottomFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = world_ptr->getBlock(x, y - 1, z);
	if(adjacentBlock.visible) return;

	auto material = make_shared<lambertian>(color(r/255.0, g/255.0, b/255.0));
	auto face = make_shared<quad>(point3(x, y, z), vec3(0, 0, 1), vec3(1, 0, 0), material);
	rtworld_ptr->add(face);
}

void addRightFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = world_ptr->getBlock(x - 1, y, z);
	if(adjacentBlock.visible) return;

	auto material = make_shared<lambertian>(color(r/255.0, g/255.0, b/255.0));
	auto face = make_shared<quad>(point3(x, y, z), vec3(0, 0, 1), vec3(0, 1, 0), material);
	rtworld_ptr->add(face);
}

void addLeftFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = world_ptr->getBlock(x + 1, y, z);
	if(adjacentBlock.visible) return;

	auto material = make_shared<lambertian>(color(r/255.0, g/255.0, b/255.0));
	auto face = make_shared<quad>(point3(x+1, y, z), vec3(0, 0, 1), vec3(0, 1, 0), material);
	rtworld_ptr->add(face);
}

void addFrontFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = world_ptr->getBlock(x, y, z - 1);
	if(adjacentBlock.visible) return;

	auto material = make_shared<lambertian>(color(r/255.0, g/255.0, b/255.0));
	auto face = make_shared<quad>(point3(x, y, z), vec3(1, 0, 0), vec3(0, 1, 0), material);
	rtworld_ptr->add(face);
}

void addBackFace(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	Block adjacentBlock = world_ptr->getBlock(x, y, z + 1);
	if(adjacentBlock.visible) return;

	auto material = make_shared<lambertian>(color(r/255.0, g/255.0, b/255.0));
	auto face = make_shared<quad>(point3(x, y, z+1), vec3(1, 0, 0), vec3(0, 1, 0), material);
	rtworld_ptr->add(face);
}

void addBlock(float x, float y, float z, GLubyte r, GLubyte g, GLubyte b){
	addTopFace(x, y, z, r, g, b);
	addBottomFace(x, y, z, r, g, b);
	addLeftFace(x, y, z, r, g, b);
	addRightFace(x, y, z, r, g, b);
	addFrontFace(x, y, z, r, g, b);
	addBackFace(x, y, z, r, g, b);
}

static void generateMesh(int chunkX, int chunkY, int chunkZ){
	unsigned int cw = CHUNK_WIDTH;

	for(unsigned int y = 0; y < cw; y++){
		for(unsigned int z = 0; z < cw; z++){
			for(unsigned int x = 0; x < cw; x++){
				int globalX = x + chunkX * CHUNK_WIDTH;
				int globalY = y + chunkY * CHUNK_WIDTH;
				int globalZ = z + chunkZ * CHUNK_WIDTH;

				Block block = world_ptr->getBlock(globalX, globalY, globalZ);

				if(block.visible){
					addBlock(globalX, globalY, globalZ, block.r, block.g, block.b);
				}
			}
		}
	}
}

int main() {

	Block* data = nullptr;
	unsigned int worldSize;

	// Try to open the world save file, if it exists, load the world from it, if not, prompt the user for world size and create a new one
	std::ifstream file("world.dat", std::ios::in | std::ios::binary);
	if (!file.good()) {
		std::cout << "Could not find world save file, creating a new one..." << std::endl;
		std::cout << "Enter the world size(1-32): ";
		std::string userInput;
		std::getline(std::cin, userInput);
		if (!isUnsignedInteger(userInput)) {
			throw std::runtime_error("The world size must be an unsigned integer(a positive whole number)");
		}
		worldSize = std::stoi(userInput);
		if (worldSize > 32 || worldSize <= 0) {
			throw std::runtime_error("Invalid range. The world size must be between 1 and 32");
		}

		data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4];
		memset(data, 0, sizeof(Block) * CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4);
		for (int i = 0; i < CHUNK_WIDTH * worldSize; i++) {
			for (int j = 0; j < CHUNK_WIDTH * worldSize; j++) {
				data[i * CHUNK_WIDTH * worldSize + j] = Block(255, 255, 255, true);
			}
		}
	}
	else {
		file.read(reinterpret_cast<char*>(&worldSize), sizeof(worldSize));
		if (!file) {
			throw std::runtime_error("Failed to load file format invalid");
		}
		if (worldSize <= 0 || worldSize > 32) {
			throw std::runtime_error("Failed to load world size out of range");
		}

		data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4];

		file.read(reinterpret_cast<char*>(data), CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4 * sizeof(Block));
		if (!file) {
			throw std::runtime_error("Failed to load insufficient world data");
		}

		file.close();
	}

	Window::create();
	InputManager::init(Window::getWindowPtr());
	GUIRenderer::init(WINDOW_WIDTH, WINDOW_HEIGHT);

	Game game;
	game.init(data, worldSize);
	PauseMenu pausemenu;
	pausemenu.init(&game.world);
	GameState gamestate = GameState::Pause;
	bool shouldRender = false;

	double before = glfwGetTime();
	while (gamestate != GameState::Exit) {
		Window::clear();
		double nowTime = glfwGetTime();
		double dt = nowTime - before;
		before = nowTime;
		if (!InputManager::processInput()) gamestate = GameState::Exit;

		if (gamestate == GameState::Game) {
			game.update(dt, gamestate);
			game.render();
		}
		else if (gamestate == GameState::Pause) {
			pausemenu.update(dt, gamestate, shouldRender);
			game.render();
			pausemenu.render(&game.player.hotbar);
		}

		Window::update();
	}

	// If we shouldn't render, uninit and return
	if(!shouldRender) {
		game.destroy();
		Window::close();
		return 0;
	}

	// If we should render, init the world and begin rendering
	hittable_list world;
	world_ptr = &game.world;
	rtworld_ptr = &world;

	for(int y = 0; y < 4; y++) {
		for(int z = 0; z < worldSize; z++) {
			for(int x = 0; x < worldSize; x++) {
				int chunkIndex = (y * worldSize * worldSize) + (z * worldSize) + x;
				generateMesh(x, y, z);
			}
		}
	}

    world = hittable_list(make_shared<bvh_node>(world));

	game.destroy();
	Window::close();

    camera cam;

	int renderWidth;
	int renderHeight;
	int samplesPerPixel;

	std::string userInput;
	std::cout << "Render width: ";
	std::cin >> userInput;
	renderWidth = std::stoi(userInput);
	
	std::cout << "Render height: ";
	std::cin >> userInput;
	renderHeight = std::stoi(userInput);
	
	std::cout << "Samples per pixel: ";
	std::cin >> userInput;
	samplesPerPixel = std::stoi(userInput);
	

	cam.image_height = renderHeight;
    cam.image_width       = renderWidth;
    cam.samples_per_pixel = samplesPerPixel;
    cam.max_depth         = 50;

    cam.vfov     = 70.0;
    cam.lookfrom = vec3(game.camera.getPosition().x, game.camera.getPosition().y, game.camera.getPosition().z);
	glm::vec3 camCenter = game.camera.getPosition() + game.camera.getForward();
    cam.lookat   = point3(camCenter.x, camCenter.y, camCenter.z);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;



    cam.render(world);

	
	return 0;
}