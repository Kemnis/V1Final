#pragma once
#ifndef _Transforms_H_
#define _Transforms_H_
#include "glm/glm.hpp"
using namespace glm;
#include <directxmath.h>
using namespace DirectX;
class Transforms
{
public:
	Transforms();
	Transforms(vec3 esc, vec3 rot, vec3 tran);
	~Transforms();
	void Translate(vec3 Translation);
	void Rotate(vec3 Rotation);
	void Scale(vec3 Scale);
	void Identity();

	void SetScale(vec3 Scale);
	void SetRotation(vec3 Rotation);
	void SetTranslation(vec3 Translation);
	void SetRadio(double radio);
	void SetNormal(vec3 normal);
	double GetRadio();
	void LookCamera(vec3 position);
	vec3 GetScale();
	vec3 GetRotation();
	vec3 GetTranslation();
	vec3 GetNormal();
	void SetTransforms(vec3 Translation, vec3 Rotation, vec3 Scale);
	void ToMatrix(vec3 Scale, vec3 Rotation, vec3 Translation);
	void ProccesParabolicShot(double deltaTime);
	void Shot(vec3 LookAt,vec3 LookRotation);
	XMMATRIX ToMatrix();
	XMMATRIX GetMatrix();
	XMMATRIX DoMatrix(vec3 pos, vec3 rot, vec3 esc);
private:
	vec3 rot, tran, esc,n;
	double radio;
	double gravity;
	double m_speedDirection;
	XMMATRIX TransformMatrix;
};
#endif