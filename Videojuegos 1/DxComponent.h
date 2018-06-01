#pragma once
#ifndef _DxComponent_H_
#define _DxComponent_H_
template<typename T>
class DxComponent
{
public:
	DxComponent() {
		specsDx = SpecsDx::GetInstance();
		device = specsDx->GetDevice();
		deviceContext = specsDx->GetDeviceContext();
	}
	~DxComponent() {}
	SpecsDx * specsDx;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
protected:
private:
	
};
#endif