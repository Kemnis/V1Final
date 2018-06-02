#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "ConstantBuffer.h"
#include "Material.h"

using namespace DirectX;
using namespace std;


enum class ShaderType {
	BasicShader = 1,
	MaterialShader = 2,
	BasicLShader = 3,
	MaterialLShader = 4,
	TerrainShader = 5,
	SkydomeShader = 6,
	GUIShader = 7,
	WaterShader = 8,
	ReflectionShader = 9
};

class Shader : public DxComponent<Shader>
{
protected:
	bool Create(D3D11_INPUT_ELEMENT_DESC* inputLayout, unsigned int layoutCount, const std::string&, const std::string&);
	void AddConstantBuffer(const string& bufferName, const ConstantBuffer &buffer);
public:
	Shader();
	Shader( ShaderType type );
	~Shader();
	int flagLight;

	ShaderType type;

	virtual bool Initialize(const std::string&, const std::string& ) = 0;
	void Shutdown();
	void BindShader();

	bool SetShaderParameters(XMMATRIX, XMMATRIX, XMMATRIX);
	bool SetShaderParametersWater(XMMATRIX, XMMATRIX, XMMATRIX,XMMATRIX);
	bool SetShaderParametersDefualt(XMMATRIX, XMMATRIX, XMMATRIX);
	bool SetShaderConstantBuffer(const std::string& bufferName, const void *values);
	string Name;

private:
	void OutputShaderErrorMessage(ID3D10Blob*, LPCTSTR);
	std::wstring s2ws(const std::string& s);

private:
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* Layout;
	std::map<string, ConstantBuffer> constantBufferMap;

};

