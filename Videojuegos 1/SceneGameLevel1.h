#pragma once
#include "Scene.h"

class SceneGameLevel1:public Scene
{
public:
	SceneGameLevel1(string id);
	~SceneGameLevel1();
	string CreateScene();
	string LoadResources();
	string BuildScene();
	string Start();
	string ProcessScene(double dt);
	string RenderScene(); 
	void DestroyScene();
	bool DownloadResources();
	void UpdateAsteriods(GameObject*terrain, GameObject*go, double dt);
	void DrawsAsteriods(GameObject*go, XMMATRIX view, XMMATRIX proj);

	void CreateShot(GameObject*bullet, vec3 rot, float intensity);
	void UpdateShot(GameObject*terrain, double dt, float gravityforce);
	void DrawsBullets(GameObject*go, XMMATRIX view, XMMATRIX proj);

	void CreateItems(GameObject*go, vec3 pos);
	void UpdateItems(GameObject*terrain, GameObject*go, double dt);
	void DrawsItems( XMMATRIX view, XMMATRIX proj);

	//RandomBillboards
	int Totalbillboards;
	vec2 rangeYBillboards;
	vec2 rangeZBillboards;
	vec2 rangeXBillboards;
	vector<vec4> PosBill;

	void CalculateAllPositionsOfBillboards();
	void ConvertBillPosToGameObjectAndDraw(GameObject * Stars, GameObject * Stars2, XMMATRIX view, XMMATRIX* projectionMatrix);

	//Bullets
	void BulletsIndicator();
	void BulletsChangeType();
	bool AssaultBullet;
	int MaxAssaultBullets;
private:
protected:
};