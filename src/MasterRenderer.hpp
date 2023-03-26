#pragma once

#include "World.hpp"
#include "Shader.hpp"
#include "ShadowMap.hpp"
#include "Camera.hpp"

class MasterRenderer {
public:

	void init();
	void render(ShadowMap* smap, Camera* camera, World* world);

	//unsigned char blockLayout = (unsigned char)i;
		/*textureData[i * 3 * 3 + 0] = ((blockLayout & 0b10000000) >> 7) * 255;
		textureData[i * 3 * 3 + 1] = ((blockLayout & 0b01000000) >> 6) * 255;
		textureData[i * 3 * 3 + 2] = ((blockLayout & 0b00100000) >> 5) * 255;
		textureData[i * 3 * 3 + 3] = ((blockLayout & 0b00010000) >> 4) * 255;
		textureData[i * 3 * 3 + 4] = 0;
		textureData[i * 3 * 3 + 5] = ((blockLayout & 0b00001000) >> 3) * 255;
		textureData[i * 3 * 3 + 6] = ((blockLayout & 0b00000100) >> 2) * 255;
		textureData[i * 3 * 3 + 7] = ((blockLayout & 0b00000010) >> 1) * 255;
		textureData[i * 3 * 3 + 8] = ((blockLayout & 0b00000001) >> 0) * 255;*/

	Shader shader;
	GLuint textureArray;

};