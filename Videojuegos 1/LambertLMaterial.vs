cbuffer MatrixBuffer : register(b0)
{
	matrix projectionViewWorldMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 normal : NORMAL;
};

PixelInputType VertexProgram(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

	float height = input.position.y / 5;
	float4 vColor = float4(height, height, height, 1.0);

	output.position = mul(input.position, projectionViewWorldMatrix);
	//output.vertexColor = vColor;
	output.normal = vColor;
    return output;
}