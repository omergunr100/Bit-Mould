#include "PNG.h"
#include<fstream>
#include<iostream>

PNG::PNG(int width, int height)
	:m_width(width), m_height(height), m_colors(std::vector<Color>(width*height))
{
}

PNG::~PNG()
{
}

Color PNG::GetColor(int x, int y)
{
	return m_colors[y*m_width+x];
}

void PNG::SetColor(const Color & color, int x, int y)
{
	m_colors[y*m_width + x].r = color.r;
	m_colors[y*m_width + x].g = color.g;
	m_colors[y*m_width + x].b = color.b;
}

void PNG::Export(const char * path)
{

	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary); // write binary

	if (!f.is_open()) {
		std::cout << "File could not be opened!\n";
		return;
	}

	const int fileHeaderSize = 8;
	unsigned char fileHeader[fileHeaderSize];

	// Writing file header
		// Magic number
	fileHeader[0] = 0x89;
		// Png identifier string
	fileHeader[1] = 'P';
	fileHeader[2] = 'N';
	fileHeader[3] = 'G';
		// Dos line ending
	fileHeader[4] = 0x0D;
	fileHeader[5] = 0x0A;
		// EOF char
	fileHeader[6] = 0x1A;
		// Unix line ending
	fileHeader[7] = 0x0A;


}

void PNG::Export(const char * path, int scale_x, int scale_y)
{
}

unsigned char * IHDR::GetData()
{
	return data;
}

void IHDR::Width(const int & width)
{
	data[0] = width;
	data[1] = width >> 8;
	data[2] = width >> 16;
	data[3] = width >> 24;
}

void IHDR::Height(const int & height)
{
	data[4] = height;
	data[5] = height >> 8;
	data[6] = height >> 16;
	data[7] = height >> 24;
}

void IHDR::BitDepth(const unsigned char & bit_depth)
{
	data[8] = bit_depth;
}

void IHDR::ColorDepth(const unsigned char & color_depth)
{
	data[9] = color_depth;
}

void IHDR::CompressionMethod(const unsigned char & compression)
{
	data[10] = compression;
}

void IHDR::FilterMethod(const unsigned char & filter)
{
	data[11] = filter;
}

void IHDR::InterfaceMethod(const unsigned char & interfaceMethod)
{
	data[12] = interfaceMethod;
}
