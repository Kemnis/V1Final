#include "stdafx.h"
KeyHandler::KeyHandler(vec3 Pos, vec3 Rot)
{
	MyPos = { Pos.x,Pos.y,Pos.z };
	MyRot = { Rot.x,Rot.y,Rot.z };
	forceSide = 0, forceFront = 0;
	MaxForceSide = 1.3,	MaxForceFront = 1.3;
	LookLR = 0, LookLRMax = 1;
	LookUD = 0, LookUDMax = 1;
	TurnLR = 0, TurnLRMax = 1.5;
	FlyUD = 0, FlyUDMax = 1;
	Keyboard = new Input();
	Keyboard->Initialize(0,0);
	Sides = 'N', Front = 'N', LSide = 'N', LFront = 'N';
	thridPerson = false;
	focus = NULL;
	focus = NULL;
	EnableMouse = false;
	RequestQuitGame = false;
	actualOption = 0;
	maxOptions = 1;
	ChangeWeapon = false;
}

KeyHandler::KeyHandler(XMFLOAT3 Pos, XMFLOAT3 Rot)
{
	MyPos = { Pos.x,Pos.y,Pos.z };
	MyRot = { Rot.x,Rot.y,Rot.z };
	forceSide = 0, forceFront = 0;
	MaxForceSide = 1.3, MaxForceFront = 1.3;
	LookLR = 0, LookLRMax = 1;
	LookUD = 0, LookUDMax = 1;
	TurnLR = 0, TurnLRMax = 1.5;
	FlyUD = 0, FlyUDMax = 1;
	Keyboard = new Input();
	Keyboard->Initialize(0, 0);
	Sides = 'N', Front = 'N', LSide = 'N', LFront = 'N';
	thridPerson = false;
	RequestQuitGame = false;
	actualOption = 0;
	maxOptions = 1;
	ChangeWeapon = false;
}
KeyHandler::~KeyHandler() {}

void KeyHandler::UpdatePosition(XMFLOAT3 XYZ)
{
	MyPos = { XYZ.x,XYZ.y,XYZ.z };
}
void KeyHandler::UpdatePosition(vec3 XYZ) 
{
	MyPos = XYZ;
}
void KeyHandler::UpdatePosition(float X, float Y, float Z)
{
	MyPos = {X,Y,Z};
}

void KeyHandler::UpdateRotation(XMFLOAT3 XYZ)
{
	MyRot = { XYZ.x,XYZ.y,XYZ.z };
}
void KeyHandler::UpdateRotation(vec3 XYZ)
{
	MyRot = XYZ;
}
void KeyHandler::UpdateRotation(float X, float Y, float Z)
{
	MyRot = { X,Y,Z };
}

void KeyHandler::SetPosition(vec3 pos)
{
	MyPos = pos;
}
vec3 KeyHandler::GetPosition() 
{
	return MyPos;
}

vec3 KeyHandler::GetRotation()
{
	return MyRot;
}

void KeyHandler::Update(int * scene)
{
	Keyboard->Update();
	if (EnableMouse)
	{
		Keyboard->ReadMouse();
		Keyboard->MouseLibreWindow();
		Keyboard->OcultarCursor();
	}

	if (requestBullet)
		DoIntensity();
	switch (*scene)
	{
	case 0:
		KeyEventLevel0(scene);
		return;
	case 1:
		KeyEventMenu(scene);
		return;
	case 2:
		KeyEventLevel1(scene);
		return;
	}
	
	return;
}

void KeyHandler::LookUpward()
{
	// Update the rotation.
	MyRot.x -= LookUD;
	LFront = 'U';
	// Keep the rotation maximum 90 degrees.
	if (MyRot.x > 90.0f)
	{
		MyRot.x = 90.0f;
	}
}

void KeyHandler::LookDownward()
{
	// Update the rotation.
	MyRot.x += LookUD;
	LFront = 'D';
	// Keep the rotation maximum 90 degrees.
	if (MyRot.x < -90.0f)
	{
		MyRot.x = -90.0f;
	}
}

void KeyHandler::LookLeft()
{
	// Update the rotation.
	MyRot.z += LookLR;
	LSide = 'L';
	// Keep the rotation maximum 90 degrees.
	if (MyRot.z > 90.0f)
	{
		MyRot.z = 90.0f;
	}
}

void KeyHandler::LookRight()
{
	// Update the rotation.
	MyRot.z -= LookLR;
	LSide = 'R';
	// Keep the rotation maximum 90 degrees.
	if (MyRot.z < -90.0f)
	{
		MyRot.z = -90.0f;
	}
}

void KeyHandler::FlyUpward()
{
	Fly = 'U';
	// Update the height position.
	MyPos.y += FlyUD;
}

void KeyHandler::FlyDownward()
{
	Fly = 'D';
	// Update the height position.
	MyPos.y -= FlyUD;
}

void KeyHandler::TurnLeft()
{
	Turn = 'L';
	// Update the rotation.
	MyRot.y -= TurnLR;

	// Keep the rotation in the 0 to 360 range.
	if (MyRot.y < 0.0f)
		MyRot.y += 360.0f;
}

void KeyHandler::TurnRight()
{
	Turn = 'R';
	// Update the rotation.
	MyRot.y += TurnLR;

	// Keep the rotation in the 0 to 360 range.
	if (MyRot.y < 360.0f)
		MyRot.y -= 360.0f;
}

void KeyHandler::MoveForward()
{
	if (ResourceManager::Donmove != 'F')
	{
		float radians;
		Front = 'F';
		// Convert degrees to radians.
		radians = MyRot.y * 0.0174532925f;

		// Update the position.
		MyPos.x += sinf(radians) * forceFront;
		MyPos.z += cosf(radians) * forceFront;
		LastKey = 'F';
	}
	else
	{
		ResourceManager::Donmove = 'O';
	}
}

void KeyHandler::MoveBackward()
{
	if (ResourceManager::Donmove != 'B')
	{
		float radians;
		Front = 'B';
		// Convert degrees to radians.
		radians = MyRot.y * 0.0174532925f;

		// Update the position.
		MyPos.x -= sinf(radians) * forceFront;
		MyPos.z -= cosf(radians) * forceFront;
		LastKey = 'B';
	}
	else
	{
		ResourceManager::Donmove = 'O';
	}
}

void KeyHandler::MenuUp()
{
	if (actualOption <= 0)
	{
		actualOption = maxOptions;
	}
	else
		actualOption--;
}

void KeyHandler::MenuDown()
{
	if (actualOption >= 1)
	{
		actualOption = 0;
	}
	else
		actualOption++;
}

void KeyHandler::MoveLeft()
{
	if (ResourceManager::Donmove != 'L')
	{
	float radians;
	Sides = 'L';
	// Convert degrees to radians.
	radians = MyRot.y * 0.0174532925f;

	// Update the position.
	MyPos.x -= sinf(radians + (XM_PI*0.5f)) * forceSide;
	MyPos.z -= cosf(radians + (XM_PI*0.5f)) * forceSide;
	LastKey = 'L';
	}
	else
	{
		ResourceManager::Donmove = 'O';
	}
}

void KeyHandler::MoveRight()
{
	if (ResourceManager::Donmove != 'R')
	{
	float radians;
	Sides = 'R';
	// Convert degrees to radians.
	radians = MyRot.y * 0.0174532925f;

	// Update the position.
	MyPos.x -= sinf(radians - (XM_PI*0.5f)) * forceSide;
	MyPos.z -= cosf(radians - (XM_PI*0.5f)) * forceSide;
	LastKey = 'R';
	}
	else
	{
		ResourceManager::Donmove = 'O';
	}
}

void KeyHandler::KeyEventMenu(int *changeScene)
{
	if (Keyboard->KEYSDOWN[KeyCodes.Enter])
	{
		if (actualOption == 0)
		{
			_RPT0(0, "Se Cambio de escena al menu");
			*changeScene = 2;
			ChangeWeapon = false;
		}
		else if (actualOption = maxOptions)
			RequestQuitGame = true;
	}
	if (Keyboard->KEYSDOWN[KeyCodes.Up])
		MenuUp();
	if (Keyboard->KEYSDOWN[KeyCodes.Down])
		MenuDown();
}

void KeyHandler::KeyEventLevel0(int *changeScene)
{
	bool MouseX = false, Mouse_X = false, MouseY = false, Mouse_Y = false;

	if (EnableMouse)
	{
		if (Keyboard->resultX == 1)
		{
			MouseX = true;
			Mouse_X = false;
		}
		else if (Keyboard->resultX == -1)
		{
			Mouse_X = true;
			MouseX = false;
		}
		else
		{
			MouseX = false;
			Mouse_X = false;
		}

		if (Keyboard->resultY == 1)
		{
			Mouse_Y = true;
			MouseY = false;

		}
		else if (Keyboard->resultY == -1)
		{

			MouseY = true;
			Mouse_Y = false;
		}
		else
		{
			MouseY = false;
			Mouse_Y = false;
		}
	}


	if (Keyboard->KEYSDOWN[KeyCodes.Enter])
	{
		_RPT0(0, "Se Cambio de escena al menu");
		*changeScene = 1;
	}

	//Smoot Movement X
	if (Keyboard->KEYS[KeyCodes.Left] || Keyboard->KEYS[KeyCodes.Right])
	{
		if (forceSide < MaxForceSide)
			forceSide += 0.01f;
	}
	else
	{
		if (forceSide > 0.01)
		{
			if (forceSide > 0.3)
				forceSide -= 0.13f;
			else
				forceSide -= 0.008f;
		}
		else
		{
			Sides = 'N';
		}

		if (Sides == 'L')
			MoveLeft();
		else if (Sides == 'R')
			MoveRight();
	}

	//Smoot Turn X
	if (Keyboard->KEYS[KeyCodes.A] || Keyboard->KEYS[KeyCodes.D] || MouseX || Mouse_X)
	{
		if (TurnLR < TurnLRMax)
			TurnLR += 0.03f;
	}
	else
	{
		if (TurnLR > 0.01)
		{
			if (TurnLR > 0.3)
				TurnLR -= 0.13f;
			else
				TurnLR -= 0.008f;
		}
		else
		{
			Turn = 'N';
		}

		if (Turn == 'L')
			TurnLeft();
		else if (Turn == 'R')
			TurnRight();
	}

	//Smoot Fly Y
	if (Keyboard->KEYS[KeyCodes.Control] || Keyboard->KEYS[KeyCodes.Z])
	{
		if (FlyUD < FlyUDMax)
			FlyUD += 0.002f;
	}
	else
	{
		if (FlyUD > 0.01f)
		{
			if (FlyUD > 0.02)
				FlyUD -= 0.02f;
			else
				FlyUD -= 0.01f;
		}
		else
		{
			Fly = 'N';
		}

		if (Fly == 'U')
			FlyUpward();
		else if (Fly == 'D')
			FlyDownward();
	}

	//Smoot Movement Z
	if (Keyboard->KEYS[KeyCodes.Up] || Keyboard->KEYS[KeyCodes.Down])
	{
		if (forceFront < MaxForceFront)
			forceFront += 0.01f;
	}
	else
	{
		if (forceFront > 0.01)
		{
			if (forceFront > 0.3)
				forceFront -= 0.13f;
			else
				forceFront -= 0.008f;
		}
		else
		{
			Front = 'N';
		}
		if (Front == 'F')
			MoveForward();
		else if (Front == 'B')
			MoveBackward();
	}

	//Smoot Look Z
	if (Keyboard->KEYS[KeyCodes.W] || Keyboard->KEYS[KeyCodes.S] || MouseY || Mouse_Y)
	{
		if (LookUD < LookUDMax)
			LookUD += 0.03f;
	}
	else
	{
		if (LookUD > 0.01)
		{
			if (LookUD > 0.3)
				LookUD -= 0.13f;
			else
				LookUD -= 0.008f;
		}
		else
		{
			LFront = 'N';
		}
		if (LFront == 'U')
			LookUpward();
		else if (LFront == 'D')
			LookDownward();
	}

	if (Keyboard->KEYS[KeyCodes.Left])
		MoveLeft();
	if (Keyboard->KEYS[KeyCodes.Up])
		MoveForward();
	if (Keyboard->KEYS[KeyCodes.Down])
		MoveBackward();
	if (Keyboard->KEYS[KeyCodes.Right])
		MoveRight();
	if (Keyboard->KEYS[KeyCodes.Control])
		FlyUpward();
	if (Keyboard->KEYS[KeyCodes.Z])
		FlyDownward();

	if (Keyboard->KEYS[KeyCodes.Q])
		LookLeft();
	if (Keyboard->KEYS[KeyCodes.A] || Mouse_X)
		TurnLeft();
	if (Keyboard->KEYS[KeyCodes.W] || MouseY)
		LookUpward();
	if (Keyboard->KEYS[KeyCodes.S] || Mouse_Y)
		LookDownward();
	if (Keyboard->KEYS[KeyCodes.D] || MouseX)
		TurnRight();
	if (Keyboard->KEYS[KeyCodes.E])
		LookRight();

	//Change Model Camera
	if (Keyboard->KEYSDOWN[KeyCodes.One])
		thridPerson = false;
	if (Keyboard->KEYSDOWN[KeyCodes.Three])
		thridPerson = true;
	if (Keyboard->KEYSDOWN[KeyCodes.P])
	{
		if (focus == NULL)
			focus = ResourceManager::GetObjectByName("BotEnemy");
		else focus = NULL;
	}

	if (Keyboard->KEYSDOWN[KeyCodes.M])
	{
		EnableMouse = !EnableMouse;
		Keyboard->MostrarCursor();
	}

	if (Keyboard->KEYSDOWN[KeyCodes.Space])
		requestBullet = true;

}

void KeyHandler::KeyEventLevel1(int *changeScene)
{
	bool MouseX = false, Mouse_X = false, MouseY = false, Mouse_Y = false;

	if (EnableMouse)
	{
		if (Keyboard->resultX == 1)
		{
			MouseX = true;
			Mouse_X = false;
		}
		else if (Keyboard->resultX == -1)
		{
			Mouse_X = true;
			MouseX = false;
		}
		else
		{
			MouseX = false;
			Mouse_X = false;
		}

		if (Keyboard->resultY == 1)
		{
			Mouse_Y = true;
			MouseY = false;

		}
		else if (Keyboard->resultY == -1)
		{

			MouseY = true;
			Mouse_Y = false;
		}
		else
		{
			MouseY = false;
			Mouse_Y = false;
		}
	}


	if (Keyboard->KEYSDOWN[KeyCodes.Backspace])
	{
		*changeScene = 1;
	}

	//Smoot Movement X
	if (Keyboard->KEYS[KeyCodes.Left] || Keyboard->KEYS[KeyCodes.Right])
	{
		if (forceSide < MaxForceSide)
			forceSide += 0.01f;
	}
	else
	{
		if (forceSide > 0.01)
		{
			if (forceSide > 0.3)
				forceSide -= 0.13f;
			else
				forceSide -= 0.008f;
		}
		else
		{
			Sides = 'N';
		}

		if (Sides == 'L')
			MoveLeft();
		else if (Sides == 'R')
			MoveRight();
	}

	if (Keyboard->KEYSDOWN[KeyCodes.C])
		ChangeWeapon = true;

	//Smoot Look X
	/*
	if (Keyboard->KEYS[KeyCodes.Q] || Keyboard->KEYS[KeyCodes.E])
	{
		if (LookLR < LookLRMax)
			LookLR += 0.03f;
	}
	else
	{
		if (LookLR > 0.01)
		{
			if (LookLR > 0.3)
				LookLR -= 0.13f;
			else
				LookLR -= 0.008f;
		}
		else
		{
			LSide = 'N';
		}

		if (LSide == 'L')
			LookLeft();
		else if (LSide == 'R')
			LookRight();
	}
	*/

	//Smoot Turn X
	if (Keyboard->KEYS[KeyCodes.A] || Keyboard->KEYS[KeyCodes.D] || MouseX || Mouse_X)
	{
		if (TurnLR < TurnLRMax)
			TurnLR += 0.03f;
	}
	else
	{
		if (TurnLR > 0.01)
		{
			if (TurnLR > 0.3)
				TurnLR -= 0.13f;
			else
				TurnLR -= 0.008f;
		}
		else
		{
			Turn = 'N';
		}

		if (Turn == 'L')
			TurnLeft();
		else if (Turn == 'R')
			TurnRight();
	}

	//Smoot Fly Y
	if (Keyboard->KEYS[KeyCodes.Control] || Keyboard->KEYS[KeyCodes.Z])
	{
		if (FlyUD < FlyUDMax)
			FlyUD += 0.002f;
	}
	else
	{
		if (FlyUD > 0.01f)
		{
			if (FlyUD > 0.02)
				FlyUD -= 0.02f;
			else
				FlyUD -= 0.01f;
		}
		else
		{
			Fly = 'N';
		}

		if (Fly == 'U')
			FlyUpward();
		else if (Fly == 'D')
			FlyDownward();
	}

	//Smoot Movement Z
	if (Keyboard->KEYS[KeyCodes.Up] || Keyboard->KEYS[KeyCodes.Down])
	{
		if (forceFront < MaxForceFront)
			forceFront += 0.01f;
	}
	else
	{
		if (forceFront > 0.01)
		{
			if (forceFront > 0.3)
				forceFront -= 0.13f;
			else
				forceFront -= 0.008f;
		}
		else
		{
			Front = 'N';
		}
		if (Front == 'F')
			MoveForward();
		else if (Front == 'B')
			MoveBackward();
	}

	//Smoot Look Z
	if (Keyboard->KEYS[KeyCodes.W] || Keyboard->KEYS[KeyCodes.S] || MouseY || Mouse_Y)
	{
		if (LookUD < LookUDMax)
			LookUD += 0.03f;
	}
	else
	{
		if (LookUD > 0.01)
		{
			if (LookUD > 0.3)
				LookUD -= 0.13f;
			else
				LookUD -= 0.008f;
		}
		else
		{
			LFront = 'N';
		}
		if (LFront == 'U')
			LookUpward();
		else if (LFront == 'D')
			LookDownward();
	}

	if (/*Keyboard->KEYS[KeyCodes.A] ||*/ Keyboard->KEYS[KeyCodes.Left])
		MoveLeft();
	if (/*Keyboard->KEYS[KeyCodes.W] ||*/ Keyboard->KEYS[KeyCodes.Up])
		MoveForward();
	if (/*Keyboard->KEYS[KeyCodes.S] ||*/ Keyboard->KEYS[KeyCodes.Down])
		MoveBackward();
	if (/*Keyboard->KEYS[KeyCodes.D] ||*/ Keyboard->KEYS[KeyCodes.Right])
		MoveRight();
	if (Keyboard->KEYS[KeyCodes.Control])
		FlyUpward();
	if (Keyboard->KEYS[KeyCodes.Z])
		FlyDownward();

	if (Keyboard->KEYS[KeyCodes.Q])
		LookLeft();
	if (Keyboard->KEYS[KeyCodes.A] || Mouse_X /*|| Keyboard->KEYS[KeyCodes.Left]*/)// || MouseX || Mouse_X
		TurnLeft();
	if (Keyboard->KEYS[KeyCodes.W] || MouseY /*|| Keyboard->KEYS[KeyCodes.Up]*/)//|| MouseY || MouseY
		LookUpward();
	if (Keyboard->KEYS[KeyCodes.S] || Mouse_Y/*|| Keyboard->KEYS[KeyCodes.Down]*/)
		LookDownward();
	if (Keyboard->KEYS[KeyCodes.D] || MouseX/*|| Keyboard->KEYS[KeyCodes.Right]*/)
		TurnRight();
	if (Keyboard->KEYS[KeyCodes.E])
		LookRight();

	//Change Model Camera
	if (Keyboard->KEYSDOWN[KeyCodes.One])
		thridPerson = false;
	if (Keyboard->KEYSDOWN[KeyCodes.Three])
		thridPerson = true;
	if(Keyboard->KEYSDOWN[KeyCodes.P])
	{
		if(focus==NULL)
		focus = ResourceManager::GetObjectByName("BotEnemy");
		else focus = NULL;
	}

	if (Keyboard->KEYSDOWN[KeyCodes.M])
	{
		EnableMouse = !EnableMouse;
		Keyboard->MostrarCursor();
	}

	if (Keyboard->KEYSDOWN[KeyCodes.Space])
		requestBullet = true;

}

bool KeyHandler::ContinuosShoot()
{
	if (Keyboard->KEYS[KeyCodes.K])
		return true;
	else
		return false;
}

bool KeyHandler::ChargeShoot()
{
	if (Keyboard->KEYS[KeyCodes.Space])
	{
		QueueBullet = true;
		CambioEstado = false;
		return true;
	}
	else
	{
		CambioEstado = true;
		return false;
	}
}

void KeyHandler::DoIntensity()
{
	if (requestBullet)
		if (intensity < MaxIntensity)
			intensity += 1;
}

float KeyHandler::GetIntensity()
{
	QueueBullet = false;
	return intensity;
}

void KeyHandler::RestoreIntensity()
{
	intensity = 15;
}

void KeyHandler::LookAtObject(vec3 posPlayer, vec3 focus)
{
	vec3 rot = ResourceManager::LookAt(posPlayer, focus);
	MyRot.y = rot.y - 180;
	MyRot.x = rot.z - 90;
}