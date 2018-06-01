#include "stdafx.h"

GameObject::GameObject(string name)
{
	Transform = new Transforms();
	colision = new Colision();
	Name = name;
}

GameObject::~GameObject()
{
}

string GameObject::GetName()
{
	return Name;
}

bool GameObject::AssignModel(Model* model)
{
	Modelo = model;

	Transform->SetRadio(model->GetRadio());
	
	return true;
}

bool GameObject::AssignLight(Light* light)
{
	this->light = light;
	return true;
}

bool GameObject::AssignShader(Shader* shader) {
	this->shader = shader;
	return true;
}

bool GameObject::AssignMaterial(Material * mat)
{
	material = mat;
	return true;
}

bool GameObject::AddTexture(Texture * texture)
{
	Tex.push_back(texture);
	return true;
}

void GameObject::Shutdown()
{

	if (Transform) {
		delete Transform;
		Transform = 0;
	}

	if (colision) {
		delete colision;
		colision = 0;
	}



}

void GameObject::SetPosBitmap(vec2 Position)
{
	Modelo->SetPosition(Position);
}

void GameObject::SetEscBitmap(vec2 Escale)
{
	Modelo->SetSize(Escale);
}

void GameObject::SetPosBitmap(float PosX,float PosY)
{
	Modelo->SetPosition(vec2(PosX,PosY));
}

void GameObject::SetEscBitmap(float EscX,float EscY)
{
	Modelo->SetSize(vec2(EscX,EscY));
}

Model* GameObject::GetModel()
{
	return Modelo;
}

Shader* GameObject::GetShader()
{
	return shader;
}

bool GameObject::ExistShader()
{
	return (shader != nullptr);
}

Material* GameObject::GetMaterial()
{
	return material;
}

Light* GameObject::GetLight()
{
	return light;
}

void GameObject::IsPatrol(vector<vec3> PosicionesPosibles)
{
	PosPos = PosicionesPosibles;
	ispatrol = true;
	PrecalculatePositions();
}

void GameObject::PrecalculatePositions()
{
	if (ispatrol == true)
	{
		for (auto it = PosPos.begin(); it != PosPos.end(); it++)
		{
			multipleworlds.push_back(Transform->DoMatrix(vec3(it->x, it->y, it->z),vec3(0,0,0),vec3(1,1,1)));
		}
	}
}

void GameObject::DrawPatrol(XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (ispatrol == true)
	{
		for (int i=0; i < multipleworlds.size(); i++)
		{
			XMMATRIX temp = multipleworlds.at(i);
			Draw(temp, viewMatrix, projectionMatrix);
		}
	}
}


void GameObject::Draw(XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	if (shader != nullptr) {

		ResourceManager::bindShader(shader);
		shader->SetShaderParameters(world, view, projection);
		switch (shader->type) {
		case ShaderType::MaterialLShader:
		case ShaderType::BasicLShader:
			if (this->light != nullptr) {
				ConstantBufferTypes::LightBuffer lightBuffer;
				lightBuffer.ambientColor = vec4(this->light->Ambient);
				lightBuffer.diffuseColor = vec4(this->light->Diffuse);
				lightBuffer.lightDirection = vec3(this->light->Direction);
				shader->SetShaderConstantBuffer("LightBuffer", &lightBuffer);
			}
		case ShaderType::MaterialShader:
		case ShaderType::SkydomeShader:
		case ShaderType::BasicShader: {
			if (this->material != nullptr) {
				ConstantBufferTypes::MaterialBuffer materialBuffer;
				materialBuffer.ColorMaterial = this->material->color;
				materialBuffer.escalar = this->material->escalar;
				shader->SetShaderConstantBuffer("MaterialBuffer", &materialBuffer);
			}
		}break;
		case ShaderType::TerrainShader:
		{
			if (this->light != nullptr)
			{
				ConstantBufferTypes::LightBuffer lightBuffer;
				lightBuffer.ambientColor = vec4(this->light->Ambient);
				lightBuffer.diffuseColor = vec4(this->light->Diffuse);
				lightBuffer.lightDirection = vec3(this->light->Direction);
				shader->SetShaderConstantBuffer("LightBuffer", &lightBuffer);
			}
			if (this->material != nullptr) {
				ConstantBufferTypes::MaterialBuffer materialBuffer;
				materialBuffer.ColorMaterial = this->material->color;
				materialBuffer.escalar = this->material->escalar;
				shader->SetShaderConstantBuffer("MaterialBuffer", &materialBuffer);
			}
		}break;
		case ShaderType::GUIShader:
		{
			//Check update bitmap
			Modelo->UpdateBitmap();

			if (this->material != nullptr) {
				ConstantBufferTypes::MaterialBuffer materialBuffer;
				materialBuffer.ColorMaterial = this->material->color;
				materialBuffer.escalar = this->material->escalar;
				shader->SetShaderConstantBuffer("MaterialBuffer", &materialBuffer);
			}
		}break;
		};
	}

	for (int i = 0; i < Tex.size(); i++) {
		Tex[i]->BindTexture(i);
	}
	ResourceManager::bindModel(Modelo);
	Modelo->Draw();
}

void GameObject::Collider(double deltaTime) {
	//static float time = 0;
	//if (colision->ColisionSphere(Transform, gameObject->Transform))
	//{
	//	ColsionAnimation = true;
	//	static bool colision = false;
	//	if (time > 0)
	//	{
	//		time -= deltaTime;
	//		AnimationDamage(deltaTime, 3);
	//		if (Damage == false)
	//		{
	//			Damage = true;
	//			CountCorazon--;
	//		}
	//	}
	//	else
	//	{
	//		Damage = false;
	//		time = TimeFlicker;
	//	}

	static float timeAnimation = 3;
	if (CountCorazon != CountCorazonOld)
	{
		CountCorazonOld = CountCorazon;
		Damage = true;
	}
	if (!Damage)
	{
		if (this->material->escalar < 0)
		{
			FilckerBoolean = false;
			this->material->escalar = 0;
		}
		else
			this->material->escalar -= deltaTime * 3;
		return;
	}
	
	AnimationDamage(deltaTime,3);

	if (timeAnimation > 0)
	{
		timeAnimation -= deltaTime;
	}
	else
	{
		timeAnimation = 3;
		Damage = false;
	}

	//	_RPT0(0, "Colison");
	//}
	//else
	//	if (time > 0)
	//	{
	//		time -= deltaTime;
	//		AnimationDamage(deltaTime, 3);
	//	}
	//	else
	//	{
	//		if (this->material->escalar < 0)
	//		{
	//			FilckerBoolean = false;
	//			this->material->escalar = 0;
	//		}
	//		else
	//			this->material->escalar -= deltaTime * 3;
	//	}

}

bool GameObject::DamageColision()
{
	if (CountCorazon > 0)
	{
		CountCorazon--;
		return false;
	}
	else
	{
		CountLifes--;
		if (CountLifes > 0)
		CountCorazon = 3;
		else
			CountCorazon = 0;
	}

	return true;//death
}

//void GameObject::AnimationDamage(double deltaTime, double time)
//{
//	if (FilckerBoolean)
//		this->material->escalar -= deltaTime * time;
//	else
//		this->material->escalar += deltaTime * time;
//
//	if (this->material->escalar < 0)
//	{
//		FilckerBoolean = false;
//		this->material->escalar = 0;
//	}
//	else if (this->material->escalar > 1)
//	{
//		FilckerBoolean = true;
//		this->material->escalar = 1;
//	}
//
//}

vec3 GameObject::RespawnPlayer(Model*Terrain)
{
	//Transform->SetTranslation(vec3(209.899216, 9.18199921, 44.6729889));
	if (CountCorazon != 3)
	{
		if (CountLifes > 0)
		{
			CountLifes--;
			if (CountLifes > 0)
				CountCorazon = 3;
			else
				CountCorazon = 0;
		}

	}

	int width, height;
	width = Terrain->Heightmap.width;
	height = Terrain->Heightmap.height;

	vec3 pos = vec3(rand() % width, rand() % 5 + 8, rand() % height);
	return pos;
}

Colision* GameObject::GetColision()
{
	return colision;
}

void GameObject::Collider(GameObject*gameObject, double deltaTime, Camera3D *Camera) {
	static float time = 0;
	if (colision->ColisionSphere(Transform, gameObject->Transform))
	{
		ColsionAnimation = true;
		static bool colision = false;
		if (time > 0)
		{
			time -= deltaTime;
			AnimationDamage(deltaTime, 3);
			if (Damage == false)
			{
				Damage = true;
				CountCorazon--;
			}
		}
		else
		{
			Damage = false;
			time = TimeFlicker;
		}
		ResourceManager::Donmove = ResourceManager::Player1->LastKey;
		Camera->SetPosition(PosAnt.x, PosAnt.y, PosAnt.z);
		/*Camera->view.x = PosAnt.x;
		Camera->view.y = PosAnt.y;
		Camera->view.z = PosAnt.z;*/
		_RPT0(0, "Colison");
	}
	else
	{
		if (time > 0)
		{
			time -= deltaTime;
			AnimationDamage(deltaTime, 3);
		}
		else
		{
			if (this->material->escalar < 0)
			{
				FilckerBoolean = false;
				this->material->escalar = 0;
			}
			else
				this->material->escalar -= deltaTime * 3;
		}
		//PosAnt = vec3(Camera->view.x, Camera->view.y, Camera->view.z);
		PosAnt = vec3(Camera->GetPosition().x, Camera->GetPosition().y, Camera->GetPosition().z);
	}

}

void GameObject::AnimationDamage(double deltaTime, double time)
{
	if (FilckerBoolean)
		this->material->escalar -= deltaTime * time;
	else
		this->material->escalar += deltaTime * time;

	if (this->material->escalar < 0)
	{
		FilckerBoolean = false;
		this->material->escalar = 0;
	}
	else if (this->material->escalar > 1)
	{
		FilckerBoolean = true;
		this->material->escalar = 1;
	}
}

void GameObject::Respawn()
{
	//Transform->SetTranslation(vec3(209.899216, 9.18199921, 44.6729889));
	CountCorazon = 3;
	CountLifes--;
}
