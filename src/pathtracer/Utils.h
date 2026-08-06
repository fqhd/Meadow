#pragma once
#include "glm/glm.hpp"

float lengthSquared(const glm::vec3& vec);
float randomFloatU();
float randomFloatV();
bool nearZero(const glm::vec3& vec);
glm::vec3 randomInUnitSphere();
glm::vec3 randomUnitVector();
glm::vec3 randomInHemisphere(const glm::vec3& normal);
glm::vec3 randomInUnitDisk();
glm::vec3 randomColor();