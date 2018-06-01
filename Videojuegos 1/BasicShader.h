#pragma once
#include "Shader.h"

using namespace DirectX;
using namespace std;

class BasicShader : public Shader
{
public:
	
	BasicShader(std::string, std::string);
	BasicShader(std::string, std::string, int WithLight);
	bool Initialize (const std::string& vsSource, const std::string& psSource) override;
};

