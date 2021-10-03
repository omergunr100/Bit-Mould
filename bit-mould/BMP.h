#pragma once
#include <vector>
#include "Color.h"

class BMP{

public:
	BMP(int width, int height);
	~BMP();

	Color GetColor(int x, int y);
	void SetColor(const Color& color, int x, int y);

	void Export(const char* path);
	void Export(const char * path, int scale_x, int scale_y);

	int GetWidth();
	int GetHeight();

private:
	int m_width;
	int m_height;
	std::vector<Color> m_colors;
};
