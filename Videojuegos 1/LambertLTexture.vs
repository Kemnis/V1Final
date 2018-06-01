cbuffer MatrixBuffer : register(b0)
{
	matrix projectionViewWorldMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType VertexProgram(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;
	output.position = mul(input.position, projectionViewWorldMatrix);
	output.tex = input.tex;
	output.normal = mul(input.normal,(float3x3)projectionViewWorldMatrix);
	output.normal = normalize(output.normal);

    return output;
}