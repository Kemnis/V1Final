#include "Shader.h"
#include "stdafx.h"

Shader::Shader()
{
	VertexShader = 0;
	PixelShader = 0;
	Layout = 0;
}

Shader::Shader(ShaderType type)
{
	VertexShader = 0;
	PixelShader = 0;
	Layout = 0;
	ConstantBuffer matrixConstantBuffer(MATRIX_BUFFER_ID, ConstantBufferLocation::VertexShader, sizeof(ConstantBufferTypes::MatrixBuffer));
	this->AddConstantBuffer("MatrixBuffer", matrixConstantBuffer);
	this->type = type;
}

Shader::~Shader()
{
}

bool Shader::Create(D3D11_INPUT_ELEMENT_DESC* inputLayout, unsigned int layoutCount, const std::string& vsSource, const std::string& fsSource)
{
	std::wstring stemp = s2ws(vsSource);
	LPCWSTR vsFileName = stemp.c_str();

	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* errorMessage;
	vertexShaderBuffer = 0;
	errorMessage = 0;
	// Compile the vertex shader code.
	HRESULT result = D3DCompileFromFile(vsFileName, NULL, NULL, "VertexProgram", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, vsFileName);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(specsDx->GetHwnd(), vsFileName, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	std::wstring stemp1 = s2ws(fsSource);
	LPCWSTR psFileName = stemp1.c_str();

	ID3D10Blob* pixelShaderBuffer;
	pixelShaderBuffer = 0;
	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFileName, NULL, NULL, "PixelProgram", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, psFileName);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(specsDx->GetHwnd(), psFileName, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &VertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &PixelShader);
	if (FAILED(result))
	{
		return false;
	}
	
	

	result = device->CreateInputLayout(inputLayout, layoutCount, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &Layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

void Shader::AddConstantBuffer(const string& bufferName, const ConstantBuffer& buffer) {
	constantBufferMap.insert_or_assign(bufferName, buffer);
	//constantBuffers.push_back(buffer);
}

std::wstring Shader::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, LPCTSTR  shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(specsDx->GetHwnd(), L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool Shader::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	XMMATRIX projectionViewMatrix;
	XMMATRIX projectionViewWorldMatrix;

	// Transpose the matrices to prepare them for the shader.
	projectionViewWorldMatrix = XMMatrixMultiply(worldMatrix, viewMatrix);
	projectionViewWorldMatrix = XMMatrixMultiplyTranspose(projectionViewWorldMatrix, projectionMatrix);

	ConstantBufferTypes::MatrixBuffer tempBuffer{};
	tempBuffer.projectionViewWorld = projectionViewWorldMatrix;

	this->SetShaderConstantBuffer("MatrixBuffer", &tempBuffer);

	return true;
}

bool Shader::SetShaderParametersWater(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX reflectionMatrix)
{
	ConstantBufferTypes::MatrixBufferReflection tempBuffer{};
	tempBuffer.world = XMMatrixTranspose(worldMatrix);
	tempBuffer.view = XMMatrixTranspose(viewMatrix);
	tempBuffer.projection = XMMatrixTranspose(projectionMatrix);
	tempBuffer.reflection = XMMatrixTranspose(reflectionMatrix);

	this->SetShaderConstantBuffer("MatrixBuffer", &tempBuffer);

	return true;
}

bool Shader::SetShaderParametersDefualt(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	ConstantBufferTypes::MatrixBufferDefault tempBuffer{};
	tempBuffer.world = XMMatrixTranspose(worldMatrix);
	tempBuffer.view = XMMatrixTranspose(viewMatrix);
	tempBuffer.projection = XMMatrixTranspose(projectionMatrix);

	this->SetShaderConstantBuffer("MatrixBuffer", &tempBuffer);

	return true;
}

bool Shader::SetShaderConstantBuffer(const string& bufferName, const void* values) {
	if (this->constantBufferMap.find(bufferName) != this->constantBufferMap.end()) {
		constantBufferMap.at(bufferName).Update(values);
		return true;
	}
	else {
		return false;
	}
}

void Shader::BindShader()
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(Layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(VertexShader, NULL, 0);
	deviceContext->PSSetShader(PixelShader, NULL, 0);
	for (std::map<std::string, ConstantBuffer>::iterator iter = constantBufferMap.begin(); iter != constantBufferMap.end(); ++iter)
	{
		if (iter->second.location == ConstantBufferLocation::VertexShader) {
			deviceContext->VSSetConstantBuffers(iter->second.id, 1, &iter->second.buffer);
		}
		else {
			deviceContext->PSSetConstantBuffers(iter->second.id, 1, &iter->second.buffer);
		}
	}
	
}



void Shader::Shutdown()
{

	// Release the layout.
	if (Layout)
	{
		Layout->Release();
		Layout = 0;
	}

	// Release the pixel shader.
	if (PixelShader)
	{
		PixelShader->Release();
		PixelShader = 0;
	}

	// Release the vertex shader.
	if (VertexShader)
	{
		VertexShader->Release();
		VertexShader = 0;
	}

	return;
}