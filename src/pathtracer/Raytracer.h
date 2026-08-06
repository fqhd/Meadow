#pragma once
#include "Camera.h"
#include "Image.h"
#include "GPUVK.h"
#include "SceneData.h"

class Raytracer {
public:

	Raytracer(int width, int height, const glm::vec3& camPos, const glm::vec3& lookAt, float fov, float aperture, float focusDistance, int worldSize, const std::string& encoding);
	void Draw(int numAccumFrames);

	Camera Camera;
	Image Canvas;

private:

	void UpdateGPUData();


	std::unique_ptr<GPUVK> m_GPUVK;
	int m_Width = 0;
	int m_Height = 0;
	int m_WorldSize = 0;
	int m_FrameCount = 0;

	unsigned char* m_WorldData;
	int m_WorldDataSizeInBytes;

};
