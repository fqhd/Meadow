#include "Raytracer.h"
#include "Benchmarker.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>

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
    // Default values
    float fov = 90.0f;
    int spp = 1;
    std::vector<std::string> renderFiles;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-fov") == 0) {
            if (i + 1 < argc) {
                fov = std::stof(argv[++i]);
            } else {
                std::cerr << "Error: -fov requires a numeric value" << std::endl;
                return -1;
            }
        } else if (std::strcmp(argv[i], "-spp") == 0) {
            if (i + 1 < argc) {
                spp = std::stoi(argv[++i]);
                if (spp < 1) {
                    std::cerr << "Error: spp must be at least 1" << std::endl;
                    return -1;
                }
            } else {
                std::cerr << "Error: -spp requires an integer value" << std::endl;
                return -1;
            }
        } else {
            renderFiles.push_back(argv[i]);
        }
    }

    if (renderFiles.empty()) {
        std::cerr << "Must specify at least one render file" << std::endl;
        return -1;
    }

    // Use the first render file to get world size for constructor
    WorldData firstData = loadWorldData(renderFiles[0]);
    Raytracer raytracer(
        1280, 720, fov, 0.01f, 18.0f, firstData.worldSize
    );

    for (const std::string& file : renderFiles) {
        std::cout << "Processing " << file << std::endl;
        WorldData data = loadWorldData(file);

        Benchmarker::Start("Draw");
        raytracer.Draw(data, spp);   // <-- spp passed here
        Benchmarker::End("Draw");

        std::string outFile = file;
        outFile.erase(outFile.size() - 3);  // remove ".txt" (assumes 3‑character extension)
        outFile += "png";
        raytracer.Canvas.Save(outFile);
    }

    return 0;
}
