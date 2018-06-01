#ifndef _SkydomeShader_H_
#define _SkydomeShader_H_
#include "Shader.h"

using namespace DirectX;
using namespace std;
class SkydomeShader : public Shader
{
private:
	
public:
	SkydomeShader(std::string, std::string);
	SkydomeShader(std::string, std::string, int WithLight);

	bool Initialize(const std::string& vsSource, const std::string& psSource) override;
};

#endif