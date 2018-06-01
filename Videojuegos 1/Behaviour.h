#pragma once
enum class BehaviourType {
	Enemy = 1,
	Asteroid = 2
};

class Behaviour : public DxComponent<Shader>
{
private:

public:
	Behaviour(BehaviourType type);
	~Behaviour();
	BehaviourType type;
	virtual string Start()=0;
	virtual string Update()=0;

};
