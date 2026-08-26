#include "Raytracer.h"
#include "../base64.h"
#include <cstring>

Raytracer::Raytracer(int width, int height, float fov, float aperture, float focusDistance, int worldSize)
	: Cam(glm::vec3(0.0), glm::vec3(1.0), width, height, fov, aperture, focusDistance), Canvas(width, height),
	m_Width(width), m_Height(height)
{
	m_WorldSize = worldSize;
	m_WorldDataSizeInBytes = worldSize * worldSize * worldSize * 16 * 16 * 16 * 4 + sizeof(SceneData);
    m_WorldData = new unsigned char[m_WorldDataSizeInBytes];

    m_GPUVK = std::make_unique<GPUVK>(m_Width, m_Height, m_WorldDataSizeInBytes);
}

void Raytracer::Draw(const WorldData& wData, int numAccumFrames)
{
    uint64_t* data = new uint64_t[m_Width * m_Height * 4];
    memset(data, 0, m_Width * m_Height * 4 * sizeof(uint64_t));

    for (int i = 0; i < numAccumFrames; i++) {
        UpdateGPUData(wData); // Make sure this function is relatively light (doesn't decompress world data every frame)
        m_GPUVK->Run(Canvas, m_WorldData, m_WorldDataSizeInBytes);
        m_FrameCount++;

        for (int i = 0; i < m_Width * m_Height * 4; i++) {
            data[i] += Canvas.GetData()[i];
        }
    }

    for (int i = 0; i < m_Width * m_Height * 4; i++) {
        Canvas.GetData()[i] = data[i] / numAccumFrames;
    }

    delete[] data;
}

void Raytracer::UpdateGPUData(const WorldData& wData)
{
    Cam.Position = wData.camPos;
    Cam.Direction = wData.camDir;
    Cam.Update();

    std::vector<BYTE> bytes = base64_decode(wData.worldEnc);
    struct BlockSegment {
        unsigned char r, g, b;
        bool visible;
        unsigned int count;
    };

    if (bytes.size() % sizeof(BlockSegment) != 0) {
        std::cerr << "World integrity check failed... Found " << bytes.size() << " bytes." << std::endl;
        return;
    }

    int numSegments = bytes.size() / sizeof(BlockSegment);

    std::vector<BlockSegment> segments;
    segments.resize(numSegments);

    memcpy(segments.data(), bytes.data(), bytes.size());

    int idx = sizeof(SceneData);

    for (int i = 0; i < segments.size(); i++) {
        for (int j = 0; j < segments[i].count; j++) {
            m_WorldData[idx++] = segments[i].r;
            m_WorldData[idx++] = segments[i].g;
            m_WorldData[idx++] = segments[i].b;
            m_WorldData[idx++] = segments[i].visible;
        }
    }

    SceneData data;
	// General Information
	data.width = m_Width;
	data.height = m_Height;
	data.worldSize = m_WorldSize;
	data.frameSeed = m_FrameCount;

	// Camera
	data.camera.position = Cam.Position;
	data.camera.lowerLeftCorner = Cam.LowerLeftCorner;
	data.camera.horizontal = Cam.Horizontal;
	data.camera.vertical = Cam.Vertical;
	data.camera.w = Cam.W;
	data.camera.u = Cam.U;
	data.camera.v = Cam.V;
	data.camera.lensRadius = Cam.LensRadius;

	memcpy(m_WorldData, &data, sizeof(SceneData));
}
