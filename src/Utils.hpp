#pragma once

#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <cstdarg>
#include <chrono>
#include "glad.h"
#include <glm/glm.hpp>

namespace Utils {

	uint8_t* readFileToBuffer(const std::string& filePath);
	void freeBuffer(uint8_t* buffer);
	std::string readFileToString(const std::string& shaderName);
	bool isInside(const glm::ivec2& mousePos, const glm::vec4& destRect);
	bool isInRange(const glm::vec3& a, const glm::vec3& b, float range);
	glm::vec4 toScreenCoords(glm::vec4 rect, int w, int th);


}

