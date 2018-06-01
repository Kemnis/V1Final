#pragma once
#include "Model.h"

class Colision
{
public:
	Colision();

	//bool Initialize(GameObject*Go1, GameObject*Go2);

	bool ColisionSphere(Transforms*t1, Transforms*t2);
	bool ColisionSphereBlock(Transforms*t1, Transforms*t2);
		//Colsiona al terrno deacuerdo con el radio esferico del modelo
	bool ColsionTerrain(Model*terrain, Transforms*transform);
private:
protected:
};