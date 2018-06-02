////////////////////////////////////////////////////////////////////////////////
// Filename: waterclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _WATERCLASS_H_
#define _WATERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: WaterClass
////////////////////////////////////////////////////////////////////////////////
class Water
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Water();
	~Water();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer *VertexBuffer, ID3D11Buffer*IndexBuffer, float waterHeight, float waterRadius);
	void Shutdown();
	void Frame();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	float GetWaterHeight();
	XMFLOAT2 GetNormalMapTiling();
	float GetWaterTranslation();
	float GetReflectRefractScale();
	XMFLOAT4 GetRefractionTint();
	float GetSpecularShininess();

private:
	bool InitializeBuffers(ID3D11Device*, ID3D11Buffer *VertexBuffer, ID3D11Buffer*IndexBuffer, float waterRadius);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	float m_waterHeight;
	int m_vertexCount, m_indexCount;
	XMFLOAT2 m_normalMapTiling;
	float m_waterTranslation;

	float m_reflectRefractScale;
	XMFLOAT4 m_refractionTint;
	float m_specularShininess;
};

#endif