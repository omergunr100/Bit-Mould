#pragma once
#include<vector>
#include "BMP.h"

struct IHDR {

private:
	const int size;
	unsigned char data[13];
	
public:
	unsigned char* GetData();

	void Width(const int& width);

	void Height(const int& height);

	void BitDepth(const unsigned char& bit_depth);

	void ColorDepth(const unsigned char& color_depth); // Color type (RGB == 2)

	void CompressionMethod(const unsigned char& compression); // Can be 0

	void FilterMethod(const unsigned char& filter); // Can be 0

	void InterfaceMethod(const unsigned char& interfaceMethod); // Can be 0
};

class PNG {

public:
	PNG(int width, int height);
	~PNG();

	Color GetColor(int x, int y);
	void SetColor(const Color& color, int x, int y);

	void Export(const char* path);
	void Export(const char* path, int scale_x, int scale_y);
	
private:
	int m_width;
	int m_height;
	std::vector<Color> m_colors;
};