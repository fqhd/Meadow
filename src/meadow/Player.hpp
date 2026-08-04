#pragma once

#include "Camera.hpp"
#include "World.hpp"
#include "Hotbar.hpp"
#include "ColorRGBA8.hpp"

struct VisibleBlocks {
	glm::ivec3 breakableBlock;
	glm::ivec3 placeableBlock;
	bool lookingAtBlock = false;
};

class Player {
public:

	void init(Camera* camera, World* world);
	void update(float deltaTime);
	glm::vec3 getEyePos() const;

	Hotbar hotbar;
	VisibleBlocks visibleBlocks;
	glm::vec3 position;
	bool sprinting = false;
	float speed = 4.0f;

private:

	void movement(float deltaTime);
	void getVisibleBlocks();
	void placeAndBreakBlocks();
	void placeBlock();
	void breakBlock();

	World* m_world = nullptr;
	Camera* m_camera = nullptr;

};