#include "Raytracer.h"
#include "Benchmarker.h"
#include <fstream>

WorldData loadWorldData(const std::string& path) {
    WorldData data;

    std::ifstream is(path);
    std::string token;

    while (is >> token) {
        if (token == "CameraPosition:") {
            is >> token;
            data.camPos.x = std::stof(token);
            is >> token;
            data.camPos.y = std::stof(token);
            is >> token;
            data.camPos.z = std::stof(token);
        } else if (token == "CameraDirection:") {
            is >> token;
            data.camDir.x = std::stof(token);
            is >> token;
            data.camDir.y = std::stof(token);
            is >> token;
            data.camDir.z = std::stof(token);
        } else if (token == "WorldSize:") {
            is >> token;
            data.worldSize = std::stoi(token);
        } else if (token == "WorldData:") {
            is >> data.worldEnc;
        }
    }

    return data;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Must specify at least one render file" << std::endl;
        return -1;
    }

    Raytracer raytracer(
		1280, 720, 90.0f, 0.01f, 18.0f, loadWorldData(argv[1]).worldSize
	);

    for (int i = 1; i < argc; i++) {
        std::string arg1 = argv[i];
        std::cout << "Processing " << arg1 << std::endl;
        WorldData data = loadWorldData(arg1);

        Benchmarker::Start("Draw");
        raytracer.Draw(data, 1);
        Benchmarker::End("Draw");
        arg1.erase(arg1.size() - 3);
    	arg1 += "png";
    	raytracer.Canvas.Save(arg1);
    }


	return 0;
}
