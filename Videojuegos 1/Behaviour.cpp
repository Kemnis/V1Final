#include "stdafx.h"

Behaviour::Behaviour(BehaviourType type){
	this->type = type;
}

Behaviour::~Behaviour(){}

string Behaviour::Start() {
	return "S_OK";
}

string Behaviour::Update() {
	return "S_OK";
}

