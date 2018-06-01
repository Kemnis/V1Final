#pragma once
//#ifndef _ResourceManager_H_
//#define _ResourceManager_H_
#include "GameObject.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "BasicShader.h"
#include "MaterialShader.h"
#include "Material.h"
#include "Light.h"
#include "KeyHandler.h"
struct ObjectInformation {
	string Name, Type, CreatedBy;
};
struct enemies {
	float velocity;
	float accuracy;
	float view;
	Model *ProjectileMesh = nullptr;
	int MyDesitionToMove = 0;
	vector<vec3> MyZone;
	GameObject *target;
	GameObject *I;
};
class PalabrasYOraciones
{
public:
	Model* Letras[10];
	Texture* Contenido[10];
	string PalabraCompleta;
	int count;
	PalabrasYOraciones()
	{
		count = 0;
		/*Letras[0] = nullptr;
		Letras[1] = nullptr;
		Letras[2] = nullptr;
		Letras[3] = nullptr;
		Letras[4] = nullptr;
		Letras[5] = nullptr;
		Letras[6] = nullptr;
		Letras[7] = nullptr;
		Letras[8] = nullptr;
		Letras[9] = nullptr;*/
	}
};
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static bool AddModel(string idFrom,string path, string name);
	static bool AddTexture(string idFrom,string path, string name);
	static bool AddStage(string idFrom,string path, string name, float Cells, float Witdth,float Height);
	static bool AddShader(string idFrom,string name, Shader* shader);
	static bool AddMaterial(string idFrom,string Nombre, vec4 Color);
	static bool AddLight(string idFrom,string Nombre, vec4 ambient, vec4 diffuse, vec3 direction);

	//Not delete change Scene
	//Define defualt system

	static bool AddBillboard(string idFrom, string Nombre, vec2 coordPositivo, vec2 coordNegativo);//1 Billboard Default
	static bool AddBitmap(string idFrom, string Nombre, vec4 rectBimap);//Count n  Bitmaps Default System

	static string BuildGameObject(string idFrom, string nameGameObject, string meshname, string texturename, string shadername, string materialname, string lightname);
	static string BuildGameObject(string idFrom, string nameGameObject, Model* Modelo, string texturename, string shadername, string materialname, string lightname);
	static string LoadLetters(string idFrom, string path, string Abecedario);
	static string BuildWord(string idFrom, string idWord, string Palabra, vec2 Pos);
	static string DrawWord(string idFrom, string idWord);
	static GameObject* GetObjectByName(string Name);
	


	static bool DestroyItemsFromScene(string idFrom);
	static bool DestroyAllGameObjects();
	static bool DestroyAllSahders();
	static bool DestroyAllModels();//Not Delete Mesh Billbaord and Bitmaps
	static bool DestoryAllMaterial();
	static bool DestroyAllTexture();
	static bool DestroyAllLight();

	static string AsingTextureToGameObject(string nameGameObject, string nameTexture);
	static Material*GetMaterial(string nameMaterial);

	static bool bindShader(Shader* basicshader);
	static bool bindModel(Model * model);
	static vec3 LookAt(vec3 pos, vec3 focus);
	static void Shutdown();

	using GameObjectMap = std::map<string, GameObject>;
	using ModelMap = std::map<string, Model>;
	using WordMap = std::map<string, PalabrasYOraciones>;
	using TextureMap = std::map<string, Texture>;
	using ShaderMap = std::map<string, Shader*>;
	using MaterialMap = std::map<string, Material>;
	using LightMap = std::map<string, Light>;
	using GlobalResourcesMap = std::map<string, ObjectInformation>;

	//Informacion de pantalla
	static int ScreenWidthF;
	static int ScreenHeightF;
	static int ScreenWidth;
	static int ScreenHeight;
	static char GraphicMode;
	static char Donmove;

	//Player 1
	static KeyHandler* Player1;




	//Enemies
	static void DefinePositionPatrol(vector<vec3> Posiciones);
	static void DefineTargetToFight(string target);
	static void MoveTowardDestination();
	static string StartEnemy(float vel, float accuraci, string Projectile, float vision);
	static string UpdateEnemy();
	static bool GetMyBody(string MyObject);

	static enemies behaviour;

protected:
	static bool AmIOnDestination();
	static bool AddGameObject(GameObject gameObject, string idFrom);
	//Actuales
	static Shader* ShaderActual;
	static Model* ModeloActual;
	//Objetos
	static GameObjectMap GameObjectIdentifier;
	static int GameObjectIndex;
	//Modelos
	static ModelMap ModelIdentifier;
	static int ModelIndex;
	//Palabras
	static WordMap WordIdentifier;
	static int WordIndex;
	//Texture
	static TextureMap TextureIdentifier;
	static int TextureIndex;
	//Shader
	static ShaderMap ShaderIdentifier;
	//Material
	static MaterialMap MaterialIdentifier;
	static int MaterialIndex;
	//Material
	static LightMap LightIdentifier;
	static int LightIndex;
	//SceneResources
	static GlobalResourcesMap GlobalObjectsIdentifier;
	static int GlobalObjectsCounter;
};

//#endif