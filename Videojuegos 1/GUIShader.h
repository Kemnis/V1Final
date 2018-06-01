#pragma once
#include "stdafx.h"

using namespace DirectX;
using namespace std;

class GUIShader : public Shader
{
private:

public:
	GUIShader(std::string, std::string);

	bool Initialize(const std::string& vsSource, const std::string& psSource) override;
};