#include "BMP.h"
#include<iostream>
#include<fstream>

BMP::BMP(int width, int height)
	:m_width(width), m_height(height), m_colors(std::vector<Color>(width*height))
{
}

BMP::BMP(const BMP & other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_colors = other.m_colors;
}

BMP::~BMP()
{
}

const Color& BMP::GetColor(BMP& image, int x, int y)
{
	return image.m_colors[y*image.m_width+x];
}

void BMP::SetColor(BMP& image, const Color & color, int x, int y)
{
	image.m_colors[y*image.m_width + x].r = color.r;
	image.m_colors[y*image.m_width + x].g = color.g;
	image.m_colors[y*image.m_width + x].b = color.b;
}

void BMP::ThreadedExport(BMP image, std::string path, const int& scale_x, const int& scale_y)
{
	if (image.m_height == scale_y && image.m_height == scale_x) {
		Export(image, path.c_str());
	}
	else {
		Export(image, path.c_str(), scale_x, scale_y);
	}
}

void BMP::Export(BMP& image, const char * path)
{
	std::ofstream f;
	f.open(path,std::ios::out | std::ios::binary); // write binary

	if (!f.is_open()) {
		std::cout << "File could not be opened!\n";
		return;
	}


	unsigned char bmpPad[3] = { 0,0,0 };
	const int paddingAmount = ((4 - (image.m_width * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + image.m_width * image.m_height * 3 + paddingAmount * image.m_height;

	unsigned char fileHeader[fileHeaderSize];

	// File type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	// File size
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
	// Reserved 1 (Not used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	// Reserved 2 (Not used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	// Pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;
	
	unsigned char informationHeader[informationHeaderSize];
	// Header size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	// Image width
	informationHeader[4] = image.m_width;
	informationHeader[5] = image.m_width >> 8;
	informationHeader[6] = image.m_width >> 16;
	informationHeader[7] = image.m_width >> 24;
	// Image height
	informationHeader[8] = image.m_height;
	informationHeader[9] = image.m_height >> 8;
	informationHeader[10] = image.m_height >> 16;
	informationHeader[11] = image.m_height >> 24;
	// Planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	// Bits per pixel (RGB)
	informationHeader[14] = 24;
	informationHeader[15] = 0;
	// Compression (No compression);
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;
	// Image size (No compression)
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;
	// X pixels per meter (Not specified)
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;
	// Y pixels per meter (Not specified)
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;
	// Total colors (Color palette not used)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;
	// Important colors (Generally ignored)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y = 0; y < image.m_height; y++) {
		for (int x = 0; x < image.m_width; x++) {
			unsigned char r = static_cast<unsigned char>(GetColor(image, x, y).r * 255.0f);
			unsigned char g = static_cast<unsigned char>(GetColor(image, x, y).g * 255.0f);
			unsigned char b = static_cast<unsigned char>(GetColor(image, x, y).b * 255.0f);

			unsigned char color[] = { b, g, r };
			f.write(reinterpret_cast<char*>(color), 3);
		}
		f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	f.close();	
	std::cout << "File created\n";
}

void BMP::Export(BMP& image, const char * path, int scale_x, int scale_y)
{
	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary); // write binary

	if (!f.is_open()) {
		std::cout << "File could not be opened!\n";
		return;
	}


	unsigned char bmpPad[3] = { 0,0,0 };
	const int paddingAmount = ((4 - (scale_x * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + scale_x * scale_y * 3 + paddingAmount * scale_y;

	unsigned char fileHeader[fileHeaderSize];

	// File type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	// File size
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
	// Reserved 1 (Not used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	// Reserved 2 (Not used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	// Pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];
	// Header size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	// Image width
	informationHeader[4] = scale_x;
	informationHeader[5] = scale_x >> 8;
	informationHeader[6] = scale_x >> 16;
	informationHeader[7] = scale_x >> 24;
	// Image height
	informationHeader[8] = scale_y;
	informationHeader[9] = scale_y >> 8;
	informationHeader[10] = scale_y >> 16;
	informationHeader[11] = scale_y >> 24;
	// Planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	// Bits per pixel (RGB)
	informationHeader[14] = 24;
	informationHeader[15] = 0;
	// Compression (No compression);
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;
	// Image size (No compression)
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;
	// X pixels per meter (Not specified)
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;
	// Y pixels per meter (Not specified)
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;
	// Total colors (Color palette not used)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;
	// Important colors (Generally ignored)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);
	int h_count = 0, w_count = 0;
	int y_ratio = (int)floor((float)scale_y / (float)image.m_height), x_ratio = (int)floor((float)scale_x / (float)image.m_width);
	std::cout << "The x_scale ratio is: " << (int)floor((float)scale_x / (float)image.m_width) << ", The y_scale ratio is: " << (int)floor((float)scale_y / (float)image.m_height) << "\n";
	for (int y = 0; y < scale_y; y++) {
		w_count = 0;
		for (int x = 0; x < scale_x; x++) {
			//std::cout << "Row: " << y << ", Col: " << x << "\n";
			//std::cout << "h_count: " << h_count << ", w_count: " << w_count << "\n";
			unsigned char r = static_cast<unsigned char>(GetColor(image, (int)floor((float)w_count / x_ratio), (int)floor((float)h_count / y_ratio)).r * 255.0f);
			unsigned char g = static_cast<unsigned char>(GetColor(image, (int)floor((float)w_count / x_ratio), (int)floor((float)h_count / y_ratio)).g * 255.0f);
			unsigned char b = static_cast<unsigned char>(GetColor(image, (int)floor((float)w_count / x_ratio), (int)floor((float)h_count / y_ratio)).b * 255.0f);
			unsigned char color[] = { b, g, r };
			f.write(reinterpret_cast<char*>(color), 3);
			w_count++;
		}
		h_count++;
		f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	f.close();
	std::cout << "File created\n";
}

int BMP::GetWidth()
{
	return m_width;
}

int BMP::GetHeight()
{
	return m_height;
}