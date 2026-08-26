#pragma once
#include "Camera.h"
#include "Image.h"
#include "GPUVK.h"
#include "SceneData.h"


struct WorldData {
    glm::vec3 camPos;
    glm::vec3 camDir;
    int worldSize;
    std::string worldEnc;
};

class Raytracer {
public:

	Raytracer(int width, int height, float fov, float aperture, float focusDistance, int worldSize);
	void Draw(const WorldData& data, int numAccumFrames);

	Camera Cam;
	Image Canvas;

private:

	void UpdateGPUData(const WorldData& data);


	std::unique_ptr<GPUVK> m_GPUVK;
	int m_Width = 0;
	int m_Height = 0;
	int m_WorldSize = 0;
	int m_FrameCount = 0;

	unsigned char* m_WorldData;
	int m_WorldDataSizeInBytes;

};
