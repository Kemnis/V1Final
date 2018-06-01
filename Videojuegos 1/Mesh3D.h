#pragma once
//#ifndef _Mesh3D_H_
//#define _Mesh3D_H_
#include "BasicShader.h"
#include <d3d11.h>
#include <directxmath.h>
#include "Transforms.h"
#include "Vertex.h"
using namespace DirectX;
class Mesh3D : DxComponent<Mesh3D>, Transforms
{
private:
	
public:
	Mesh3D();
	~Mesh3D();

	bool Initialize(string NameOfFigure);
	void Shutdown();
	void Draw();
	void BindMesh();

	string Name;
	Vertex MeshVertex;
protected:

private:
	void DefineTriangle();
	void DefineSquare();
	void DefineCube(XMFLOAT3 size);
	void DefineSphere(float diameter, size_t tessellation);
	void DefineGeoSphere(float diameter, size_t tessellation);
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
};

//#endif