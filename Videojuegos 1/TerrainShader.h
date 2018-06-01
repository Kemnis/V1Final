#pragma once
#include "Shader.h"

using namespace DirectX;
using namespace std;
class TerrainShader : public Shader
{
private:

public:
	TerrainShader(std::string, std::string);

	bool Initialize(const std::string& vsSource, const std::string& psSource) override;
};