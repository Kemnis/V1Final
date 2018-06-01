#pragma once
#ifndef _EnemyBehaviour_H_
#define _EnemyBehaviour_H_
#include "stdafx.h"
#include "Model.h"
#include "GameObject.h"
using namespace std;

class EnemyBehaviour
{
private:
	float accuracy;
	Model ProjectileMesh;
	GameObject *IBe;
	GameObject * Target;
	int MyDesitionToMove = 0;
	float view, velocity;

	void DefineMydestination();

protected:
	bool AmIOnDestination();

public:
	EnemyBehaviour(GameObject *MyObject, float vel, float accuraci, Model *Projectile, float vision);
	~EnemyBehaviour();

	string Start(float vel, float accuraci, Model *Projectile, float vision);

	string Process();

	string Update();

	void DefineTargetToFight(GameObject *target);
	void MoveTowardDestination();

	vector<vec3> MyZone;
	bool GetMyBody(GameObject *MyObject);

protected:

};

#endif