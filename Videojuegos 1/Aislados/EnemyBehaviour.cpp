#include "stdafx.h"
#include "EnemyBehaviour.h"
#include "GameObject.h"
EnemyBehaviour::EnemyBehaviour(GameObject *MyObject, float vel, float accuraci, Model *Projectile, float vision) 
{
	IBe = MyObject;
	Start(vel, accuraci, Projectile, vision);
}

EnemyBehaviour::~EnemyBehaviour()
{

}

string EnemyBehaviour::Start(float vel, float accuraci, Model *Projectile, float vision)
{
	view = vision;
	velocity = vel;
	accuracy = accuraci;
	ProjectileMesh = *Projectile;
	return "S_OK";
}

void EnemyBehaviour::DefineTargetToFight(GameObject *target)
{
	Target = target;
}


bool EnemyBehaviour::GetMyBody(GameObject *MyObject) {
	IBe = MyObject;
	return true;
}

string EnemyBehaviour::Update()
{
	if(AmIOnDestination())
		MyDesitionToMove = rand() % MyZone.size();
	else
	{

	}

	return "S_OK";
}

void EnemyBehaviour::MoveTowardDestination()
{
	//Get vector toward destination
	vec3 vectop2 = MyZone.at(MyDesitionToMove) - IBe->Transform->GetTranslation();

}

bool EnemyBehaviour::AmIOnDestination()
{
	if(IBe == nullptr)
		return false;
	if (IBe->Transform->GetTranslation() == MyZone.at(MyDesitionToMove))
	{
		return true;
	}
	else
		return false;
	return true;
}