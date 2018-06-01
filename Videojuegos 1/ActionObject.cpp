#include "stdafx.h"

ActionObject::ActionObject()
{
	transform = NULL;
	transform = new Transforms();
	death = false;
	destroy = false;
}

ActionObject::~ActionObject()
{

}

string ActionObject::Inizialite(vec2 rangeX, vec2 rangeY, vec3 rangeZ, vec4 rangeAxisYZ)
{
	ciclos = 1;
	return "";
}

void ActionObject::Update(double dt)
{
	return;
}

void ActionObject::Shutdown()
{
	if (transform)
	{
		delete transform;
		transform = NULL;
	}
}
