#include "stdafx.h"
//SceneMenu Menu Inicial del juego
SceneMenu::SceneMenu(string id):Scene(id)
{
	nameScene = "Scene Menu";
}

SceneMenu::~SceneMenu(){}

//Main instruction to Initialize a Scene
string SceneMenu::CreateScene()
{
	//Load Resources of Scene
	RS = LoadResources();
	//Build GameObjects and define Multitexture or Behaviours to objects
	RS = BuildScene();
	//Define the start points of every object if it needs
	RS = Start();
	if (RS == "S_OK")
		_RPT1(0, "SceneCodeName: %s Created!\n", this->GetNameCodeScene().c_str());
	else
		_RPT1(0, "SceneCodeName: %s cannot be created!\n", this->GetNameCodeScene().c_str());
	return "S_OK";
}

//Load all objects you need 
string SceneMenu::LoadResources()
{
	RB = ResourceManager::AddTexture(id, "assets/Title.png", "TitleTexture");
	RB = ResourceManager::AddTexture(id, "assets/TitleBackground.jpg", "Background");
	RB = ResourceManager::AddTexture(id, "assets/start.png", "start");
	RB = ResourceManager::AddTexture(id, "assets/close.png", "Close");
	RB = ResourceManager::AddTexture(id, "assets/arrow.png", "arrow");
	RB = ResourceManager::AddShader(id, "GUIShader", new GUIShader("GUIShader.vsh", "GUIShader.psh"));
	RB = ResourceManager::AddMaterial(id, "ColorBlanco", vec4(0.2, 0.2, 0.4,1.0));

	if (RB != true)
	{
		_RPT1(0, "SceneCodeName: %s Something went wrong!\n", this->GetNameCodeScene().c_str());
		return "E_Fail";
	}
	else
	{
		_RPT1(0, "SceneCodeName: %s Objects loaded\n", this->GetNameCodeScene().c_str());
		return "S_OK";
	}
}

//Then Build a GameObject or Define a Behaviour of its (Also Multitextures goes here)
string SceneMenu::BuildScene()
{
	
	RS = ResourceManager::BuildGameObject(id, "BitmapTile", "Bitmap00", "TitleTexture", "GUIShader", "ColorBlanco", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapBack", "TitleBit", "Background", "GUIShader", "ColorBlanco", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapStart", "BitmapStart", "start", "GUIShader", "ColorBlanco", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapClose", "BitmapClose", "Close", "GUIShader", "ColorBlanco", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapArrow", "BitmapArrow", "arrow", "GUIShader", "ColorBlanco", "");
	if (RS != "S_OK")
	{
		_RPT1(0, "SceneCodeName: %s Something went wrong!\n", this->GetNameCodeScene().c_str());
		return "S_OK";
	}
	else
	{
		_RPT1(0, "SceneCodeName: %s Objects Builded and correctly defined behaviour\n", this->GetNameCodeScene().c_str());
		return "E_Fail";
	}
}

//Define the first location of EVERY Object
string SceneMenu::Start()
{
	ResourceManager::GetObjectByName("BitmapTile")->GetModel()->SetRect(vec4(0, 100, 150, 150));
	ResourceManager::GetObjectByName("BitmapBack")->GetModel()->SetRect(vec4(0, 0, 800, 600));
	ResourceManager::GetObjectByName("BitmapStart")->GetModel()->SetRect(vec4(0, -100, 100, 100));
	ResourceManager::GetObjectByName("BitmapClose")->GetModel()->SetRect(vec4(0, -200, 100, 100));
	ResourceManager::GetObjectByName("BitmapArrow")->GetModel()->SetRect(vec4(-90, -100, 70, 70));
	return "S_OK";
}

//Then Process all the changes of everything and every object
string SceneMenu::ProcessScene(double dt)
{
	if (ResourceManager::Player1->actualOption == 0)
		ResourceManager::GetObjectByName("BitmapArrow")->GetModel()->SetRect(vec4(-90, -100, 70, 70));
	else
		ResourceManager::GetObjectByName("BitmapArrow")->GetModel()->SetRect(vec4(-90, -220, 70, 70));

	this->ProcesedSucess = "First frame processed sucessful";
	return "S_OK";
}

//Finally Render it
string SceneMenu::RenderScene()
{
	XMMATRIX viewMatrix, *orthoMatrix;
	// Clear the buffers to begin the scene
	specsDx->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	SceneCamera->Watch();
	SceneCamera->GetViewMatrix2D(viewMatrix);
	orthoMatrix = SceneCamera->GetOrthoMatrix();

	GameObject*goTitle = ResourceManager::GetObjectByName("BitmapTile");
	GameObject*goTitleBack = ResourceManager::GetObjectByName("BitmapBack");
	GameObject*goTitleStart = ResourceManager::GetObjectByName("BitmapStart");
	GameObject*goTitleClose = ResourceManager::GetObjectByName("BitmapClose");
	GameObject*goTitleArrow = ResourceManager::GetObjectByName("BitmapArrow");


	specsDx->TurnZBufferOff();
	goTitleBack->Draw(XMMatrixIdentity(), viewMatrix, *orthoMatrix);
	goTitle->Draw(XMMatrixIdentity(), viewMatrix, *orthoMatrix);
	goTitleStart->Draw(XMMatrixIdentity(), viewMatrix, *orthoMatrix);
	goTitleClose->Draw(XMMatrixIdentity(), viewMatrix, *orthoMatrix);
	goTitleArrow->Draw(XMMatrixIdentity(), viewMatrix, *orthoMatrix);
	specsDx->TurnZBufferOn();

	// Present the rendered scene to the screen.
	specsDx->EndScene();
	this->RenderedSucess = "First frame rendered sucessful";
	return "S_OK";
}

//Destroys all the objects that were created
void SceneMenu::DestroyScene()
{
	RB = DownloadResources();
	if (RB == true)
		_RPT1(0, "SceneCodeName: %s Destroyed!\n", this->GetNameCodeScene().c_str());
	else
		_RPT1(0, "SceneCodeName: %s Error when try destroy!\n", this->GetNameCodeScene().c_str());
}

bool SceneMenu::DownloadResources()
{
	bool Sucess = ResourceManager::DestroyItemsFromScene(id);
	if (Sucess == true)
		return true;
	else
		return false;
}
