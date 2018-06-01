#include "ResourceManager.h"
#include "stdafx.h"
#include "GameObject.h"
#include "Model.h"
#include "Texture.h"
#include "BasicShader.h"
#include "MaterialShader.h"
#include "Material.h"
#include "Light.h"
#include "KeyHandler.h"
Shader* ResourceManager::ShaderActual = nullptr;
Model* ResourceManager::ModeloActual = nullptr;

ResourceManager::GameObjectMap ResourceManager::GameObjectIdentifier;
ResourceManager::ModelMap ResourceManager::ModelIdentifier;
ResourceManager::WordMap ResourceManager::WordIdentifier;
ResourceManager::TextureMap ResourceManager::TextureIdentifier;
ResourceManager::ShaderMap ResourceManager::ShaderIdentifier;
ResourceManager::MaterialMap ResourceManager::MaterialIdentifier;
ResourceManager::LightMap ResourceManager::LightIdentifier;
ResourceManager::GlobalResourcesMap ResourceManager::GlobalObjectsIdentifier;
int ResourceManager::GameObjectIndex = 0;
int ResourceManager::ModelIndex = 0;
int ResourceManager::WordIndex = 0;
int ResourceManager::TextureIndex = 0;
int ResourceManager::MaterialIndex = 0;
int ResourceManager::LightIndex = 0;
int ResourceManager::GlobalObjectsCounter = 0;

//Informacion de la pantalla
int ResourceManager::ScreenWidthF = GetSystemMetrics(SM_CXSCREEN);
int ResourceManager::ScreenHeightF = GetSystemMetrics(SM_CYSCREEN);
int ResourceManager::ScreenWidth = 800;
int ResourceManager::ScreenHeight = 600;
char ResourceManager::GraphicMode = 'W';
char ResourceManager::Donmove= 'O';

KeyHandler* ResourceManager::Player1 = nullptr;

//Player1

enemies ResourceManager::behaviour;


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

//Add Block
bool ResourceManager::AddGameObject(GameObject Gameobject, string idFrom)
{
	GameObjectIdentifier.insert(std::pair<string, GameObject>(Gameobject.GetName(), Gameobject));
	GameObjectIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = Gameobject.GetName();
	Info.Type = "GameObject";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddModel(string idFrom, string path, string name)
{
	Model nuevo(path);
	nuevo.Name = name;
	ModelIdentifier.insert(std::pair<string, Model>(name, nuevo));
	ModelIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = name;
	Info.Type = "Model";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddTexture(string idFrom, string path, string name)
{
	Texture nuevo;
	nuevo.Name = name;
	nuevo.Initialize(path);
	TextureIdentifier.insert(std::pair<string, Texture>(name, nuevo));
	TextureIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = name;
	Info.Type = "Texture";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddShader(string idFrom, string name, Shader* shader) {
	shader->Name = name;
	ShaderIdentifier.insert(std::pair<string, Shader*>(name, shader));
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = name;
	Info.Type = "Shader";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddStage(string idFrom, string HeightmapFile, string name, float Cells, float Width, float Height)
{
	Model nuevo(HeightmapFile,Cells, Width, Height);
	nuevo.Name = name;
	ModelIdentifier.insert(std::pair<string, Model>(name, nuevo));
	ModelIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = name;
	Info.Type = "Model";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddBillboard(string idFrom, string Nombre, vec2 coordPositivo, vec2 coordNegativo)
{
	Model nuevo(coordPositivo, coordNegativo);
	nuevo.Name = Nombre;
	ModelIdentifier.insert(std::pair<string, Model>(Nombre, nuevo));
	ModelIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = Nombre;
	Info.Type = "Model";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddBitmap(string idFrom, string Nombre, vec4 rectBimap)
{
	Model* nuevo;
	if (GraphicMode == 'W')
		nuevo = new Model(rectBimap, ResourceManager::ScreenWidth, ResourceManager::ScreenHeight);
	else
		nuevo = new Model(rectBimap, ResourceManager::ScreenWidthF, ResourceManager::ScreenHeightF);
	nuevo->Name = Nombre;
	ModelIdentifier.insert(std::pair<string, Model>(Nombre, *nuevo));
	ModelIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = Nombre;
	Info.Type = "Model";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddMaterial(string idFrom, string Nombre, vec4 Color)
{
	Material nuevo;
	nuevo.Name = Nombre;
	nuevo.color = Color;
	nuevo.escalar = 0.0f;
	MaterialIdentifier.insert(std::pair<string, Material>(Nombre, nuevo));
	MaterialIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = Nombre;
	Info.Type = "Material";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

bool ResourceManager::AddLight(string idFrom, string Nombre, vec4 ambient, vec4 diffuse, vec3 direction)
{
	Light nuevo;
	nuevo.Name = Nombre;
	nuevo.Ambient = ambient;
	nuevo.Diffuse = diffuse;
	//nuevo.Diffuse = diffuse;
	nuevo.Direction = direction;
	LightIdentifier.insert(std::pair<string, Light>(Nombre, nuevo));
	LightIndex++;
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = Nombre;
	Info.Type = "Light";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return true;
}

string ResourceManager::BuildGameObject(string idFrom, string nameGameObject, string meshname, string texturename, string shadername, string materialname, string lightname)
{
	GameObject nuevo(nameGameObject);
	if (meshname != "")
	{
		nuevo.AssignModel(&ModelIdentifier.find(meshname)->second);
		if (nuevo.GetModel() == nullptr)
			return "E_Fail";
	}
	if (texturename != "")
	{
		Texture* texture = &TextureIdentifier.find(texturename)->second;
		if (texture != nullptr) {
			nuevo.AddTexture(texture);
		}
	}
	if (shadername != "")
	{
		Shader* shader = ShaderIdentifier.find(shadername)->second;
		if (shader != nullptr) {
			nuevo.AssignShader(shader);
			if (nuevo.ExistShader() == false)
				return "E_Fail";
		}
	}

	if (materialname != "")
	{
		nuevo.AssignMaterial(&MaterialIdentifier.find(materialname)->second);
		if (nuevo.GetMaterial() == nullptr)
			return "E_Fail";
	}

	if (lightname != "" || nuevo.GetShader()->flagLight == 1)
	{
		nuevo.AssignLight(&LightIdentifier.find(lightname)->second);
		if (nuevo.GetLight() == nullptr)
			return "E_Fail";
	}
	AddGameObject(nuevo,idFrom);
	return "S_OK";
}

string ResourceManager::BuildGameObject(string idFrom, string nameGameObject, Model* Modelo, string texturename, string shadername, string materialname, string lightname)
{
	GameObject nuevo(nameGameObject);
	nuevo.AssignModel(Modelo);
	if (nuevo.GetModel() == nullptr)
		return "E_Fail";
	if (texturename != "")
	{
		Texture* texture = &TextureIdentifier.find(texturename)->second;
		if (texture != nullptr) {
			nuevo.AddTexture(texture);
		}
	}
	if (shadername != "")
	{
		Shader* shader = ShaderIdentifier.find(shadername)->second;
		if (shader != nullptr) {
			nuevo.AssignShader(shader);
			if (nuevo.ExistShader() == false)
				return "E_Fail";
		}
	}

	if (materialname != "")
	{
		nuevo.AssignMaterial(&MaterialIdentifier.find(materialname)->second);
		if (nuevo.GetMaterial() == nullptr)
			return "E_Fail";
	}

	if (lightname != "" || nuevo.GetShader()->flagLight == 1)
	{
		nuevo.AssignLight(&LightIdentifier.find(lightname)->second);
		if (nuevo.GetLight() == nullptr)
			return "E_Fail";
	}
	AddGameObject(nuevo, idFrom);
	return "S_OK";
}

string ResourceManager::LoadLetters(string idFrom, string path, string Abecedario)
{
	std::string str = Abecedario;
	string constante = "Letter:";
	for (char& c : str) {
		Texture nuevo();
		AddTexture(idFrom,path + c + ".png", constante + c);
	}
	return "S_OK";
}

string ResourceManager::BuildWord(string idFrom, string idWord, string PalabraMax10, vec2 pos)
{
	PalabrasYOraciones Palabra;
	Palabra.PalabraCompleta = PalabraMax10;
	string constante = "Letter:";
	std::string str = PalabraMax10;
	for (char& c : str) {
		if (Palabra.count < 10)
		{
			if (GraphicMode == 'W')
				Palabra.Letras[Palabra.count] = new Model(vec4((pos.x*(Palabra.count + 1)), (pos.y*(Palabra.count + 1)), 20, 20), ScreenWidth, ScreenHeight);
			else
				Palabra.Letras[Palabra.count] = new Model(vec4((pos.x*(Palabra.count + 1)), (pos.y*(Palabra.count + 1)), 20, 20), ScreenWidthF, ScreenHeightF);
			Palabra.Contenido[Palabra.count] = &TextureIdentifier.find(constante + c)->second;
			Palabra.Letras[Palabra.count]->Name = idWord + c;
			Palabra.Letras[Palabra.count]->Type = "Letter";
			//Construir GameObject para ser dibujado
			string res = ResourceManager::BuildGameObject(idFrom, idWord + c, Palabra.Letras[Palabra.count],
				constante + c, "GUIShader", "ColorBlanco", "");
			Palabra.count++;
		}
	}
	WordIdentifier.insert(std::pair<string, PalabrasYOraciones>(idWord, Palabra));
	WordIndex++;
	
	//Identify if the resource is from Scene
	ObjectInformation Info;
	Info.CreatedBy = idFrom;
	Info.Name = idWord;
	Info.Type = "Letter";
	GlobalObjectsIdentifier.insert(std::pair<string, ObjectInformation>(to_string(GlobalObjectsCounter), Info));
	GlobalObjectsCounter++;
	return "S_OK";
}

string ResourceManager::AsingTextureToGameObject(string nameGameObject, string nameTexture)
{
	if (nameGameObject != "" && nameTexture != "")
	{
		Texture* texture = &TextureIdentifier.find(nameTexture)->second;
		GameObject* gameObject = ResourceManager::GetObjectByName(nameGameObject);
		if (texture == nullptr || gameObject == nullptr)
		{
			return "E_Fail";
		}
		gameObject->AddTexture(texture);
	}

	return "S_OK";
}

//Destory Objects

bool ResourceManager::DestroyItemsFromScene(string idFrom)
{
	string PosAnt = "";
	for (GlobalResourcesMap::iterator it = GlobalObjectsIdentifier.begin(); it != GlobalObjectsIdentifier.end(); ++it)
	{
		if (PosAnt !="")
		{
			if (GlobalObjectsIdentifier.find(PosAnt)->second.CreatedBy == idFrom)
				GlobalObjectsIdentifier.erase(PosAnt);
		}
		if (it->second.CreatedBy == idFrom)
		{
			ObjectInformation identifier = it->second;

			if (identifier.Type == "Model")
			{
				Model* aObject = &ModelIdentifier.find(identifier.Name)->second;
				aObject->ShutdownModel();
				ModelIdentifier.erase(ModelIdentifier.find(identifier.Name)->first);
			}
			else if (identifier.Type == "GameObject")
			{
				GameObject* aObject = &GameObjectIdentifier.find(identifier.Name)->second;
				aObject->Shutdown();
				GameObjectIdentifier.erase(GameObjectIdentifier.find(identifier.Name)->first);
			}
			else if (identifier.Type == "Texture")
			{
				Texture* aObject = &TextureIdentifier.find(identifier.Name)->second;
				aObject->Shutdown();
				TextureIdentifier.erase(TextureIdentifier.find(identifier.Name)->first);
			}
			else if (identifier.Type == "Shader")
			{
				Shader* aObject = ShaderIdentifier.find(identifier.Name)->second;
				aObject->Shutdown();
				ShaderIdentifier.erase(ShaderIdentifier.find(identifier.Name)->first);
			}
			else if (identifier.Type == "Material")
			{
				Material* aObject = &MaterialIdentifier.find(identifier.Name)->second;
				MaterialIdentifier.erase(MaterialIdentifier.find(identifier.Name)->first);
			}
			else if (identifier.Type == "Light")
			{
				Light* aObject = &LightIdentifier.find(identifier.Name)->second;
				LightIdentifier.erase(LightIdentifier.find(identifier.Name)->first);
			}
			PosAnt = it->first;
		}
		else
			PosAnt = "";
	}
	if (PosAnt != "")
	{
		if (GlobalObjectsIdentifier.find(PosAnt)->second.CreatedBy == idFrom)
			GlobalObjectsIdentifier.erase(PosAnt);
	}
	return true;
}

bool ResourceManager::DestroyAllModels() 
{
	for (ModelMap::iterator it = ModelIdentifier.begin(); it!= ModelIdentifier.end();++it)
	{

		it->second.ShutdownModel();
	}
	ModelIdentifier.clear();

	return true;
}

bool ResourceManager::DestoryAllMaterial()
{
	MaterialIdentifier.clear();
	return true;
}

bool ResourceManager::DestroyAllSahders()
{
	for (ShaderMap::iterator it = ShaderIdentifier.begin(); it != ShaderIdentifier.end(); ++it)
	{
		it->second->Shutdown();
	}
	ShaderIdentifier.clear();
	return true;
}
bool ResourceManager::DestroyAllGameObjects()
{
	GameObjectIdentifier.clear();
	return true;
}

bool ResourceManager::DestroyAllLight()
{
	LightIdentifier.clear();
	return true;
}

bool ResourceManager::DestroyAllTexture()
{
	for (TextureMap::iterator it = TextureIdentifier.begin(); it!=TextureIdentifier.end();++it)
	{
		it->second.Shutdown();
	}
	TextureIdentifier.clear();
	return true;
}

// Get Objects

GameObject* ResourceManager::GetObjectByName(string nameSearch)
{
	return &GameObjectIdentifier.find(nameSearch)->second;
}


Material*ResourceManager::GetMaterial(string nameMaterial)
{
	Material * material = &MaterialIdentifier.find(nameMaterial)->second;

	return material;
}

bool ResourceManager::bindShader(Shader * shader) {
	if (ShaderActual != shader)
	{
		shader->BindShader();
		ShaderActual = shader;
		return true;
	}
	return false;
}

bool ResourceManager::bindModel(Model * model)
{
	if (ModeloActual != model)
	{
		ModeloActual = model;
		ModeloActual->BindMesh(/*(ModeloActual->Type =="Terrain") ? D3D10_PRIMITIVE_TOPOLOGY_LINELIST : */D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return true;
	}
	return false;
}

void ResourceManager::Shutdown()
{
	GameObjectIdentifier.clear();
	ModelIdentifier.clear();
	TextureIdentifier.clear();
	ShaderIdentifier.clear();
	MaterialIdentifier.clear();
	LightIdentifier.clear();
	GlobalObjectsIdentifier.clear();
	GameObjectIndex = 0;
	ModelIndex = 0;
	TextureIndex = 0;
	MaterialIndex = 0;
	LightIndex = 0;
	GlobalObjectsCounter = 0;
}













//Enemies

string ResourceManager::StartEnemy(float vel, float accuraci, string Projectile, float vision)
{
	behaviour.view = vision;
	behaviour.velocity = vel;
	behaviour.accuracy = accuraci;
	behaviour.ProjectileMesh = &ModelIdentifier.find(Projectile)->second;;
	behaviour.MyDesitionToMove = rand() % behaviour.MyZone.size();
	return "S_OK";
}

void ResourceManager::DefinePositionPatrol(vector<vec3> Posiciones) {
	behaviour.MyZone = Posiciones;
}

void ResourceManager::DefineTargetToFight(string target)
{
	behaviour.target = &GameObjectIdentifier.find(target)->second;
}


bool ResourceManager::GetMyBody(string MyObject) {
	behaviour.I = &GameObjectIdentifier.find(MyObject)->second;
	return true;
}

string ResourceManager::UpdateEnemy()
{
	if (AmIOnDestination())
		behaviour.MyDesitionToMove = rand() % behaviour.MyZone.size();
	else
	{
		MoveTowardDestination();
	}

	return "S_OK";
}


void ResourceManager::MoveTowardDestination()
{
	//Get vector toward destination
	vec3 vectop2 = behaviour.MyZone.at(behaviour.MyDesitionToMove) - behaviour.I->Transform->GetTranslation();
	if (vectop2.x < behaviour.I->Transform->GetTranslation().x);
	{	behaviour.I->Transform->Translate(vec3(.001, 0, 0)); }
	//if(vectop2.x > behaviour.I->Transform->GetTranslation().x);
	//{	behaviour.I->Transform->Translate(vec3(-.001, 0, 0)); }

}

bool ResourceManager::AmIOnDestination()
{
	if (behaviour.I == nullptr)
		return false;
	if (behaviour.I->Transform->GetTranslation() == behaviour.MyZone.at(behaviour.MyDesitionToMove))
	{
		return true;
	}
	else
		return false;
	return true;

}

vec3 ResourceManager::LookAt(vec3 pos, vec3 focus)
{
	vec3 v1, v2;
	vec3 axis = vec3(0.0, 0.0, 0.0);
	vec3 vr = pos - focus;
	float r = sqrt(pow(vr.x, 2) + pow(vr.y, 2) + pow(vr.z, 2));

	axis.y = degrees((float)atan2(vr.x, vr.z));
	axis.z = degrees((float)acos((-vr.y) / r));

	return axis;

}