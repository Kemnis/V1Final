#pragma once
#include "stdafx.h"

class ActionBillboard : public ActionObject
{
public:
	ActionBillboard(vec3 postion, float r, Transforms*t);
	~ActionBillboard();
	string Inizialite(vec3 postion, float r, Transforms*t);//Inicializa el objecto mirando hacia abajo
	void Update(double dt);
private:
protected:
};