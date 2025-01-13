#include "Player.hpp"
#include "Utils.hpp"
#include <iostream>
#include <algorithm>

const unsigned int PRECISION = 50;
const unsigned int REACH_DISTANCE = 5;

void Player::init(Camera* camera, World* world) {
	m_camera = camera;
	m_world = world;
	float worldWidthInBlocks = world->getWorldSize() * CHUNK_WIDTH;
	position = glm::vec3(worldWidthInBlocks / 2.0f, 0.0f, worldWidthInBlocks / 2.0f);
}

void Player::update(float deltaTime) {
	movement(deltaTime);
	placeAndBreakBlocks();
	hotbar.update();
}

void Player::placeAndBreakBlocks() {
	getVisibleBlocks();

	if (!visibleBlocks.lookingAtBlock) return;

	if (InputManager::isKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		breakBlock();
	}
	else if (InputManager::isKeyPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		placeBlock();
	}

	getVisibleBlocks();
}


void Player::movement(float deltaTime) {
	if (sprinting) {
		speed = 12.0f;
	}
	else {
		speed = 4.0f;
	}
	glm::vec3 camForward = m_camera->getForward();
	glm::vec3 forward = glm::normalize(glm::vec3(camForward.x, 0.0f, camForward.z));
	glm::vec3 side = glm::normalize(glm::cross(camForward, glm::vec3(0.0f, 1.0f, 0.0f)));

	if (InputManager::isKeyDown(GLFW_KEY_W)) {
		position += forward * speed * deltaTime;
	}
	if (InputManager::isKeyDown(GLFW_KEY_S)) {
		position -= forward * speed * deltaTime;
	}
	if (InputManager::isKeyDown(GLFW_KEY_A)) {
		position -= side * speed * deltaTime;
	}
	if (InputManager::isKeyDown(GLFW_KEY_D)) {
		position += side * speed * deltaTime;
	}
	if (InputManager::isKeyDown(GLFW_KEY_SPACE)) {
		position.y += speed * deltaTime;
	}
	if (InputManager::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		position.y -= speed * deltaTime;
	}
}

glm::vec3 Player::getEyePos() const {
	return glm::vec3(position.x + 0.5f, position.y + 1.5f, position.z + 0.5f);
}

glm::ivec3 vecToBlock(const glm::vec3& vec) {
	return glm::ivec3(glm::floor(vec.x), glm::floor(vec.y), glm::floor(vec.z));
}

void Player::getVisibleBlocks() {
	glm::ivec3 pos = vecToBlock(m_camera->getPosition());
	visibleBlocks.lookingAtBlock = false;

	glm::vec3 rayPosition = m_camera->getPosition();
	for (unsigned int i = 0; i < PRECISION; i++) {
		rayPosition += m_camera->getForward() * (REACH_DISTANCE / (float)PRECISION);

		visibleBlocks.breakableBlock = vecToBlock(rayPosition);
		Block b = m_world->getBlock(visibleBlocks.breakableBlock.x, visibleBlocks.breakableBlock.y, visibleBlocks.breakableBlock.z);

		if (b.visible) {
			visibleBlocks.lookingAtBlock = true;
			rayPosition -= m_camera->getForward() * (REACH_DISTANCE / (float)PRECISION);
			visibleBlocks.placeableBlock = vecToBlock(rayPosition);
			break;
		}
	}
}

void Player::placeBlock() {
	ColorRGBA8 color = hotbar.colors[hotbar.selectorIndex];
	Block b(color.r, color.g, color.b, true);
	m_world->setBlock(visibleBlocks.placeableBlock.x, visibleBlocks.placeableBlock.y, visibleBlocks.placeableBlock.z, b);
}

void Player::breakBlock() {
	glm::ivec3 vb = visibleBlocks.breakableBlock;

	m_world->setBlock(vb.x, vb.y, vb.z, Block(0, 0, 0, false));
}
