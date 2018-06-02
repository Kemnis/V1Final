#include "stdafx.h"

Model::Model(){
	
}

Model::Model(string path)
{
	dynamicVertexBuffer = false;
	VertexBuffer = 0;
	IndexBuffer = 0;
	if (path.substr(path.find_last_of(".") + 1) == "obj")
	{
		if (!LoadModel(path))
			ErrorFnc("No se pudo cargar el modelo");
		else
			Initialize("");
		Type = "OBJ";
	}
	else
	{
		Initialize(path);
		Type = "PRIMITIVE";
	}
}

Model::Model(string HeightmapFile, float Cells, float Width, float Height)
{
	dynamicVertexBuffer = false;
	VertexBuffer = 0;
	IndexBuffer = 0;
	DefineTerrain(Cells, Width, Height, HeightmapFile);
	Initialize("Terrain");
	Type = "Terrain";
}

Model::Model(vec2 coordPositivo, vec2 coordNegativo){
	dynamicVertexBuffer = false;
	VertexBuffer = 0;
	IndexBuffer = 0;
	this->coordPositivo = coordPositivo;
	this->coordNegativo = coordNegativo;
	Type = "Billboard";
	Initialize(Type);
}

Model::Model(vec4 rectBitmap, int widthScreen, int heightScreen){
	this->rectBitmap = rectBitmap;
	dynamicVertexBuffer = true;
	VertexBuffer = 0;
	IndexBuffer = 0;
	DefineBitmap(rectBitmap, widthScreen, heightScreen);
	Type = "Bitmap";
	Initialize(Type);
	SetRect(rectBitmap);
}

Model::Model(string Palabra, int widthScreen, int heightScreen) {
	Type = "Letter";
	Initialize(Type);
}

Model::Model(string typeName, float height, float radius) {

	Type = typeName;
	height_water = height;
	radius_water = radius;
	Initialize(Type);
}

Model::~Model()
{
}

bool Model::isIntoTerrain(vec3 pos)
{
	if ((pos.x>Heightmap.width || pos.z>Heightmap.height) || (pos.x< 0.0f || pos.z < 0.0f))
	{
		return false;
	}

	return true;
}

float Model::GetPositionHeightMap(vec3 pos)
{
	if (!isIntoTerrain(pos))
		return pos.y;

	return Mesh.GetVertex((int)pos.x + Heightmap.width*(int)pos.z).y;
}

void Model::DefineTerrain(float Cells, float Width, float Height, string HeightmapFile) {

	Heightmap.LoadBitmapFromFile(HeightmapFile);
	sizeTerrain = vec2(Heightmap.width, Heightmap.height);
	Cells = (Heightmap.width);
	vec2 CellSize = { Width ,Height };
	unsigned int index = 0;
	//Cantidad a repetir
	float countRepet = 8;
	for (int i = 0; i < (Heightmap.height); i++)
	{
		for (int j = 0; j < (Heightmap.width); j++)
		{
			//unsigned int index = ((i * (Heightmap.width * Heightmap.bytesPerPixel)) + (j * Heightmap.bytesPerPixel));     //(j)+(i* ((int)Heightmap.bitsPerPixel / 8));
			float height = ((float)Heightmap.GetPixelRGB(index)->r / 255.0f);
			//Draw in origin
			//Mesh.AddVertex(CellSize.x*0.5f - CellSize.x * (float)((float)j / (float)Heightmap.width), height*10.0f, CellSize.y*0.5f - CellSize.y * (float)((float)i / (float)Heightmap.height));
			Mesh.AddVertex(j, height*10.0f,i);
			Mesh.AddNormals(0.0f, height,0.0f);
			//Standar
			Mesh.AddUV((float)j*countRepet / (float)Heightmap.width, (float)i*countRepet / (float)Heightmap.height);
			index++;
		}

	}

	 //Set the index count to the same as the vertex count.
	for (int i = 0; i < Heightmap.height - 1; i++)
	{
		for (int j = 0; j < Heightmap.width - 1; j++)
		{
			int index1 = (Heightmap.width *   i) + j;    // Bottom left.
			int index2 = (Heightmap.width *   i) + (j + 1);  // Bottom right.
			int index3 = (Heightmap.width * (i + 1)) + j;    // Upper left.
			int index4 = (Heightmap.width * (i + 1)) + (j + 1);  // Upper right.
			
			Mesh.AddIndex(index1);
			Mesh.AddIndex(index3);
			Mesh.AddIndex(index2);

			Mesh.AddIndex(index2);
			Mesh.AddIndex(index3);
			Mesh.AddIndex(index4);
		}
	}

	Mesh.DoFinalMesh();
}

void Model::DefineTriangle()
{
	Mesh.AddVertex(-1.0f, -1.0f, 0.0f);  // Bottom left.

	Mesh.AddVertex(0.0f, 1.0f, 0.0f);  // Top middle.

	Mesh.AddVertex(1.0f, -1.0f, 0.0f);  // Bottom right.

	Mesh.AddIndex(0); // Bottom left.
	Mesh.AddIndex(1);  // Top middle.
	Mesh.AddIndex(2);  // Bottom right.
	Mesh.AddUV(0.0, 1.0);
	Mesh.AddUV(0.0, 0.0);
	Mesh.AddUV(1.0, 1.0);

	//All primitives must to generate the Final Mesh
	Mesh.DoFinalMesh();
}

void Model::DefineSquare()
{
	Mesh.AddVertex(-1.0f, -1.0f, 0.0f);  // Bottom Left.
	Mesh.AddUV(0.0, 1.0);
	Mesh.AddNormals(0.0, 0.0, -1.0);

	Mesh.AddVertex(-1.0f, 1.0f, 0.0f);  // Top Left.
	Mesh.AddUV(0.0, 0.0);
	Mesh.AddNormals(0.0, 0.0, -1.0);

	Mesh.AddVertex(1.0f, 1.0f, 0.0f);  // Top Right.
	Mesh.AddUV(1.0, 0.0);
	Mesh.AddNormals(0.0, 0.0, -1.0);

	Mesh.AddVertex(1.0f, -1.0f, 0.0f);  // Bottom Right.
	Mesh.AddUV(1.0, 1.0);
	Mesh.AddNormals(0.0, 0.0, -1.0);

	Mesh.AddIndex(0); // Bottom left.
	Mesh.AddIndex(1);  // Top left.
	Mesh.AddIndex(2);  // Bottom right.

	Mesh.AddIndex(0);  // Bottom left.
	Mesh.AddIndex(2);  // Top right.
	Mesh.AddIndex(3);  // Bottom right.
	Mesh.DoFinalMesh();
}

void Model::DefineCube(XMFLOAT3 size)
{
	// A box has six faces, each one pointing in a different direction.
	const int FaceCount = 6;

	static const XMVECTORF32 faceNormals[FaceCount] =
	{
		{ { { 0,  0,  1, 0 } } },
	{ { { 0,  0, -1, 0 } } },
	{ { { 1,  0,  0, 0 } } },
	{ { { -1,  0,  0, 0 } } },
	{ { { 0,  1,  0, 0 } } },
	{ { { 0, -1,  0, 0 } } },
	};

	static const XMVECTORF32 textureCoordinates[4] =
	{
		{ { { 1, 0, 0, 0 } } },
	{ { { 1, 1, 0, 0 } } },
	{ { { 0, 1, 0, 0 } } },
	{ { { 0, 0, 0, 0 } } },
	};

	XMVECTOR tsize = XMLoadFloat3(&size);
	tsize = XMVectorDivide(tsize, g_XMTwo);

	// Create each face in turn.
	for (int i = 0; i < FaceCount; i++)
	{
		XMVECTOR normal = faceNormals[i];

		// Get two vectors perpendicular both to the face normal and to each other.
		XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

		XMVECTOR side1 = XMVector3Cross(normal, basis);
		XMVECTOR side2 = XMVector3Cross(normal, side1);

		// Six indices (two triangles) per face.
		size_t vbase = Mesh.GetAllIndex().x;
		Mesh.AddIndex(vbase + 2);
		Mesh.AddIndex(vbase + 1);
		Mesh.AddIndex(vbase + 0);

		Mesh.AddIndex(vbase + 3);
		Mesh.AddIndex(vbase + 2);
		Mesh.AddIndex(vbase + 0);

		// Four vertices per face.
		XMVECTOR Result = (normal - side1 - side2) * tsize;
		XMFLOAT4 Res;
		XMStoreFloat4(&Res, Result); 	Mesh.AddVertex(Res);
		Mesh.AddUV(1, 0);	XMStoreFloat4(&Res, normal);	Mesh.AddNormals(Res);

		Result = (normal - side1 + side2) * tsize;		XMStoreFloat4(&Res, Result);	Mesh.AddVertex(Res);
		Mesh.AddUV(1, 1);	XMStoreFloat4(&Res, normal);	Mesh.AddNormals(Res);

		Result = (normal + side1 + side2) * tsize;		XMStoreFloat4(&Res, Result);	Mesh.AddVertex(Res);
		Mesh.AddUV(0, 1);	XMStoreFloat4(&Res, normal);	Mesh.AddNormals(Res);

		Result = (normal + side1 - side2) * tsize;		XMStoreFloat4(&Res, Result);	Mesh.AddVertex(Res);
		Mesh.AddUV(0, 0);	XMStoreFloat4(&Res, normal);	Mesh.AddNormals(Res);
		Mesh.DoFinalMesh();
	}
}

void Model::DefineGeoSphere(float diameter, size_t tessellation)
{
	// An undirected edge between two vertices, represented by a pair of indexes into a vertex array.
	// Becuse this edge is undirected, (a,b) is the same as (b,a).
	typedef std::pair<uint16_t, uint16_t> UndirectedEdge;

	// Makes an undirected edge. Rather than overloading comparison operators to give us the (a,b)==(b,a) property,
	// we'll just ensure that the larger of the two goes first. This'll simplify things greatly.
	auto makeUndirectedEdge = [](uint16_t a, uint16_t b)
	{
		return std::make_pair(std::max(a, b), std::min(a, b));
	};

	// Key: an edge
	// Value: the index of the vertex which lies midway between the two vertices pointed to by the key value
	// This map is used to avoid duplicating vertices when subdividing triangles along edges.
	typedef std::map<UndirectedEdge, uint16_t> EdgeSubdivisionMap;


	static const XMFLOAT3 OctahedronVertices[] =
	{
		// when looking down the negative z-axis (into the screen)
		XMFLOAT3(0,  1,  0), // 0 top
		XMFLOAT3(0,  0, -1), // 1 front
		XMFLOAT3(1,  0,  0), // 2 right
		XMFLOAT3(0,  0,  1), // 3 back
		XMFLOAT3(-1,  0,  0), // 4 left
		XMFLOAT3(0, -1,  0), // 5 bottom
	};
	static const uint16_t OctahedronIndices[] =
	{
		0, 1, 2, // top front-right face
		0, 2, 3, // top back-right face
		0, 3, 4, // top back-left face
		0, 4, 1, // top front-left face
		5, 1, 4, // bottom front-left face
		5, 4, 3, // bottom back-left face
		5, 3, 2, // bottom back-right face
		5, 2, 1, // bottom front-right face
	};

	const float radius = diameter / 2.0f;

	// Start with an octahedron; copy the data into the vertex/index collection.

	std::vector<XMFLOAT3> vertexPositions(std::begin(OctahedronVertices), std::end(OctahedronVertices));
	//Checar como implementar en Vertex
	Mesh.SpecialGetIndexVector()->insert(Mesh.SpecialGetIndexVector()->begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));
	Mesh.SpecialIndexVector();
	// We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
	// about to go on, these values aren't ever going to change because the vertices don't move around in the array.
	// We'll need these values later on to fix the singularities that show up at the poles.
	const uint16_t northPoleIndex = 0;
	const uint16_t southPoleIndex = 5;

	for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision)
	{
		assert(Mesh.GetIndex().size() % 3 == 0); // sanity

													   // We use this to keep track of which edges have already been subdivided.
		EdgeSubdivisionMap subdividedEdges;

		// The new index collection after subdivision.
		vector<unsigned long> newIndices;

		const size_t triangleCount = Mesh.GetIndex().size() / 3;
		for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
		{
			// For each edge on this triangle, create a new vertex in the middle of that edge.
			// The winding order of the triangles we output are the same as the winding order of the inputs.

			// Indices of the vertices making up this triangle
			uint16_t iv0 = Mesh.GetIndex()[iTriangle * 3 + 0];
			uint16_t iv1 = Mesh.GetIndex()[iTriangle * 3 + 1];
			uint16_t iv2 = Mesh.GetIndex()[iTriangle * 3 + 2];

			// Get the new vertices
			XMFLOAT3 v01; // vertex on the midpoint of v0 and v1
			XMFLOAT3 v12; // ditto v1 and v2
			XMFLOAT3 v20; // ditto v2 and v0
			uint16_t iv01; // index of v01
			uint16_t iv12; // index of v12
			uint16_t iv20; // index of v20

						   // Function that, when given the index of two vertices, creates a new vertex at the midpoint of those vertices.
			auto divideEdge = [&](uint16_t i0, uint16_t i1, XMFLOAT3& outVertex, uint16_t& outIndex)
			{
				const UndirectedEdge edge = makeUndirectedEdge(i0, i1);

				// Check to see if we've already generated this vertex
				auto it = subdividedEdges.find(edge);
				if (it != subdividedEdges.end())
				{
					// We've already generated this vertex before
					outIndex = it->second; // the index of this vertex
					outVertex = vertexPositions[outIndex]; // and the vertex itself
				}
				else
				{
					// Haven't generated this vertex before: so add it now

					// outVertex = (vertices[i0] + vertices[i1]) / 2
					XMStoreFloat3(
						&outVertex,
						XMVectorScale(
							XMVectorAdd(XMLoadFloat3(&vertexPositions[i0]), XMLoadFloat3(&vertexPositions[i1])),
							0.5f
						)
					);

					outIndex = static_cast<uint16_t>(vertexPositions.size());
					vertexPositions.push_back(outVertex);

					// Now add it to the map.
					subdividedEdges.insert(std::make_pair(edge, outIndex));
				}
			};

			// Add/get new vertices and their indices
			divideEdge(iv0, iv1, v01, iv01);
			divideEdge(iv1, iv2, v12, iv12);
			divideEdge(iv0, iv2, v20, iv20);

			// Add the new indices. We have four new triangles from our original one:
			//        v0
			//        o
			//       /a\
						            //  v20 o---o v01
//     /b\c/d\
            // v2 o---o---o v1
//       v12
			const uint16_t indicesToAdd[] =
			{
				iv0, iv01, iv20, // a
				iv20, iv12,  iv2, // b
				iv20, iv01, iv12, // c
				iv01,  iv1, iv12, // d
			};
			newIndices.insert(newIndices.end(), std::begin(indicesToAdd), std::end(indicesToAdd));
		}

		Mesh.GetIndex() = std::move(newIndices);
	}

	// Now that we've completed subdivision, fill in the final vertex collection
	Mesh.SpecialGetVertex().reserve(vertexPositions.size());
	for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it)
	{
		auto vertexValue = *it;

		auto normal = XMVector3Normalize(XMLoadFloat3(&vertexValue));
		auto pos = XMVectorScale(normal, radius);

		XMFLOAT3 normalFloat3;
		XMStoreFloat3(&normalFloat3, normal);

		// calculate texture coordinates for this vertex
		float longitude = atan2(normalFloat3.x, -normalFloat3.z);
		float latitude = acos(normalFloat3.y);

		float u = longitude / XM_2PI + 0.5f;
		float v = latitude / XM_PI;

		auto texcoord = XMVectorSet(1.0f - u, v, 0.0f, 0.0f);
		XMFLOAT4 Res;
		XMStoreFloat4(&Res, pos);
		Mesh.AddVertex(Res);
		XMStoreFloat4(&Res, normal);
		Mesh.AddNormals(Res);
		Mesh.AddUV(u, v);
	}

	// There are a couple of fixes to do. One is a texture coordinate wraparound fixup. At some point, there will be
	// a set of triangles somewhere in the mesh with texture coordinates such that the wraparound across 0.0/1.0
	// occurs across that triangle. Eg. when the left hand side of the triangle has a U coordinate of 0.98 and the
	// right hand side has a U coordinate of 0.0. The intent is that such a triangle should render with a U of 0.98 to
	// 1.0, not 0.98 to 0.0. If we don't do this fixup, there will be a visible seam across one side of the sphere.
	// 
	// Luckily this is relatively easy to fix. There is a straight edge which runs down the prime meridian of the
	// completed sphere. If you imagine the vertices along that edge, they circumscribe a semicircular arc starting at
	// y=1 and ending at y=-1, and sweeping across the range of z=0 to z=1. x stays zero. It's along this edge that we
	// need to duplicate our vertices - and provide the correct texture coordinates.
	size_t preFixupVertexCount = Mesh.SpecialGetVertex().size();
	for (unsigned long i = 0; i < preFixupVertexCount; ++i)
	{
		// This vertex is on the prime meridian if position.x and texcoord.u are both zero (allowing for small epsilon).
		bool isOnPrimeMeridian = XMVector2NearEqual(
			XMVectorSet(Mesh.SpecialGetVertex().at(i).x, Mesh.SpecialGetUV().at(i).x, 0.0f, 0.0f),
			XMVectorZero(),
			XMVectorSplatEpsilon());

		if (isOnPrimeMeridian)
		{
			size_t newIndex = Mesh.SpecialGetVertex().size(); // the index of this vertex that we're about to add

																	// copy this vertex, correct the texture coordinate, and add the vertex
			glm::vec3 pos = Mesh.GetVertex(i);
			glm::vec2 tex = Mesh.GetTexture(i);
			glm::vec3 norm = Mesh.GetNormal(i);
			tex.x = 1.0f;
			Mesh.AddVertex(pos);
			Mesh.AddUV(tex);
			Mesh.AddNormals(norm);

			// Now find all the triangles which contain this vertex and update them if necessary
			for (unsigned long j = 0; j < Mesh.GetIndex().size(); j += 3)
			{
				unsigned long* triIndex0 = &Mesh.GetIndex().at(j + 0);
				unsigned long* triIndex1 = &Mesh.GetIndex().at(j + 1);
				unsigned long* triIndex2 = &Mesh.GetIndex().at(j + 2);

				if (*triIndex0 == i)
				{
					// nothing; just keep going
				}
				else if (*triIndex1 == i)
				{
					std::swap(triIndex0, triIndex1); // swap the pointers (not the values)
				}
				else if (*triIndex2 == i)
				{
					std::swap(triIndex0, triIndex2); // swap the pointers (not the values)
				}
				else
				{
					// this triangle doesn't use the vertex we're interested in
					continue;
				}

				// If we got to this point then triIndex0 is the pointer to the index to the vertex we're looking at
				assert(*triIndex0 == i);
				assert(*triIndex1 != i && *triIndex2 != i); // assume no degenerate triangles

				glm::vec2 tex0 = Mesh.GetTexture(*triIndex0);
				glm::vec2 tex1 = Mesh.GetTexture(*triIndex1);
				glm::vec2 tex2 = Mesh.GetTexture(*triIndex2);

				/*const VertexPositionNormalTexture& v0 = vertices[*triIndex0];
				const VertexPositionNormalTexture& v1 = vertices[*triIndex1];
				const VertexPositionNormalTexture& v2 = vertices[*triIndex2];
				*/
				// check the other two vertices to see if we might need to fix this triangle

				if (abs(tex0.x - tex1.x) > 0.5f ||
					abs(tex0.x - tex2.x) > 0.5f)
				{
					// yep; replace the specified index to point to the new, corrected vertex
					*triIndex0 = static_cast<uint16_t>(newIndex);
				}
			}
		}
	}

	// And one last fix we need to do: the poles. A common use-case of a sphere mesh is to map a rectangular texture onto
	// it. If that happens, then the poles become singularities which map the entire top and bottom rows of the texture
	// onto a single point. In general there's no real way to do that right. But to match the behavior of non-geodesic
	// spheres, we need to duplicate the pole vertex for every triangle that uses it. This will introduce seams near the
	// poles, but reduce stretching.
	auto fixPole = [&](size_t poleIndex)
	{
		glm::vec3 poleVertexPos = Mesh.GetVertex(poleIndex);
		glm::vec2 poleVertexTex = Mesh.GetTexture(poleIndex);
		glm::vec3 poleVertexNorm = Mesh.GetNormal(poleIndex);

		bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex
		auto indices = Mesh.GetIndex();
		for (size_t i = 0; i < Mesh.IndexCount; i += 3)
		{
			// These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
			// entry in the index array which represents the pole index, and the other two pointers point to the other
			// two indices making up this triangle.
			unsigned long* pPoleIndex;
			unsigned long* pOtherIndex0;
			unsigned long* pOtherIndex1;
			if (indices[i + 0] == (unsigned long)poleIndex)
			{
				pPoleIndex = &indices[i + 0];
				pOtherIndex0 = &indices[i + 1];
				pOtherIndex1 = &indices[i + 2];
			}
			else if (indices[i + 1] == poleIndex)
			{
				pPoleIndex = &indices[i + 1];
				pOtherIndex0 = &indices[i + 2];
				pOtherIndex1 = &indices[i + 0];
			}
			else if (indices[i + 2] == poleIndex)
			{
				pPoleIndex = &indices[i + 2];
				pOtherIndex0 = &indices[i + 0];
				pOtherIndex1 = &indices[i + 1];
			}
			else
			{
				continue;
			}

			glm::vec2 otherVertexTex0 = Mesh.GetTexture(*pOtherIndex0);
			glm::vec2 otherVertexTex1 = Mesh.GetTexture(*pOtherIndex1);

			// Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
			glm::vec3 newPoleVertexPos = poleVertexPos;
			glm::vec2 newPoleVertexTex = poleVertexTex;
			glm::vec3 newPoleVertexNorm = poleVertexNorm;

			newPoleVertexTex.x = (otherVertexTex0.x + otherVertexTex1.x) / 2;
			newPoleVertexTex.y = poleVertexTex.y;

			if (!overwrittenPoleVertex)
			{
				Mesh.SetVertexTextureNormal(poleIndex, newPoleVertexPos, newPoleVertexTex, newPoleVertexNorm);
				overwrittenPoleVertex = true;
			}
			else
			{

				*pPoleIndex = static_cast<uint16_t>(Mesh.VertexCount);
				Mesh.AddVertex(newPoleVertexPos);
				Mesh.AddUV(newPoleVertexTex);
				Mesh.AddNormals(newPoleVertexNorm);
			}
		}
	};

	fixPole(northPoleIndex);
	fixPole(southPoleIndex);
}

void Model::DefineSphere(float diameter, size_t tessellation)
{
	float radius = diameter / 2;
	if (tessellation < 3)
		throw std::out_of_range("tesselation parameter out of range");

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	// Create rings of vertices at progressively higher latitudes.
	for (size_t i = 0; i <= verticalSegments; i++)
	{
		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
		float dy, dxz;

		XMScalarSinCos(&dy, &dxz, latitude);

		// Create a single ring of vertices at this latitude.
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * XM_2PI / horizontalSegments;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
			XMVECTOR result = normal * radius;
			XMFLOAT4 res;
			XMStoreFloat4(&res, result);
			Mesh.AddVertex(res);
			Mesh.AddUV(u, v);
			XMStoreFloat4(&res, normal);
			Mesh.AddNormals(res);
		}
	}

	// Fill the index buffer with triangles joining each pair of latitude rings.
	size_t stride = horizontalSegments + 1;

	for (size_t i = 0; i < (verticalSegments); i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			Mesh.AddIndex(i * stride + j);
			Mesh.AddIndex(nextI * stride + j);
			Mesh.AddIndex(i * stride + nextJ);

			Mesh.AddIndex(i * stride + nextJ);
			Mesh.AddIndex(nextI * stride + j);
			Mesh.AddIndex(nextI * stride + nextJ);
		}
	}
	Mesh.DoFinalMesh();
}

void Model::DefineBillboard()
{
	int indexCount = 0;
	//Define array vertex

	//First triangle
	Mesh.AddVertex(-1.0f*coordNegativo.x,1.0f*coordPositivo.y,0.0f);
	Mesh.AddUV(0.0f,0.0f);
	Mesh.AddNormals(0.0f,0.0f,-1.0f);
	Mesh.AddIndex(indexCount);
	indexCount++;

	Mesh.AddVertex(1.0f*coordPositivo.x, 1.0f*coordPositivo.y, 0.0f);
	Mesh.AddUV(1.0f, 0.0f);
	Mesh.AddNormals(0.0f, 0.0f, -1.0f);
	Mesh.AddIndex(indexCount);
	indexCount++;

	Mesh.AddVertex(-1.0f*coordNegativo.x, -1.0f*coordNegativo.y, 0.0f);
	Mesh.AddUV(0.0f, 1.0f);
	Mesh.AddNormals(0.0f, 0.0f, -1.0f);
	Mesh.AddIndex(indexCount);
	indexCount++;

	//Second triangle
	Mesh.AddVertex(-1.0f*coordNegativo.x, -1.0f*coordNegativo.y, 0.0f);
	Mesh.AddUV(0.0f, 1.0f);
	Mesh.AddNormals(0.0f, 0.0f, -1.0f);
	Mesh.AddIndex(indexCount);
	indexCount++;

	Mesh.AddVertex(1.0f*coordPositivo.x, 1.0f*coordPositivo.y, 0.0f);
	Mesh.AddUV(1.0f, 0.0f);
	Mesh.AddNormals(0.0f, 0.0f, -1.0f);
	Mesh.AddIndex(indexCount);
	indexCount++;

	Mesh.AddVertex(1.0f*coordPositivo.x, -1.0f*coordNegativo.y, 0.0f);
	Mesh.AddUV(1.0f, 1.0f);
	Mesh.AddNormals(0.0f, 0.0f, -1.0f);
	Mesh.AddIndex(indexCount);

	Mesh.DoFinalMesh();
	radio = 1;
}

void Model::DefineBitmap(vec4 rectBitmap, int widthScreen, int heightScreen){
	float left, right, top, bottom;
	int countIndex = 0;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((widthScreen / 2) * -1) + (float)rectBitmap.x;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)rectBitmap.z;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(heightScreen / 2) - (float)rectBitmap.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)rectBitmap.w;

	// First triangle.
	Mesh.AddVertex(XMFLOAT3(left, top, 0.0f));
	Mesh.AddUV(XMFLOAT2(0.0f, 0.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(right, bottom, 0.0f));
	Mesh.AddUV(XMFLOAT2(1.0f, 1.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(left, bottom, 0.0f));
	Mesh.AddUV(XMFLOAT2(0.0f, 1.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	// Second triangle.

	Mesh.AddVertex(XMFLOAT3(left, top, 0.0f));
	Mesh.AddUV(XMFLOAT2(0.0f, 0.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(right, top, 0.0f));
	Mesh.AddUV(XMFLOAT2(1.0f, 0.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(right, bottom, 0.0f));
	Mesh.AddUV(XMFLOAT2(1.0f, 1.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);

	Mesh.DoFinalMesh();
}

void Model::DefineWater() {

	m_water = new Water();
	
	if (!m_water)
		return;
	m_water->Initialize(specsDx->GetDevice(),specsDx->GetDeviceContext(),VertexBuffer,IndexBuffer,height_water,radius_water);

}

bool Model::UpdateBufferBitmap(vec4 rectBitmap, int widthScreen, int heightScreen)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex::VertexType * verticesPtr;
	HRESULT result;
	float left, right, top, bottom;
	int countIndex = 0;

	//ClearBuffer
	Mesh.ClearVertex();

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((widthScreen / 2) * -1) + (float)rectBitmap.x;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)rectBitmap.z;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(heightScreen / 2) - (float)rectBitmap.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)rectBitmap.w;

	// First triangle.
	Mesh.AddVertex(XMFLOAT3(left, top, 0.0f));
	Mesh.AddUV(XMFLOAT2(0.0f, 0.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f,0.0f,-1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(right, bottom, 0.0f));
	Mesh.AddUV(XMFLOAT2(1.0f, 1.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(left, bottom, 0.0f));
	Mesh.AddUV(XMFLOAT2(0.0f, 1.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	// Second triangle.

	Mesh.AddVertex(XMFLOAT3(left, top, 0.0f));
	Mesh.AddUV(XMFLOAT2(0.0f, 0.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(right, top, 0.0f));
	Mesh.AddUV(XMFLOAT2(1.0f, 0.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);
	countIndex++;

	Mesh.AddVertex(XMFLOAT3(right, bottom, 0.0f));
	Mesh.AddUV(XMFLOAT2(1.0f, 1.0f));
	Mesh.AddNormals(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Mesh.AddIndex(countIndex);

	Mesh.DoFinalMesh();
	
	result = deviceContext->Map(VertexBuffer, 0 , D3D11_MAP_WRITE_DISCARD, 0 , &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Vertex::VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)&Mesh.FinalMesh[0], (sizeof(Vertex::VertexType) * Mesh.VertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(VertexBuffer, 0);

	return true;
}

//void Model::HaveColition(bool tieneColision)
//{
//	colisiona = tieneColision;
//}
//
//bool Model::HaveMeshColition()
//{
//	return colisiona;
//}

//void Model::InitCol(string MeshCol)
//{
//	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
//	D3D11_SUBRESOURCE_DATA vertexData, indexData;
//	HRESULT result;
//
//	if (MeshCol == "Sphere")
//		DefineSphere(2, 8);
//	if (MeshCol == "Cube")
//		DefineCube(XMFLOAT3(1, 1, 1));
//
//	//Set up the description of the static vertex buffer
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;//Defualt buffer usage
//
//
//	vertexBufferDesc.ByteWidth = sizeof(Vertex::VertexType)* colition.VertexCount;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	vertexData.pSysMem = &colition.FinalMesh[0];
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	//Now create the vertex buffer
//	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexColBuffer);
//	if (FAILED(result))
//		ErrorFnc("No se pudo crear el buffer de vertices");
//
//	//Set up the description of the static index buffer
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(unsigned long)* colition.IndexCount;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//	indexBufferDesc.StructureByteStride = 0;
//
//	vector<unsigned long> indices;
//	indices = colition.GetIndex();
//
//
//	//Give the subresource structure a pointer to the index data
//	indexData.pSysMem = &indices[0];
//	indexData.SysMemPitch = 0;
//	indexData.SysMemSlicePitch = 0;
//
//	//Create the index buffer
//	result = device->CreateBuffer(&indexBufferDesc, &indexData, &IndexColBuffer);
//	if (FAILED(result))
//		ErrorFnc("No se pudo crear el buffer de indices");
//	
//}

//bool Model::ConstructBehaviour() {
//
//}

bool Model::Initialize(string NameOfFigure)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	// Values defualt
	m_previousWidth = -1;
	m_previousHeight = -1;
	m_previousPosX = -1;
	m_previousPosY = -1;
	m_screenWidth = 800;
	m_screenHeight = 600;
	m_water = NULL;

	if (NameOfFigure == "Triangle")
		DefineTriangle();
	if (NameOfFigure == "Square")
		DefineSquare();
	if (NameOfFigure == "Sphere")
		DefineSphere(2, 8);
	if (NameOfFigure == "Cube")
		DefineCube(XMFLOAT3(1, 1, 1));
	if (NameOfFigure == "Geosphere")
		DefineGeoSphere(2, 10);
	if (NameOfFigure == "Billboard")
		DefineBillboard();
	if (NameOfFigure == "SphereCollider")
		DefineSphere(2, 4);
	if(NameOfFigure == "Water")


	if (radio == 0)
		radio = 1;

	//Set up the description of the static vertex buffer
	if(!dynamicVertexBuffer)
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;//Defualt buffer usage
	else
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;//Dynamic buffer

	if (NameOfFigure != "")
		vertexBufferDesc.ByteWidth = sizeof(Vertex::VertexType)* Mesh.VertexCount;
	else
		vertexBufferDesc.ByteWidth = sizeof(Vertex::VertexType)* Mesh.IndexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (!dynamicVertexBuffer)
		vertexBufferDesc.CPUAccessFlags = 0;
	else
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &Mesh.FinalMesh[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer);
	if (FAILED(result))
		ErrorFnc("No se pudo crear el buffer de vertices");

	//Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* Mesh.IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	vector<unsigned long> indices;
	if (NameOfFigure != "")
		indices = Mesh.GetIndex();
	else
		indices = Mesh.IndexResult();

	//Give the subresource structure a pointer to the index data
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer);
	if (FAILED(result))
		ErrorFnc("No se pudo crear el buffer de indices");


	return true;
}

void Model::Draw()
{
	deviceContext->DrawIndexed(Mesh.IndexCount, 0, 0);
	return;
}

//void Model::DrawCol()
//{
//	deviceContext->DrawIndexed(colition.IndexCount, 0, 0);
//	return;
//}

void Model::BindMesh(D3D_PRIMITIVE_TOPOLOGY TOPOLOGY)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex::VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(TOPOLOGY);
}

//void Model::BindMeshCol(D3D_PRIMITIVE_TOPOLOGY TOPOLOGY)
//{
//	unsigned int stride;
//	unsigned int offset;
//
//	// Set vertex buffer stride and offset.
//	stride = sizeof(Vertex::VertexType);
//	offset = 0;
//
//	// Set the vertex buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetVertexBuffers(0, 1, &VertexColBuffer, &stride, &offset);
//
//	// Set the index buffer to active in the input assembler so it can be rendered.
//	deviceContext->IASetIndexBuffer(IndexColBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
//	deviceContext->IASetPrimitiveTopology(TOPOLOGY);
//}

Vertex Model::GetMesh()
{
	return Mesh;
}

bool Model::LoadModel(string path)
{
	string result = "";

	std::string fn = path;
	if (fn.substr(fn.find_last_of(".") + 1) != "obj")
		ErrorFnc("Compruebe la extension del archivo, debe ser .obj");

	ifstream fin;
	char input, in;
	vec3 FaceIndex;

	try {
		fin.open(path);

		if (fin.fail() == true)
			ErrorFnc("No se pudo abrir el archivo antes de cargarlo en ram");

		fin.get(input);
		while (!fin.eof())
		{
			/*if (input == 'm')
			{
			fin.get(input);
			if (input == 't')
			fin.get(input);
			if (input == 'l')
			fin.get(input);
			if (input == 'l')
			fin.get(input);
			if (input == 'i')
			fin.get(input);
			if (input == 'b')
			fin.get(input);
			MtlFile.assign((std::istreambuf_iterator<char>(fin)),
			(std::istreambuf_iterator<char>()));;
			}*/
			if (input == 'v')
			{
				fin.get(input);
				if (input == ' ')
				{
					vec3 temp;
					fin >> temp.x >> temp.y >> temp.z;
					temp.z = -1.0f *temp.z;
					Mesh.AddVertex(temp);
				}
				if (input == 't')
				{
					vec2 temp;
					fin >> temp.x >> temp.y;
					temp.y = 1.0f - temp.y;
					Mesh.AddUV(temp);
				}
				if (input == 'n')
				{
					vec3 temp;
					fin >> temp.x >> temp.y >> temp.z;
					temp.z = -1.0f * temp.z;
					Mesh.AddNormals(temp);
				}
			}

			if (input == 'f')
			{
				fin.get(input);
				if (input == ' ')
				{
					vec3 Vertice, Tex, Normal;
					fin >> Vertice.z >> in >> Tex.z >> in >> Normal.z
						>> Vertice.y >> in >> Tex.y >> in >> Normal.y
						>> Vertice.x >> in >> Tex.x >> in >> Normal.x;
					Mesh.AddTriangleFaces(Vertice, Tex, Normal);
				}
			}


			while (input != '\n')
			{
				fin.get(input);
			}
			fin.get(input);
		}

		//Solo para fines de debugeo en caso de errores... Este codigo puede decir cosas relevantes
		/*_RPT3(0, "First Vertex: %f,%f,%f\n", Mesh.GetFirstVertex().x, Mesh.GetFirstVertex().y, Mesh.GetFirstVertex().z);
		_RPT2(0, "First UVs: %f, %f\n", Mesh.GetFirstTexture().x, Mesh.GetFirstTexture().y);
		_RPT3(0, "First Normals: %f,%f,%f\n", Mesh.GetFirstNormal().x, Mesh.GetFirstNormal().y, Mesh.GetFirstNormal().z);

		_RPT3(0, "Last Vertex: %f,%f,%f\n", Mesh.GetLastVertex().x, Mesh.GetLastVertex().y, Mesh.GetLastVertex().z);
		_RPT2(0, "Last UVs: %f, %f\n", Mesh.GetFirstTexture().x, Mesh.GetFirstTexture().y);
		_RPT3(0, "Last Normals: %f,%f,%f\n", Mesh.GetLastNormal().x, Mesh.GetLastNormal().y, Mesh.GetLastNormal().z);
		*/
		//_RPT4(0, "#V: %f   #T: %f   #N: %f   #F: %f\n", Mesh.GetVertex(), Mesh.GetTexture(), Mesh.GetNormal(), Mesh.GetTriangleFN());
		//------------------------------------------------------------------------------------------

		fin.close();

		int i,j=0;
		for (i = 0; i < Mesh.VertexCount; i++)
		{
			//_RPT1(0, "i %d\n", i);
			FaceIndex.x = Mesh.GetTriangleFV(i).x-1;
			FaceIndex.y = Mesh.GetTriangleFT(i).x-1;
			FaceIndex.z = Mesh.GetTriangleFN(i).x-1;
			Mesh.DoFinalMeshFromTriangles(Mesh.GetVertex(FaceIndex.x), Mesh.GetTexture(FaceIndex.y), Mesh.GetNormal(FaceIndex.z),j);
			j++;

			FaceIndex.x = Mesh.GetTriangleFV(i).y-1;
			FaceIndex.y = Mesh.GetTriangleFT(i).y-1;
			FaceIndex.z = Mesh.GetTriangleFN(i).y-1;
			Mesh.DoFinalMeshFromTriangles(Mesh.GetVertex(FaceIndex.x), Mesh.GetTexture(FaceIndex.y), Mesh.GetNormal(FaceIndex.z),j);
			j++;

			FaceIndex.x = Mesh.GetTriangleFV(i).z-1;
			FaceIndex.y = Mesh.GetTriangleFT(i).z-1;
			FaceIndex.z = Mesh.GetTriangleFN(i).z-1;
			Mesh.DoFinalMeshFromTriangles(Mesh.GetVertex(FaceIndex.x), Mesh.GetTexture(FaceIndex.y), Mesh.GetNormal(FaceIndex.z),j);
			j++;

			//Buscar X maxima y minima - Y maxima y minima - Z maxima y minima
			if (Max.x <= Mesh.GetVertex(i).x)
				Max.x = Mesh.GetVertex(i).x;
			else if (Min.x >= Mesh.GetVertex(i).x)
				Min.x = Mesh.GetVertex(i).x;
			if (Max.y <= Mesh.GetVertex(i).y)
				Max.y = Mesh.GetVertex(i).y;
			else if (Min.y >= Mesh.GetVertex(i).y)
				Min.y = Mesh.GetVertex(i).y;
			if (Max.z <= Mesh.GetVertex(i).z)
				Max.z = Mesh.GetVertex(i).z;
			else if (Min.z >= Mesh.GetVertex(i).z)
				Min.z = Mesh.GetVertex(i).z;
		}

		//Definir el radio del modelo
		double radioMax = Max.x;
		if (radioMax < Max.y)
			radioMax = Max.y;
		if (radioMax < Max.z)
			radioMax = Max.z;
		double radioMin = Min.x;
		if (radioMin > Max.y)
			radioMin = Max.y;
		if (radioMin > Max.z)
			radioMin = Max.z;
		if (radioMax > (-1)*radioMin)
			radio = radioMax;
		else
			radio = radioMin;

		if (radio < 0)
			radio *= -1;

	}
	catch (std::exception& e)
	{
		result = string("\nError exception: ") + e.what();
	}


	if (result != "")
		ErrorFnc("no se logro cargar en ram.");

	/*_RPT1(0, "Vertices: %f\n", Mesh.GetAllIndex().x);
	_RPT1(0, "UVs: %f\n", Mesh.GetAllIndex().y);
	_RPT1(0, "Normals: %f\n", Mesh.GetAllIndex().z);
	_RPT1(0, "Faces: %f\n", Mesh.GetAllIndex().w);*/
	return true;
}

void Model::ShutdownModel()
{
	if (IndexBuffer)
	{
		IndexBuffer->Release();
		IndexBuffer = 0;
	}

	if (VertexBuffer)
	{
		VertexBuffer->Release();
		VertexBuffer = 0;
	}

	Mesh.ClearVertex();

	if (m_water)
	{
		m_water->Shutdown();
		delete m_water;
		m_water = NULL;
	}

	return;
}

//Method Bitmap
void Model::SetRect(vec4 rectBitmap)
{
	CenterX = rectBitmap.x;
	CenterY = rectBitmap.y;
	m_bitmapWidth = rectBitmap.z;
	m_bitmapHeight = rectBitmap.w;

	PosX = rectBitmap.x + m_screenWidth * 0.5f - m_bitmapWidth * 0.5f;
	PosY = m_screenHeight * 0.5f - m_bitmapHeight * 0.5f - rectBitmap.y;
	return;
}

//Method Bitmap
void Model::SetPosition(vec2 position)
{
	CenterX = position.x;
	CenterY = position.y;
	PosX = position.x + m_screenWidth * 0.5f - m_bitmapWidth * 0.5f;
	PosY = m_screenHeight * 0.5f - m_bitmapHeight * 0.5f - position.y;
	return;
}

//Method Bitmap
void Model::SetSize(vec2 size)
{
	m_bitmapWidth = size.x;
	m_bitmapHeight = size.y;

	PosX = PosX + m_screenWidth * 0.5f - m_bitmapWidth * 0.5f;
	PosY = m_screenHeight * 0.5f - m_bitmapHeight * 0.5f - PosY;
}

//Update Bitmap
void Model::UpdateBitmap()
{
	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if ((PosX == m_previousPosX) && (PosY == m_previousPosY) && (m_bitmapWidth == m_previousWidth) && (m_bitmapHeight == m_previousHeight))
	{
		return;
	}

	// If it has changed then update the position it is being rendered to.
	m_previousPosX = PosX;
	m_previousPosY = PosY;
	m_previousWidth = m_bitmapWidth;
	m_previousHeight = m_bitmapHeight;

	UpdateBufferBitmap(vec4(PosX, PosY, m_bitmapWidth, m_bitmapHeight), m_screenWidth, m_screenHeight);
}

double Model::GetRadio()
{
	return radio;
}