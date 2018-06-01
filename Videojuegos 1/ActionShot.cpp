#include "stdafx.h"

ActionShot::ActionShot(vec3 pos, vec3 rot, float r, Transforms*t, float intensity) :ActionObject()
{
	Inizialite(pos,rot,r,t, intensity);
}

ActionShot::~ActionShot()
{

}

string ActionShot::Inizialite(vec3 pos, vec3 rot, float r, Transforms*t,float intensity)
{
	XMFLOAT3 n = XMFLOAT3(0.0,0.0,1.0);//Normal Direction default axis z
	transform->SetTranslation(pos);
	transform->SetScale(t->GetScale());
	transform->SetRotation(rot);
	transform->SetRadio(r);

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x*0.0174532925f, rot.y*0.0174532925f, rot.z*0.0174532925f);
	XMVECTOR direction;
	direction = XMLoadFloat3(&n);
	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	direction = XMVector3TransformCoord(direction, rotationMatrix);
	XMStoreFloat3(&n, direction);
	transform->SetNormal(vec3(n.x,n.y,n.z));
	force = intensity;

	return "";
}


void ActionShot::Update(double dt)
{
	float dtf = dt;
	vec3 vr = transform->GetNormal()*force*dtf;

	if(!death)
	transform->Translate(vr);

	if (death)
	{
		if (TimeDeath > 0)
			TimeDeath -= dt;
		else
			destroy = true;
	}


}