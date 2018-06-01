#pragma once

#include<vector>
#include<string>

typedef unsigned char ubyte;

struct RGB {
	byte r;
	byte g;
	byte b;
};

struct RGBA {
	byte r;
	byte g;
	byte b;
	byte a;
};

class RawImage {
	typedef unsigned int uint;

public:
	RawImage();
	RawImage(std::string fileName);
	bool LoadBitmapFromFile(std::string fileName);
	uint width;
	uint height;
	byte bitsPerPixel;
	byte bytesPerPixel;
	std::vector<byte> pixels;
	byte *pixel;
	
	RGB* GetPixelRGB(unsigned int index);
	RGBA* GetPixelRGBA(unsigned int index);
};
