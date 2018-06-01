#pragma once


#define MATRIX_BUFFER_ID			0
#define CUSTOM_BUFFER_1				1
#define CUSTOM_BUFFER_2				2
#define CUSTOM_BUFFER_3				3
#define CUSTOM_BUFFER_4				4
#define CUSTOM_BUFFER_5				5
#define CUSTOM_BUFFER_6				6
#define CUSTOM_BUFFER_7				7
#define CUSTOM_BUFFER_8				8
#define CUSTOM_BUFFER_9				9

enum class ConstantBufferLocation {
	VertexShader = 1,
	PixelShader = 2
};

class ConstantBuffer : public DxComponent<ConstantBuffer> {
private:
	unsigned int structSize;

	HRESULT CreateBuffer(unsigned int bufferSize);

	bool Initialize();
public:
	unsigned int id;
	ConstantBufferLocation location;
	ConstantBuffer(unsigned int id, ConstantBufferLocation location, unsigned int bufferSize);
	~ConstantBuffer();

	void Update(const void* source);

	ID3D11Buffer* buffer;
};