#pragma once
#include "WICTextureLoader.h"
#include <vector>
using std::vector;
using namespace std;
using namespace DirectX;

class Texture : DxComponent<Texture>
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	~Texture();

	string Name;
	bool Initialize(string);
	ID3D11ShaderResourceView* GetTexture();
	void Shutdown();

	void BindTexture(unsigned int slot);

private:
	string LoadTarga(string, int&, int&);
	string LoadTextureTarga(string);
	string LoadTextureWic(string);
	bool LoadBipmap(string);

private:
	ID3D11ShaderResourceView * Texture2D;
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11SamplerState* Sampler;

};