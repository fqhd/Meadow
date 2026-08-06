#pragma once
#include <glm/glm.hpp>

struct CameraData {
    glm::vec3 position;
    glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 w;
	glm::vec3 u;
	glm::vec3 v;
	float lensRadius;
};

struct SceneData {
    int width;
    int height;
    int worldSize;
    uint frameSeed;
	CameraData camera;
};
