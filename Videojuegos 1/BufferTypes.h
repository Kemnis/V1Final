#pragma once
#include <directxmath.h>
#include "glm\glm.hpp"

namespace ConstantBufferTypes {
	struct MatrixBuffer
	{
		XMMATRIX projectionViewWorld;
	};

	struct MaterialBuffer
	{
		glm::vec4 ColorMaterial;
		float escalar;
		float padding;
		float padding1;
		float padding2;
	};

	struct LightBuffer
	{
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec3 lightDirection;
		float padding;
	};
	
	struct MatrixBufferReflection
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX reflection;
	};

	struct CamNormBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding1;
		XMFLOAT2 normalMapTiling;
		XMFLOAT2 padding2;
	};

	struct WaterBufferType
	{
		XMFLOAT4 refractionTint;
		XMFLOAT3 lightDirection;
		float waterTranslation;
		float reflectRefractScale;
		float specularShininess;
		XMFLOAT2 padding;
	};

	struct MatrixBufferDefault
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ClipPlaneBufferType
	{
		XMFLOAT4 clipPlane;
	};

	struct LightBufferType
	{
		XMFLOAT4 lightDiffuseColor;
		XMFLOAT3 lightDirection;
		float colorTextureBrightness;
	};
}