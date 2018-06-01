#include "stdafx.h"
#include "Transforms.h"
Transforms::Transforms()
{
	this->esc = { 1,1,1 };
	this->rot = { 0, 0, 0 };
	this->tran = { 0, 0, 0 };
	this->gravity = 0.0;
	this->m_speedDirection = 0.0;
}

Transforms::Transforms(vec3 esc, vec3 rot, vec3 tran)
{
	this->esc = esc;
	this->rot = rot;
	this->tran = tran;
}

Transforms::~Transforms() {}

void Transforms::Scale(vec3 Scale)
{
	this->esc = Scale;
}

void Transforms::Rotate(vec3 Rotation)
{
	Rotation.x = Rotation.x*0.0174533f;
	Rotation.y = Rotation.y*0.0174533f;
	Rotation.z = Rotation.z*0.0174533f;
	this->rot = this->rot + Rotation;
}

void Transforms::Translate(vec3 Translation)
{
	this->tran += Translation;
}

void Transforms::Identity()
{
	this->tran = vec3(0, 0, 0);
	this->rot = vec3(0, 0, 0);
	this->esc = vec3(0, 0, 0);
}

void Transforms::SetScale(vec3 Scale)
{
	this->esc = Scale;
}

void Transforms::SetRotation(vec3 Rotation)
{
	Rotation.x = Rotation.x*0.0174533f;
	Rotation.y = Rotation.y*0.0174533f;
	Rotation.z = Rotation.z*0.0174533f;
	this->rot = Rotation;
}

void Transforms::SetTranslation(vec3 Translation)
{
	this->tran = Translation;
}

void Transforms::SetRadio(double radio)
{
	this->radio = radio;
}
double Transforms::GetRadio()
{
	return radio;
}

vec3 Transforms::GetScale()
{
	return this->esc;
}

vec3 Transforms::GetRotation()
{
	return this->rot;
}

vec3 Transforms::GetTranslation()
{
	return this->tran;
}

void Transforms::SetTransforms(vec3 Translation, vec3 Rotation, vec3 Scale)
{
	this->tran = Translation;
	this->rot = Rotation;
	this->esc = Scale;
	ToMatrix();
}

void Transforms::ToMatrix(vec3 Scale, vec3 Rotation, vec3 Translation)
{
	/*this->rot.x = this->rot.x*0.0174533f;
	this->rot.y = this->rot.y*0.0174533f;
	this->rot.z = this->rot.z*0.0174533f;*/
	TransformMatrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z)*XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z)* XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
}

XMMATRIX Transforms::ToMatrix()
{
	TransformMatrix = XMMatrixScaling(this->esc.x, this->esc.y, this->esc.z)* XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->tran.x, this->tran.y, this->tran.z);
	return TransformMatrix;
}

XMMATRIX Transforms::DoMatrix(vec3 pos, vec3 rot, vec3 esc)
{
	return XMMatrixScaling(esc.x, esc.y, esc.z)* XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);
}

XMMATRIX Transforms::GetMatrix()
{
	return TransformMatrix;
}

void Transforms::LookCamera(vec3 position)
{
	this->rot.y = (float)atan2(this->tran.x - position.x, this->tran.z - position.z) * (180.0 / XM_PI)*0.0174532925f;
}

void Transforms::ProccesParabolicShot(double deltaTime)
{
	m_speedDirection = deltaTime * 0.03f;
	gravity += deltaTime * 0.000098f;

	tran = vec3(tran.x + (n.x*m_speedDirection),tran.y + (n.y*m_speedDirection),tran.z + (n.z*m_speedDirection));
	tran.y -= gravity;
}

void Transforms::Shot(vec3 LookAt, vec3 LookRotation)
{
	m_speedDirection = 0.0;
	gravity = 0.0;

	rot = LookAt;

	//XMVECTOR direction = XMStoreFloat3();
}

void Transforms::SetNormal(vec3 normal) {
	n = normal;
}

vec3 Transforms::GetNormal() {
	return n;
}