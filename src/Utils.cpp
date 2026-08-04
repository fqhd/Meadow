#include "Utils.hpp"
#include <cmath>

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


void Utils::freeBuffer(uint8_t* buffer) {
	free(buffer);
}

glm::vec4 Utils::toScreenCoords(glm::vec4 rect, int w, int h) {
	return glm::vec4((rect.x / 1920.0f) * w, (rect.y / 1080.0f) * h, (rect.z / 1920.f) * w, (rect.w / 1080.0f) * h);
}

std::string Utils::generateRandomString(int length) {
    std::string s = "";
    std::vector<char> possibleChars = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
    };
    for (int i = 0; i < length; i++) {
        int random_index = rand() % possibleChars.size();
        s += possibleChars[random_index];
    }
    return s;
}

void Utils::writeStringToFile(const std::string& text, const std::string& filename) {
    if (filename.size() > 32) {
        std::cerr << "Error saving render state, filename too big to store..." << std::endl;
        return;
    }
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    file << text;

    if (!file.good()) {
        throw std::runtime_error("Failed to write to file: " + filename);
    }
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
