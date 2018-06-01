#pragma once
#include "stdafx.h"

class ActionShot : public ActionObject
{
public:
	ActionShot(vec3 pos,vec3 rot, float r, Transforms*t, float intensity);
	~ActionShot();
	string Inizialite(vec3 pos, vec3 rot, float r, Transforms*t, float intensity);
	void Update(double dt);
private:
protected:
};
