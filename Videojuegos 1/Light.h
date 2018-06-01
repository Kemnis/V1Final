#pragma once
#include "glm/glm.hpp"
using namespace glm;
class Light {
public:
	string Name;
	vec4 Ambient;
	vec4 Diffuse;
	vec3 Direction;
	vec4 color;
	float intensity;
};