#pragma once
#include "stdafx.h"

class ActionGravity : public ActionObject
{
public:
	ActionGravity(vec2 rangeX, vec2 rangeY, vec2 rangeZ, vec4 rangeAxisYZ, float r,Transforms*t);
	~ActionGravity();
	string Inizialite(vec2 rangeX,vec2 rangeY,vec2 rangeZ,vec4 rangeAxisYZ, float r, Transforms*t);//Inicializa el objecto mirando hacia abajo
	void Update(double dt);
private:
protected:
};