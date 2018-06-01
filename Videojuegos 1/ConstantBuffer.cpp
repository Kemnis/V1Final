#include "stdafx.h"

ConstantBuffer::ConstantBuffer(unsigned int id, ConstantBufferLocation location, unsigned int bufferSize){
	this->id = id;
	this->location = location;
	this->structSize = bufferSize;
	this->Initialize();
}

ConstantBuffer::~ConstantBuffer() {
	/*if (this->buffer) {
		this->buffer->Release();
	}*/
}

bool ConstantBuffer::Initialize() {
	HRESULT result = this->CreateBuffer(structSize);
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

HRESULT ConstantBuffer::CreateBuffer(unsigned int bufferSize) {
	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = bufferSize;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	return device->CreateBuffer(&matrixBufferDesc, NULL, &buffer);
}

void ConstantBuffer::Update(const void* source) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(this->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	void* dataPtr = mappedResource.pData;
	memcpy(dataPtr, source, this->structSize);
	deviceContext->Unmap(this->buffer, 0);
}
