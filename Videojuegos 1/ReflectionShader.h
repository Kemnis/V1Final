////////////////////////////////////////////////////////////////////////////////
// Filename: ReflectionShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ReflectionShader_H_
#define _ReflectionShader_H_

#include "Shader.h"

class ReflectionShader : public Shader
{

public:
	ReflectionShader(std::string vsSource, std::string psSource);
	~ReflectionShader();

	bool Initialize(std::string vsSource, std::string psSource);

	/*
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT4,
				XMFLOAT3, float, XMFLOAT4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT4,
							 XMFLOAT3, float, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_clipPlaneBuffer;
	ID3D11Buffer* m_lightBuffer;
	*/
};

#endif