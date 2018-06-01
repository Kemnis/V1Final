#include "stdafx.h"

ActionBillboard::ActionBillboard(vec3 postion, float r, Transforms*t)
{
	Inizialite(postion, r, t);
}
ActionBillboard::~ActionBillboard()
{

}
string ActionBillboard::Inizialite(vec3 postion, float r, Transforms*t)
{
	vec3 n = vec3(0.0, -1.0, 0.0);
	force = 20.0f;

	transform->SetTranslation(postion);
	transform->SetRotation(vec3(0.0,0.0,0.0));
	transform->SetNormal(n);
	transform->SetRadio(r);
	transform->SetScale(t->GetScale());
	item = rand() % 12;
	death = false;
	return "";
}
void ActionBillboard::Update(double dt)
{
	float dtz = dt;
	if(!death)
	transform->Translate(transform->GetNormal()*force*dtz);
}