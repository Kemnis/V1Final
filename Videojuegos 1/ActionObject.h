#pragma once
#include "stdafx.h"

enum class ActionType
{
	Gravity = 1,
	Shot = 2
};
class ActionObject
{
public:
	ActionObject();
	~ActionObject();
	virtual string Inizialite(vec2 rangeX, vec2 rangeY, vec3 rangeZ, vec4 rangeAxisYZ);
	virtual void Update(double dt);
	void Shutdown();
	bool death;
	bool destroy;
	Transforms*transform;
	float ciclos;
	int item;
protected:
	double TimeDeath = 3;
	float force;
	


};