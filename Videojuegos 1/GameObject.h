#pragma once
#ifndef _GameObject_H_
#define _GameObject_H_
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "BasicShader.h"
#include "MaterialShader.h"
#include "Camera3D.h"
#include "Material.h"
#include "Light.h"
#include "Colision.h"


class GameObject
{
public:
	GameObject(string Name);
	~GameObject();
	
	string GetName();
	bool AssignModel(Model* model);
	bool AssignShader(Shader* shader);
	bool AssignMaterial(Material* mat);
	bool AssignLight(Light* light);
	bool AddTexture(Texture* texture);
	void SetPosBitmap(vec2 Position);
	void SetEscBitmap(vec2 Escale);
	void SetPosBitmap(float PosX, float PosY);
	void SetEscBitmap(float EscX, float EscY);
	Model* GetModel();
	Light* GetLight();
	//Texture* GetTexture();
	bool ExistShader();
	Material* GetMaterial();
	void Draw(XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	void Draw(XMMATRIX world, XMMATRIX view, XMMATRIX projection, XMMATRIX reflection);
	void DrawPatrol(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void Collider(GameObject*gameObject, double deltaTime, Camera3D *SceneCamera);
	void PrecalculatePositions();
	void Respawn();
	void IsPatrol(vector<vec3> PosicionesPosibles);
	Shader* GetShader();
	void Shutdown();

	void AnimationDamage(double deltaTime, double time);
	vec3 RespawnPlayer(Model*Terrain);
	Colision * GetColision();
	void Collider(double deltaTime);

	Transforms* Transform;

	bool DamageColision();


	bool FilckerBoolean;
	int CountLifes = 0;
	int CountCorazon = 3;
	int CountCorazonOld = 3;


protected:

	string Name, Status;
	Model* Modelo;//Mesh
	std::vector<Texture*> Tex;
	Shader* shader;
	Material* material;
	Light* light;
	vector<vec3> PosPos;
	vector<XMMATRIX> multipleworlds;
	bool ispatrol = false;
	vec3 PosAnt;
	//EnemyBehaviour MyBehaviour();

	Colision * colision;
	int CountFlicker;
	int TimeFlicker = 3;
	float Time = 2;

	bool ColsionAnimation;
	bool Damage = false;

	//Variables Water Plane
	XMFLOAT3 cameraPosition;
	XMFLOAT2 normalMapTiling;
	float waterTranslation;
	float reflectRefractScale;
	XMFLOAT4 refractionTint;
	//Hardcore light
	XMFLOAT3 lightDirection;
	float specularShininess;

	float colorTextureBrightness;
	XMFLOAT4 clipPlane;
private:

	
};

#endif