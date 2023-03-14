#include "Utils.hpp"

uint8_t* Utils::readFileToBuffer(const std::string& filePath) {

	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return nullptr;
	}

	file.seekg(0, std::ios::end);

	unsigned int fileSize = (unsigned int)file.tellg();
	file.seekg(0, std::ios::beg);

	fileSize -= (unsigned int)file.tellg();

	//Creating new buffer
	uint8_t* buffer = (uint8_t*)malloc(fileSize);

	//Reading data into buffer
	file.read((char*)buffer, fileSize);
	file.close();

	return buffer;
}

/*
	TODO: this tokenize function doesn't work properly it must be fixed.
	The goal of this function would be to take a string filled with both
	letters and spaces and return an array of strings corresponding to the
	letters/words separated by said spaces e.g:

	Input: "Hello, this is an example string"
	Output: "Hello", "this", "is", "an", "example", "string"

	The function shoudl also account for multiple spaces as exceptions. For example:

	Input: "   Hey, this     i    s me"
	Output: "Hey", "this", "i", "s", "me"
*/
std::vector<std::string> Utils::tokenizeString(const std::string& _str) {
	std::vector<std::string> tokens;
	tokens.push_back(std::string());
	for (unsigned int i = 0; i < _str.size(); i++) {
		if (_str[i] == ' ') {
			tokens.push_back(std::string());
		}
		else {
			tokens.back().push_back(_str[i]);
		}
	}
	return tokens;
}

void Utils::freeBuffer(uint8_t* buffer) {
	free(buffer);
}

std::string Utils::readFileToString(const std::string& shaderName) {
	std::string shaderCode = "";
	std::string line;
	std::ifstream is;
	is.open(shaderName);
	if (is.fail()) {
		std::cout << "Utils: Failed to find file: " << shaderName << std::endl;
		is.close();
		return shaderCode;
	}

	while (std::getline(is, line)) {
		shaderCode += line + "\n";
	}

	is.close();
	return shaderCode;
}

bool Utils::isInside(const glm::ivec2& pos, const glm::vec4& destRect) {
	return (pos.x >= destRect.x && pos.x <= destRect.x + destRect.z && pos.y >= destRect.y && pos.y <= destRect.y + destRect.w);
}

bool Utils::isInRange(const glm::vec3& a, const glm::vec3& b, float range) {
	return glm::abs(glm::length(b - a)) < range;
}
