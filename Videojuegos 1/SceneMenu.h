#pragma once
#include "Scene.h"

class SceneMenu:public Scene
{
public:
	SceneMenu(string id);
	~SceneMenu();
	string CreateScene();
	string LoadResources();
	string BuildScene();
	string Start();
	string ProcessScene(double dt);
	string RenderScene();
	void DestroyScene();
	bool DownloadResources();
private:
protected:
};
