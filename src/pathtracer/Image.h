#pragma once
#include <string>

class Pixel {
public:
	Pixel(unsigned char r, unsigned char g, unsigned char b)
		: R(r), G(g), B(b) {}
	Pixel() {}
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
};

class Image {
public:

	Image(int width, int height);
	~Image();
	unsigned char* GetData() const;
	Pixel GetPixel(int x, int y) const;
	void SetPixel(int x, int y, const Pixel& p) const;
	const int GetWidth() const;
	const int GetHeight() const;
	void Save(const std::string& fileName) const;

private:
	int m_Width = 0;
	int m_Height = 0;
	unsigned char* m_Data = nullptr;

};