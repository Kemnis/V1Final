#include "stdafx.h"

Vertex::Vertex()
{
	ClearVertex();
}


vector<unsigned long> Vertex::IndexResult() {
	vector<unsigned long> resulti;
	resulti.reserve(IndexCount);

	for (int i = 0; i < (IndexCount); i++) {
		resulti.push_back(i);
	}
	return resulti;
}

vector<unsigned long> Vertex::GetIndex() {
	return indices;
}

void Vertex::DoFinalMesh()
{
	FinalMesh.reserve(VertexCount);
	
	for (int i = 0; i < (VertexCount); i++) {
		VertexType nuevo;
		nuevo.position = XMFLOAT3(GetVertex(i).x, GetVertex(i).y, GetVertex(i).z);
		nuevo.texture = XMFLOAT2(GetTexture(i).x, GetTexture(i).y);
		nuevo.normal = XMFLOAT3(GetNormal(i).x, GetNormal(i).y, GetNormal(i).z);
		FinalMesh.push_back(nuevo);
	}
}

void Vertex::ClearVertex()
{
	IndexCount = 0;
	VertexCount = 0;
	indexNormals = 0;
	indexUv = 0;
	indexTriangles = 0;

	vertices.clear();
	uv.clear();
	normal.clear();
	indices.clear();

	TriangleFaceVertex.clear();
	TriangleFaceTexture.clear();
	TriangleFaceNormal.clear();

	FinalMesh.clear();
}






vec3 Vertex::GetVertex(int index)
{
	if (VertexCount == 0)
		return vec3(0, 0, 0);
	else
		return vertices[index];
}

vec2 Vertex::GetTexture(int index)
{
	if (indexUv == 0)
		return vec2(0, 0);
	else
		return uv[index];
}

vec3 Vertex::GetNormal(int index)
{
	if (indexNormals == 0)
		return vec3(0, 0, 0);
	else
		return normal.at(index);
}







vec3 Vertex::GetFirstVertex()
{
	if (VertexCount == 0)
		return vec3(0, 0, 0);
	else
		return vertices[0];
}

vec2 Vertex::GetFirstTexture()
{
	if (indexUv == 0)
		return vec2(0, 0);
	else
		return uv[0];
}

vec3 Vertex::GetFirstNormal()
{
	if (indexNormals == 0)
		return vec3(0, 0, 0);
	else
		return normal[0];
}








vec3 Vertex::GetLastVertex()
{
	if (VertexCount == 0)
		return vec3(0, 0, 0);
	else
		return vertices.at(VertexCount - 1);
}

vec2 Vertex::GetLastTexture()
{
	if (indexUv == 0)
		return vec2(0, 0);
	else
		return uv.at(indexUv - 1);
}

vec3 Vertex::GetLastNormal()
{
	if (indexNormals == 0)
		return vec3(0, 0, 0);
	else
		return normal.at(indexNormals - 1);
}

void Vertex::SetVertexTextureNormal(int index, vec3 vertex, vec2 uv, vec3 normal)
{
	vertices[index] = vertex;
	this->uv[index] = uv;
	this->normal[index] = normal;
}

//X=Vertex Y=Index Z=Uv W=Normals 
vec4 Vertex::GetAllIndex()
{
	return vec4(VertexCount,IndexCount,indexUv,indexNormals);
}

vec3 Vertex::GetTriangleFV(int index)
{
	return TriangleFaceVertex.at(index);
}

vec3 Vertex::GetTriangleFT(int index)
{
	return TriangleFaceTexture.at(index);
}

vec3 Vertex::GetTriangleFN(int index)
{
	return TriangleFaceNormal.at(index);
}





int Vertex::sizeClass()
{
	return (sizeof(vertices) + sizeof(uv) + sizeof(normal) + sizeof(TriangleFaceVertex) + sizeof(TriangleFaceTexture) + sizeof(TriangleFaceNormal));
}

void Vertex::SpecialIndexVector()
{
	if (IndexCount != indices.size())
		IndexCount = indices.size();
}

vector<vec3> Vertex::SpecialGetVertex()
{
	return vertices;
}

vector<vec2> Vertex::SpecialGetUV()
{
	return uv;
}

vector<unsigned long>* Vertex::SpecialGetIndexVector()
{
	return &indices;
}


void Vertex::AddVertex(float x, float y, float z)
{
	VertexCount++;
	vertices.push_back(vec3(x, y, z));
}

void Vertex::AddVertex(vec3 VertexVec)
{
	VertexCount++;
	vertices.push_back(VertexVec);
}

void Vertex::AddVertex(XMFLOAT3 VertexVec)
{
	VertexCount++;
	vertices.push_back(vec3(VertexVec.x, VertexVec.y, VertexVec.z));
}

void Vertex::AddVertex(XMFLOAT4 VertexVec)
{
	VertexCount++;
	vertices.push_back(vec3(VertexVec.x, VertexVec.y, VertexVec.z));
}

void Vertex::AddUV(float u, float v)
{
	indexUv++;
	uv.push_back(vec2(u, v));
}

void Vertex::AddUV(vec2 UvVec)
{
	indexUv++;
	uv.push_back(UvVec);
}

void Vertex::AddUV(XMFLOAT2 UvVec)
{
	indexUv++;
	uv.push_back(vec2(UvVec.x, UvVec.y));
}

void Vertex::AddNormals(float x, float y, float z)
{
	indexNormals++;
	normal.push_back(vec3(x, y, z));
}

void Vertex::AddNormals(vec3 InfoNormal)
{
	indexNormals++;
	normal.push_back(InfoNormal);
}

void Vertex::AddNormals(XMFLOAT3 NormalVec)
{
	indexNormals++;
	normal.push_back(vec3(NormalVec.x, NormalVec.y, NormalVec.z));
}

void Vertex::AddNormals(XMFLOAT4 NormalVec)
{
	indexNormals++;
	normal.push_back(vec3(NormalVec.x, NormalVec.y, NormalVec.z));
}

void Vertex::AddIndex(int InfoIndex)
{
	IndexCount++;
	indices.push_back(InfoIndex);
}

void Vertex::AddTriangleFaces(float TIndex1, float TIndex2, float TIndex3,
	float TTextures1, float TTextures2, float TTextures3,
	float TNormal1, float TNormal2, float TNormal3)
{

	indexTriangles++;
	TriangleFaceVertex.push_back(vec3(TIndex1, TIndex2, TIndex3));
	TriangleFaceTexture.push_back(vec3(TTextures1, TTextures2, TTextures3));
	TriangleFaceNormal.push_back(vec3(TNormal1, TNormal2, TNormal3));
}

void Vertex::AddTriangleFaces(vec3 InfoTriangleFaceVertex, vec3 InfoTriangleFaceTexture, vec3 InfoTriangleFaceNormal)
{
	indexTriangles++;
	TriangleFaceVertex.push_back(InfoTriangleFaceVertex);
	TriangleFaceTexture.push_back(InfoTriangleFaceTexture);
	TriangleFaceNormal.push_back(InfoTriangleFaceNormal);
}

void Vertex::DoFinalMeshFromTriangles(vec3 Vertex, vec2 Uv, vec3 Normal, int Index)
{
	FinalMesh.reserve(indexTriangles);
	VertexType nuevo;
	nuevo.position = XMFLOAT3(Vertex.x, Vertex.y, Vertex.z);
	nuevo.texture = XMFLOAT2(Uv.x, Uv.y);
	nuevo.normal = XMFLOAT3(Normal.x, Normal.y, Normal.z);
	FinalMesh.push_back(nuevo);
	IndexCount++;
	indices.push_back(Index);
}