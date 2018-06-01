cbuffer MatrixBuffer
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
};

PixelInputType VertexProgram(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;
	output.position = mul(input.position, projectionViewWorldMatrix);
    
    return output;
}