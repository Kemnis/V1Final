#include "stdafx.h"
#include <comdef.h>  // you will need this
#include "WICTextureLoader.h"

Texture::Texture()
{
	m_targaData = 0;
	m_texture = 0;
	Texture2D = 0;
}

Texture::~Texture()
{

}


bool Texture::Initialize(string filename)
{
	string result;
	
	//*************Conversion WCHAR a Char*, de Char* a Char y de Char a String*******************
	//WideCharToMultiByte(CP_ACP, 0, filename, -1, ch, 260, &DefChar, NULL);
	//--------------------------------------------------------------------------------------------


	// Carga la textura del modelo, tiene 2 modos, tga y otros.
	if (filename != "")
	{
		if (filename.substr(filename.find_last_of(".") + 1) == "tga")
			result = LoadTextureTarga(filename);
		else
			result = LoadTextureWic(filename);
		if (result != "")
			return false;//string("No se pudo cargar la textura del modelo correctamente: \n Texture ") + string(" \nTEXTURA ") + string(fn) + string(" ") + string(result);
	}

	// Create a Light sampler state description.
	D3D11_SAMPLER_DESC samplerDesc {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &Sampler);

	return true;
}


string Texture::LoadTextureWic(string filename)
{
	//Debugg Vars
	Microsoft::WRL::ComPtr<ID3D11Resource> res;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	D3D11_TEXTURE2D_DESC desc;
	//-----------------------
	std::wstring widestr = std::wstring(filename.begin(), filename.end());
	const wchar_t* szName = widestr.c_str();
	HRESULT TextureWic = CreateWICTextureFromFile(device, deviceContext,
		szName, res.GetAddressOf(), &Texture2D);
	//Debugg
	if (SUCCEEDED(TextureWic))
	{
		TextureWic = res.As(&tex);
		tex->GetDesc(&desc);
		return "";
	}
	else
		return "\nNo se pudo crear el Wic de Textura";
}

string Texture::LoadTextureTarga(string filename)
{
	string result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load the targa image data into memory.
	result = LoadTarga(filename, height, width);
	if (result != "")
	{
		return result;
	}

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		return string("No se pudo crear texture2D tga");;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &Texture2D);
	if (FAILED(hResult))
	{
		return string("No se pudo crear TextureResourceView tga");
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(Texture2D);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_targaData;
	m_targaData = 0;

	return "";
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return Texture2D;
}


string Texture::LoadTarga(string filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;
	std::wstring widestr = std::wstring(filename.begin(), filename.end());
	const wchar_t* szName = widestr.c_str();
	_bstr_t b(szName);
	const char* c = b;
	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, c, "rb");
	if (error != 0)
	{
		return string("Error al abrir la imagen tga " + string(b));
	}

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return string("Error al intentar leer la imagen tga " + string(b));
	}

	// Get the important information from the header.
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32)
	{
		return string("La imagen tga no es un formato bpp 32 bit " + string(b));
	}

	// Calculate the size of the 32 bit image data.
	imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return string("Error al crear buffer de imagen tga " + string(b));
	}

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return string("Error al intentar leer la imagen tga fread " + string(b));
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return string("Error al cerrar la imagen tga " + string(b));
	}

	// Allocate memory for the targa destination data.
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		return string("Error al asingar memoria a la imagen tga " + string(b));;
	}

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data.
	k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (j = 0; j<height; j++)
	{
		for (i = 0; i<width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

														 // Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = 0;

	return "";
}

void Texture::Shutdown()
{
	if (Sampler) {
		Sampler->Release();
		Sampler = 0;
	}

	// Release the texture view resource.
	if (Texture2D)
	{
		Texture2D->Release();
		Texture2D = 0;
	}

	// Release the texture.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data.
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return;
}

void Texture::BindTexture(unsigned int slot)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(slot, 1, &Sampler);
	deviceContext->PSSetShaderResources(slot, 1, &Texture2D);
}

