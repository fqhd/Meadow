#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "PauseMenu.hpp"
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "raytracing/rtweekend.h"
#include "raytracing/camera.h"
#include "raytracing/hittable_list.h"
#include "raytracing/material.h"
#include "raytracing/quad.h"
#include "raytracing/sphere.h"

static bool isUnsignedInteger(const std::string& str) {
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
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
			throw std::runtime_error("Failed to load world, file format invalid");
		}
		if (worldSize <= 0 || worldSize > 32) {
			throw std::runtime_error("Failed to load world, world size out of range");
		}

		data = new Block[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4];

		file.read(reinterpret_cast<char*>(data), CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * worldSize * worldSize * 4 * sizeof(Block));
		if (!file) {
			throw std::runtime_error("Failed to load world, insufficient world data");
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

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    // Cornell box sides
    world.add(make_shared<quad>(point3(555,0,0), vec3(0,0,555), vec3(0,555,0), green));
    world.add(make_shared<quad>(point3(0,0,555), vec3(0,0,-555), vec3(0,555,0), red));

    // Light
    world.add(make_shared<quad>(point3(213,554,227), vec3(130,0,0), vec3(0,0,105), light));

    // Box
    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    // Glass Sphere
    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(190,90,190), 90, glass));

    // Light Sources
    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(
        make_shared<quad>(point3(343,554,332), vec3(-130,0,0), vec3(0,0,-105), empty_material));
    lights.add(make_shared<sphere>(point3(190, 90, 190), 90, empty_material));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 1;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 90;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0;


	game.destroy();
	Window::close();

    cam.render(world, lights);
	
	return 0;
}