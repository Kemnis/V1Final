#include "stdafx.h"

RawImage::RawImage() {}
RawImage::RawImage(std::string fileName) {
	this->LoadBitmapFromFile(fileName);
}

bool RawImage::LoadBitmapFromFile(std::string fileName) {

	std::ifstream file(fileName.c_str(), std::ios::binary);
	if (!file) {
		return false;
	}
	byte* headerPointer[2] = { nullptr, nullptr };
	headerPointer[0] = new byte[sizeof(BITMAPFILEHEADER)];
	headerPointer[1] = new byte[sizeof(BITMAPINFOHEADER)];

	file.read((char*)headerPointer[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)headerPointer[1], sizeof(BITMAPINFOHEADER));

	BITMAPFILEHEADER* bmpHeader = (BITMAPFILEHEADER*)headerPointer[0];
	BITMAPINFOHEADER* bmpInfoHeader = (BITMAPINFOHEADER*)headerPointer[1];;

	if (bmpHeader->bfType != 0x4D42)
	{
		// Esto no es un bitmap!
		return false;
	}


	this->width = bmpInfoHeader->biWidth;
	this->height = bmpInfoHeader->biHeight;
	this->pixels.resize(bmpInfoHeader->biSizeImage);
	this->bitsPerPixel = bmpInfoHeader->biBitCount;
	this->bytesPerPixel = this->bitsPerPixel / 8;
	/*pixel = nullptr;
	pixel = new byte[bmpInfoHeader->biSizeImage];*/
	file.seekg(bmpHeader->bfOffBits);
	//file.read((char*)(pixel), bmpInfoHeader->biSizeImage);
	file.read((char*)(&pixels[0]), bmpInfoHeader->biSizeImage);

	/*
	int padding = this->width % 4;
	if (padding > 0) {

	}
	*/

	byte tempPixel= 0;
	for (unsigned long i = 0; i < bmpInfoHeader->biSizeImage - 2; i += 3) {
		tempPixel = pixels[i];
		pixels[i] = pixels[i + 2];
		pixels[i + 2] = tempPixel;
	}

	delete headerPointer[0];
	delete headerPointer[1];

	return true;
}

RGB* RawImage::GetPixelRGB(unsigned int index) {
	return (RGB*)&this->pixels[index*sizeof(RGB)];
}

RGBA* RawImage::GetPixelRGBA(unsigned int index) {
	return (RGBA*)&this->pixels[index*sizeof(RGBA)];
}

