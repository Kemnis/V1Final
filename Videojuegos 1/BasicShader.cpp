#include "BasicShader.h"
#include "stdafx.h"

BasicShader::BasicShader(std::string vsSource, std::string psSource) : Shader(ShaderType::BasicShader) { 
	Initialize(vsSource, psSource);
}

BasicShader::BasicShader(std::string vsSource, std::string psSource, int WithLight) : Shader(ShaderType::BasicLShader) {
	flagLight = WithLight;
	Initialize(vsSource, psSource);
}


bool BasicShader::Initialize(const std::string& vsSource, const std::string& psSource)
{
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	
	ConstantBuffer materialConstantBuffer(CUSTOM_BUFFER_1, ConstantBufferLocation::PixelShader, sizeof(ConstantBufferTypes::MaterialBuffer));
	this->AddConstantBuffer("MaterialBuffer", materialConstantBuffer);
	if (flagLight == 1)
	{
		ConstantBuffer lightConstantBuffer(CUSTOM_BUFFER_2, ConstantBufferLocation::PixelShader, sizeof(ConstantBufferTypes::LightBuffer));
		this->AddConstantBuffer("LightBuffer", lightConstantBuffer);
	}

	this->Create(polygonLayout, numElements, vsSource, psSource);

	

	return true;
}
