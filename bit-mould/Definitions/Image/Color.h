#pragma once
struct Color {
	float r, g, b;
	static const int size = 3;

	Color();
	Color(const Color& other);
	Color(float r, float g, float b);
	~Color();
};