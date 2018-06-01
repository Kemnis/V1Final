#pragma once
#ifndef _GameManager_H_
#define _GameManager_H_
#include <map>
#include "Camera3D.h"
#include "Mesh3D.h"
#include "BasicShader.h"
#include "GameObject.h"
#include "SpecsDx.h"
#include "Scene.h"
class GameManager
{
public:
	
	GameManager();
	~GameManager();
	string DefineNumberOfLevels();
	virtual string FrameProcess(double DeltaTime);
	virtual string FrameRender();
	string AddScene(Scene* pScene, int indexScene);

	Scene * GetCurrentScene();
	int GetCurrentIdScene();

	void ChangeScene(int newScene);
	bool OnChangeScene(int oldScene, int newScene);
	virtual void Shutdown();

	string Name;
protected:
	//Variables de respuesta
	bool RB;
	string RS;
	// Variables de respuesta
		
	HWND hwnd;

	//Escenas
	std::map<int, Scene*> Niveles;
	Scene* CurrentScene;
	int CurrentSceneIndex;
	
private:

};

#endif