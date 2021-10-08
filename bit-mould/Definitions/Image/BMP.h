#pragma once
#include <vector>
#include <thread>
#include "Color.h"

class BMP{

public:
	BMP(int width, int height);
	BMP(const BMP& other);
	~BMP();

	static const Color& GetColor(BMP& image, int x, int y);
	static void SetColor(BMP& image, const Color& color, int x, int y);

	static void ThreadedExport(BMP image, std::string path, const int& scale_x, const int& scale_y);

	static void Export(BMP& image, const char* path);
	static void Export(BMP& image, const char * path, int scale_x, int scale_y);

	int GetWidth();
	int GetHeight();

private:
	int m_width;
	int m_height;
	std::vector<Color> m_colors;
};
