#pragma once
#include "Input.h"
#include <math.h>
#include <DirectXMath.h>
using namespace DirectX;

class KeyHandler
{
public:
	KeyHandler(vec3 Pos, vec3 Rot);
	KeyHandler(XMFLOAT3 Pos, XMFLOAT3 Rot);
	~KeyHandler();

	void UpdatePosition(XMFLOAT3 XYZ);
	void UpdatePosition(vec3 XYZ);
	void UpdatePosition(float X, float Y, float Z);
	void UpdateRotation(XMFLOAT3 XYZ);
	void UpdateRotation(vec3 XYZ);
	void UpdateRotation(float X, float Y, float Z);
	void DoIntensity();
	float GetIntensity();
	bool ContinuosShoot();
	bool ChargeShoot();
	void RestoreIntensity();
	void MenuUp();
	void MenuDown();

	void Update(int *scene);
	void LookAtObject(vec3 posPlayer,vec3 focus);

	void SetPosition(vec3 pos);
	vec3 GetPosition();
	vec3 GetRotation();
	vec3 MyRot;
	bool thridPerson;
	GameObject*focus;
	Input* Keyboard;
	KeyCode KeyCodes;
	bool requestBullet, QueueBullet, CambioEstado, RequestQuitGame;
	char LastKey;
	int actualOption,maxOptions;
	bool ChangeWeapon;
private:
	void KeyEventMenu(int *changeScene);
	void KeyEventLevel1(int *changeScene);
	void KeyEventLevel0(int *changeScene);
	vec3 MyPos;
	float intensity = 15, MaxIntensity = 30;
	vec3 RotLerp;//For focus object
	float forceSide, MaxForceSide, forceFront, MaxForceFront;
	float LookLR, LookLRMax, LookUD, LookUDMax;
	float TurnLR, TurnLRMax;
	double FlyUD, FlyUDMax;
	
	
	bool EnableMouse;

	char Sides, Front, LSide, LFront, Turn, Fly;

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void FlyUpward();
	void FlyDownward();
	void LookUpward();
	void LookDownward();
	void TurnLeft();
	void TurnRight();
	void LookLeft();
	void LookRight();
};