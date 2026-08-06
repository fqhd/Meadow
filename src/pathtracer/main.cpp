#include "Raytracer.h"
#include "Benchmarker.h"
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Must specify a render file" << std::endl;
        return -1;
    }
    std::string arg1 = argv[1];
    std::cout << "Processing " << arg1 << std::endl;

    glm::vec3 camPos;
    glm::vec3 camDir;
    int worldSize;
    std::string worldEnc;

    std::ifstream is(arg1);
    std::string token;
    while (is >> token) {
        if (token == "CameraPosition:") {
            is >> token;
            camPos.x = std::stof(token);
            is >> token;
            camPos.y = std::stof(token);
            is >> token;
            camPos.z = std::stof(token);
        } else if (token == "CameraDirection:") {
            is >> token;
            camDir.x = std::stof(token);
            is >> token;
            camDir.y = std::stof(token);
            is >> token;
            camDir.z = std::stof(token);
        } else if (token == "WorldSize:") {
            is >> token;
            worldSize = std::stoi(token);
        } else if (token == "WorldData:") {
            is >> worldEnc;
        }
    }

	Raytracer raytracer(
		1920, 1080, camPos, camPos + camDir, 50.0f, 1.5f, 10.0f, worldSize, worldEnc
	);

	std::cout << "Started Timer" << std::endl;
	Benchmarker::Start("Draw");
    raytracer.Draw(100);
	Benchmarker::End("Draw");
	arg1.erase(arg1.size() - 3);
	arg1 += "png";
	raytracer.Canvas.Save(arg1);

	return 0;
}
