#include "Mesh3D.h"
#include "stdafx.h"
#include <algorithm> 

Mesh3D::Mesh3D()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

Mesh3D::~Mesh3D()
{
}

void Mesh3D::DefineTriangle()
{
	MeshVertex.AddVertex(-1.0f, -1.0f, 0.0f);  // Bottom left.

	MeshVertex.AddVertex(0.0f, 1.0f, 0.0f);  // Top middle.

	MeshVertex.AddVertex(1.0f, -1.0f, 0.0f);  // Bottom right.

	MeshVertex.AddIndex(0); // Bottom left.
	MeshVertex.AddIndex(1);  // Top middle.
	MeshVertex.AddIndex(2);  // Bottom right.
}

void Mesh3D::DefineSquare()
{
	MeshVertex.AddVertex(-1.0f, -1.0f, 0.0f);  // Top left.

	MeshVertex.AddVertex(-1.0f, 1.0f, 0.0f);  // Top right.

	MeshVertex.AddVertex(1.0f, 1.0f, 0.0f);  // Bottom right.

	MeshVertex.AddVertex(1.0f, -1.0f, 0.0f);  // Bottom left.

	MeshVertex.AddIndex(0); // Bottom left.
	MeshVertex.AddIndex(1);  // Top middle.
	MeshVertex.AddIndex(2);  // Bottom right.

	MeshVertex.AddIndex(0);  // Bottom left.
	MeshVertex.AddIndex(2);  // Top right.
	MeshVertex.AddIndex(3);  // Bottom right.
}
void Mesh3D::DefineCube(XMFLOAT3 size)
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
		size_t vbase = MeshVertex.GetAllIndex().x;
		MeshVertex.AddIndex(vbase + 0);
		MeshVertex.AddIndex(vbase + 1);
		MeshVertex.AddIndex(vbase + 2);

		MeshVertex.AddIndex(vbase + 0);
		MeshVertex.AddIndex(vbase + 2);
		MeshVertex.AddIndex(vbase + 3);

		// Four vertices per face.
		XMVECTOR Result = (normal - side1 - side2) * tsize;
		XMFLOAT4 Res;
		XMStoreFloat4(&Res, Result); 	MeshVertex.AddVertex(Res);
		MeshVertex.AddUV(1,0);	XMStoreFloat4(&Res, normal);	MeshVertex.AddNormals(Res);

		Result = (normal - side1 + side2) * tsize;		XMStoreFloat4(&Res, Result);	MeshVertex.AddVertex(Res);
		MeshVertex.AddUV(1, 1);	XMStoreFloat4(&Res, normal);	MeshVertex.AddNormals(Res);

		Result = (normal + side1 + side2) * tsize;		XMStoreFloat4(&Res, Result);	MeshVertex.AddVertex(Res);
		MeshVertex.AddUV(0, 1);	XMStoreFloat4(&Res, normal);	MeshVertex.AddNormals(Res);
		
		Result = (normal + side1 - side2) * tsize;		XMStoreFloat4(&Res, Result);	MeshVertex.AddVertex(Res);
		MeshVertex.AddUV(0, 0);	XMStoreFloat4(&Res, normal);	MeshVertex.AddNormals(Res);
	}
}

void Mesh3D::DefineSphere(float diameter,size_t tessellation)
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
			XMVECTOR result = normal*radius;
			XMFLOAT4 res;
			XMStoreFloat4(&res, result);
			MeshVertex.AddVertex(res);
			MeshVertex.AddUV(u, v);
			XMStoreFloat4(&res, normal);
			MeshVertex.AddNormals(res);
		}
	}

	// Fill the index buffer with triangles joining each pair of latitude rings.
	size_t stride = horizontalSegments + 1;

	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;
			
			MeshVertex.AddIndex(i * stride + j);
			MeshVertex.AddIndex(nextI * stride + j);
			MeshVertex.AddIndex(i * stride + nextJ);

			MeshVertex.AddIndex(i * stride + nextJ);
			MeshVertex.AddIndex(nextI * stride + j);
			MeshVertex.AddIndex(nextI * stride + nextJ);
		}
	}
}

void Mesh3D::DefineGeoSphere(float diameter, size_t tessellation)
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
	MeshVertex.SpecialGetIndexVector()->insert(MeshVertex.SpecialGetIndexVector()->begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));
	MeshVertex.SpecialIndexVector();
	// We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
	// about to go on, these values aren't ever going to change because the vertices don't move around in the array.
	// We'll need these values later on to fix the singularities that show up at the poles.
	const uint16_t northPoleIndex = 0;
	const uint16_t southPoleIndex = 5;

	for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision)
	{
		assert(MeshVertex.GetIndex().size() % 3 == 0); // sanity

										 // We use this to keep track of which edges have already been subdivided.
		EdgeSubdivisionMap subdividedEdges;

		// The new index collection after subdivision.
		vector<unsigned long> newIndices;

		const size_t triangleCount = MeshVertex.GetIndex().size() / 3;
		for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
		{
			// For each edge on this triangle, create a new vertex in the middle of that edge.
			// The winding order of the triangles we output are the same as the winding order of the inputs.

			// Indices of the vertices making up this triangle
			uint16_t iv0 = MeshVertex.GetIndex()[iTriangle * 3 + 0];
			uint16_t iv1 = MeshVertex.GetIndex()[iTriangle * 3 + 1];
			uint16_t iv2 = MeshVertex.GetIndex()[iTriangle * 3 + 2];

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

		MeshVertex.GetIndex() = std::move(newIndices);
	}

	// Now that we've completed subdivision, fill in the final vertex collection
	MeshVertex.SpecialGetVertex().reserve(vertexPositions.size());
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
		MeshVertex.AddVertex(Res);
		XMStoreFloat4(&Res, normal);
		MeshVertex.AddNormals(Res);
		MeshVertex.AddUV(u,v);
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
	size_t preFixupVertexCount = MeshVertex.SpecialGetVertex().size();
	for (unsigned long i = 0; i < preFixupVertexCount; ++i)
	{
		// This vertex is on the prime meridian if position.x and texcoord.u are both zero (allowing for small epsilon).
		bool isOnPrimeMeridian = XMVector2NearEqual(
			XMVectorSet(MeshVertex.SpecialGetVertex().at(i).x, MeshVertex.SpecialGetUV().at(i).x, 0.0f, 0.0f),
			XMVectorZero(),
			XMVectorSplatEpsilon());

		if (isOnPrimeMeridian)
		{
			size_t newIndex = MeshVertex.SpecialGetVertex().size(); // the index of this vertex that we're about to add

			// copy this vertex, correct the texture coordinate, and add the vertex
			glm::vec3 pos = MeshVertex.GetVertex(i);
			glm::vec2 tex = MeshVertex.GetTexture(i);
			glm::vec3 norm = MeshVertex.GetNormal(i);
			tex.x = 1.0f;
			MeshVertex.AddVertex(pos);
			MeshVertex.AddUV(tex);
			MeshVertex.AddNormals(norm);

			// Now find all the triangles which contain this vertex and update them if necessary
			for (unsigned long j = 0; j < MeshVertex.GetIndex().size(); j += 3)
			{
				unsigned long* triIndex0 = &MeshVertex.GetIndex().at(j + 0);
				unsigned long* triIndex1 = &MeshVertex.GetIndex().at(j + 1);
				unsigned long* triIndex2 = &MeshVertex.GetIndex().at(j + 2);

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

				glm::vec2 tex0 = MeshVertex.GetTexture(*triIndex0);
				glm::vec2 tex1 = MeshVertex.GetTexture(*triIndex1);
				glm::vec2 tex2 = MeshVertex.GetTexture(*triIndex2);

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
		glm::vec3 poleVertexPos = MeshVertex.GetVertex(poleIndex);
		glm::vec2 poleVertexTex = MeshVertex.GetTexture(poleIndex);
		glm::vec3 poleVertexNorm = MeshVertex.GetNormal(poleIndex);

		bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex
		auto indices = MeshVertex.GetIndex();
		for (size_t i = 0; i < MeshVertex.IndexCount; i += 3)
		{
			// These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
			// entry in the index array which represents the pole index, and the other two pointers point to the other
			// two indices making up this triangle.
			unsigned long* pPoleIndex;
			unsigned long* pOtherIndex0;
			unsigned long* pOtherIndex1;
			if (indices[i + 0] == (unsigned long) poleIndex)
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

			glm::vec2 otherVertexTex0 = MeshVertex.GetTexture(*pOtherIndex0);
			glm::vec2 otherVertexTex1 = MeshVertex.GetTexture(*pOtherIndex1);

			// Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
			glm::vec3 newPoleVertexPos = poleVertexPos;
			glm::vec2 newPoleVertexTex = poleVertexTex;
			glm::vec3 newPoleVertexNorm = poleVertexNorm;

			newPoleVertexTex.x = (otherVertexTex0.x + otherVertexTex1.x) / 2;
			newPoleVertexTex.y = poleVertexTex.y;

			if (!overwrittenPoleVertex)
			{
				MeshVertex.SetVertexTextureNormal(poleIndex, newPoleVertexPos, newPoleVertexTex, newPoleVertexNorm);
				overwrittenPoleVertex = true;
			}
			else
			{

				*pPoleIndex = static_cast<uint16_t>(MeshVertex.VertexCount);
				MeshVertex.AddVertex(newPoleVertexPos);
				MeshVertex.AddUV(newPoleVertexTex);
				MeshVertex.AddNormals(newPoleVertexNorm);
			}
		}
	};

	fixPole(northPoleIndex);
	fixPole(southPoleIndex);
}

bool Mesh3D::Initialize(string NameOfFigure)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	if (NameOfFigure == "Triangle")
		DefineTriangle();
	if (NameOfFigure == "Square")
		DefineSquare();
	if (NameOfFigure == "Sphere")
		DefineSphere(2, 5);
	if (NameOfFigure == "Cube")
		DefineCube(XMFLOAT3(1, 1, 1));
	if (NameOfFigure == "Geosphere")
		DefineGeoSphere(2, 10);

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex::VertexType) * MeshVertex.VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vector<Vertex::VertexType> vertices = MeshVertex.VertexResult();
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * MeshVertex.IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	vector<unsigned long> indices = MeshVertex.GetIndex();
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Mesh3D::BindMesh()
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex::VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Mesh3D::Draw()
{
	deviceContext->DrawIndexed(MeshVertex.IndexCount, 0, 0);
	return;
}

void Mesh3D::Shutdown()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}