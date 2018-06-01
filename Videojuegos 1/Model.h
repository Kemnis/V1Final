#pragma once
#include "Vertex.h"
#include "Transforms.h"
#include "RawImage.h"

class Model : DxComponent<Model>
{
public:
	Model();
	Model(string HeightmapFile,float Cells, float Width,float Height);//Mesh for Terrain
	Model(string path);//Mesh of Objects
	Model(vec2 coordPositivo, vec2 coordNegativo);//<-Width and Height my Billboard
	Model(vec4 rectBitmap, int widthScreen, int heightScreen);//Mesh for Bitmap images in 2D screens
	Model(string Oracion, int widthScreen, int heightScreen);//Construct Words
	~Model();
	bool Initialize(string primitive);
	void Draw();
	void BindMesh(D3D_PRIMITIVE_TOPOLOGY TOPOLOGY);
	void ShutdownModel();
	float GetPositionHeightMap(vec3 pos);
	bool isIntoTerrain(vec3 pos);
	bool UpdateBufferBitmap(vec4 rectBitmap, int widthScreen, int heightScreen);

	//Methods bitmaps
	void SetRect(vec4 rectBitmap);
	void SetPosition(vec2 position);
	void SetSize(vec2 size);
	void UpdateBitmap();

	//GetRadio
	double GetRadio();

	Vertex GetMesh();
	string Name;
	string Type;
	RawImage Heightmap;
	vec2 SizeXZ;
	vec2 coordPositivo, coordNegativo;
	vec4 rectBitmap;
	vec2 sizeTerrain;
	
protected:
	bool haveBehaviour;
	bool dynamicVertexBuffer;//<- For re-position bitmap2D
	Vertex Mesh;
	ID3D11Buffer *VertexBuffer, *IndexBuffer;
	
private:
	bool LoadModel(string);
	void DefineTerrain(float Cells, float Width, float Height, string HeightmapFile);
	void DefineTriangle();
	void DefineSquare();
	void DefineCube(XMFLOAT3 size);
	void DefineSphere(float diameter, size_t tessellation);
	void DefineGeoSphere(float diameter, size_t tessellation);
	void DefineBillboard();
	void InitCol(string MeshCol);
	void ConstructAWord(string Oracion, int widthScreen, int heightScreen);
	void DefineBitmap(vec4 rectBitmap, int widthScreen, int heightScreen);

	//Measure
	int m_screenWidth, m_screenHeight;

	//previous
	int m_previousWidth, m_previousHeight;
	int m_previousPosX, m_previousPosY;
	int PosX, PosY;

	int m_bitmapWidth, m_bitmapHeight;
	int CenterX, CenterY;
	
	vec3 Max, Min;
	double radio;
};


