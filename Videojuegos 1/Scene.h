#pragma once
#include "Camera3D.h"
#include "Mesh3D.h"
#include "ActionObject.h"


class Scene :public DxComponent<Scene>
{
public:
	Scene(string id);
	~Scene();
	virtual string LoadResources();
	virtual string CreateScene();
	virtual string BuildScene();
	virtual string Start();
	virtual string ProcessScene(double dt);
	virtual string RenderScene();
	virtual void DestroyScene();
	virtual bool DownloadResources();
	virtual string GetNameCodeScene();
public:
	Camera3D * SceneCamera;
	bool FirstTimeProcessed, FirstTimeRendered;
	string ProcesedSucess, RenderedSucess;
protected:
	string RS,id, nameScene;
	bool RB;
	void DrawSphereColsion(GameObject*GoColi, GameObject*gameObject, XMMATRIX view, XMMATRIX projecton);
	std::vector<GameObject*> List_Corazones;
	std::vector<GameObject*> List_Vidas;
	std::vector<ActionObject*> List_Asteroid;
	std::vector<ActionObject*> List_Bullet;
	std::vector<ActionObject*> List_BillboardItems;
	bool win=false, loser = false;
private:
	XMMATRIX CameraMatrix;
};