////////////////////////////////////////////////////////////////////////////////
// Filename: 
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix projectionViewWorldMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VertexProgram(VertexInputType input)
{
	PixelInputType output;


	input.position.w = 1.0f;
	output.position = mul(input.position, projectionViewWorldMatrix);
	output.tex = input.tex;

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}