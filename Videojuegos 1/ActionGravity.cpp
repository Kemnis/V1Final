#include "stdafx.h"

ActionGravity::ActionGravity(vec2 rangeX, vec2 rangeY, vec2 rangeZ, vec4 rangeAxisYZ, float r, Transforms*t) :ActionObject()
{
	Inizialite(rangeX, rangeY, rangeZ, rangeAxisYZ,r,t);
}
ActionGravity::~ActionGravity()
{

}
//
string ActionGravity::Inizialite(vec2 rangeX, vec2 rangeY, vec2 rangeZ, vec4 rangeAxisYZ, float r, Transforms*t)
{
	vec3 pos = vec3(rand() % (int)rangeX.x + (int)rangeX.y,
					rand() % (int)rangeY.x + (int)rangeY.y,
					rand() % (int)rangeZ.x + (int)rangeZ.y);

	vec3 rot = vec3( 0.0,
					rand() % (int)rangeAxisYZ.x + (int)rangeAxisYZ.y,
					rand() % (int)rangeAxisYZ.z + (int)rangeAxisYZ.w);
	vec3 n = vec3(0.0,-1.0,0.0);
	force = 0.1f;

	transform->SetTranslation(pos);
	transform->SetRotation(rot);
	transform->SetNormal(n);
	transform->SetRadio(r);
	transform->SetScale(t->GetScale());

	return "";
}

void ActionGravity::Update(double dt)
{
	if(!death)
	transform->Translate(transform->GetNormal()*force);
	if (death)
	{
		if (TimeDeath > 0)
			TimeDeath -= dt;
		else
			destroy = true;
	}
	

	return;
}