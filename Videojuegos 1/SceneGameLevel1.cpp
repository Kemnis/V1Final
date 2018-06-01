#include "stdafx.h"

SceneGameLevel1::SceneGameLevel1(string id):Scene(id)
{
	nameScene = "Scene Level1";
}
SceneGameLevel1::~SceneGameLevel1()
{

}

//Main instruction to Initialize a Scene
string SceneGameLevel1::CreateScene()
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
string SceneGameLevel1::LoadResources()
{
	Totalbillboards = 10;
	rangeYBillboards = vec2(80, 250);
	rangeXBillboards = vec2(100, 700);
	rangeZBillboards = vec2(100, 700);
	CalculateAllPositionsOfBillboards();
	RB = ResourceManager::AddModel(id, "assets/LowPolyPlayer.obj", "nave");
	RB = ResourceManager::AddTexture(id, "assets/player.jpg", "TextureNave");

	RB = ResourceManager::AddModel(id, "assets/LowPolyEnemy.obj", "meshEnemy");
	RB = ResourceManager::AddTexture(id, "assets/enemy.jpg", "TextureEnemy");


	RB = ResourceManager::AddModel(id, "SphereCollider", "SphereColi");
	RB = ResourceManager::AddModel(id, "Sphere", "SphereMesh");
	RB = ResourceManager::AddModel(id, "assets/Sphere.obj", "SphereModel");

	RB = ResourceManager::AddModel(id, "Sphere", "Patrol");
	RB = ResourceManager::AddTexture(id, "assets/Star.png", "Star");
	RB = ResourceManager::AddTexture(id, "assets/Star2.png", "Star2");

	RB = ResourceManager::AddStage(id, "assets/Stage1.bmp", "Stage1", 256, 1024, 1024);
	RB = ResourceManager::AddTexture(id, "assets/SkyStage1Day.jpg", "World");
	RB = ResourceManager::AddTexture(id, "assets/SkyStage1Night.jpg", "WorldNight");
	RB = ResourceManager::AddTexture(id, "assets/mt1Stage1.jpg", "Layer1-Bottom");
	RB = ResourceManager::AddTexture(id, "assets/mt2Stage1.jpg", "Layer2-Mid");
	RB = ResourceManager::AddTexture(id, "assets/mt3Stage1.jpg", "Layer3-Top");

	RB = ResourceManager::AddMaterial(id, "ColorBlanco", vec4(0.2, 0.2, 0.4,1.0));

	RB = ResourceManager::AddLight(id, "Luz", vec4(0.1f, 0.8f, 0.8f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f));
	RB = ResourceManager::AddShader(id, "LambertMaterialShader", new MaterialShader("LambertTexture.vs", "LambertTexture.ps"));
	RB = ResourceManager::AddShader(id, "LambertLMaterialShader", new MaterialShader("LambertLTexture.vs", "LambertLTexture.ps", 1));

	RB = ResourceManager::AddShader(id, "MaterialPatrol", new MaterialShader("LambertMaterial.vs", "LambertMaterial.ps"));

	RB = ResourceManager::AddShader(id, "GUIShader", new GUIShader("GUIShader.vsh", "GUIShader.psh"));
	RB = ResourceManager::AddShader(id, "ShaderCollider", new MaterialShader("LambertLMaterial.vs", "LambertLMaterial.ps"));

 	RB = ResourceManager::AddShader(id, "SkydomeShader", new SkydomeShader("Skydome.vs", "Skydome.ps", 1));
	RB = ResourceManager::AddShader(id, "TerrenoShader", new TerrainShader("Terrain.vs", "Terrain.ps"));
	//texture Bitmaps
	RB = ResourceManager::AddTexture(id, "assets/LifeIco.png", "TextureCorazon");
	RB = ResourceManager::AddTexture(id, "assets/LifesIco.png", "TextureVidas");
	RB = ResourceManager::AddTexture(id, "assets/Asteroid1Tex.jpg", "Asteriode_1");

	RB = ResourceManager::AddMaterial(id, "ColorAsteriode", vec4(1.0, 1.0, 1.0, 1.0));

	RB = ResourceManager::AddMaterial(id, "ColorMaterialCheckPoint", vec4(0.0, 0.0, 1.0, 0.2));
	RB = ResourceManager::AddMaterial(id, "ColorCollider", vec4(1.0, 0.0, 0.0, 0.5));
	RB = ResourceManager::AddMaterial(id, "ColorDamage", vec4(1.0, 0.0, 0.0, 0.3));
	RB = ResourceManager::AddMaterial(id, "ColorBitmap", vec4(1.0, 1.0, 1.0, 1.0));
	RB = ResourceManager::AddMaterial(id, "ColorBitmapTrans", vec4(1.0, 1.0, 1.0, 0.1));
	RB = ResourceManager::AddMaterial(id, "ColorDamageBitmap", vec4(1.0, 0.0, 0.0, 1.0));
	RB = ResourceManager::AddMaterial(id, "colorBillboard", vec4(1.0, 1.0, 1.0, 1.0));


	//RB = ResourceManager::AddModel(id, "assets/Bullet.obj", "meshBullet");
	RB = ResourceManager::AddTexture(id, "assets/Bullet1Tex.jpg", "TextureBullet");
	RB = ResourceManager::AddMaterial(id, "ColorBullet", vec4(1.0, 1.0, 1.0, 1.0));
	RB = ResourceManager::AddTexture(id, "assets/FisrtAidTex.jpg", "TextureLifes");
	RB = ResourceManager::AddTexture(id, "assets/Meta.jpg", "TextureMeta");

	RB = ResourceManager::AddTexture(id, "assets/win.png", "TextureWin");
	RB = ResourceManager::AddTexture(id, "assets/over.png", "TextureLoser");
	
	
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
string SceneGameLevel1::BuildScene()
{

	RS = ResourceManager::BuildGameObject(id, "NaveJugador", "nave", "TextureNave", "LambertMaterialShader", "ColorCollider", "Luz");

	RS = ResourceManager::BuildGameObject(id, "BotEnemy", "meshEnemy", "TextureEnemy", "LambertMaterialShader", "ColorBlanco", "Luz");
	RS = ResourceManager::BuildGameObject(id, "SphereMes", "SphereMesh", "World", "LambertLMaterialShader", "ColorBlanco", "Luz");
	RS = ResourceManager::BuildGameObject(id, "SphereMod", "SphereModel", "World", "SkydomeShader", "ColorBlanco", "Luz");
	RS = ResourceManager::BuildGameObject(id, "Stage1", "Stage1", "", "TerrenoShader", "ColorBlanco", "Luz");


	RS = ResourceManager::BuildGameObject(id, "Patrols", "Patrol", "", "MaterialPatrol", "ColorMaterialCheckPoint", "");
	RS = ResourceManager::BuildGameObject(id, "Colision", "SphereColi", "", "ShaderCollider", "ColorCollider", "");

	RS = ResourceManager::BuildGameObject(id, "Stars", "Billboard", "Star", "LambertMaterialShader", "ColorBlanco", "");
	RS = ResourceManager::BuildGameObject(id, "Stars2", "Billboard", "Star2", "LambertMaterialShader", "ColorBlanco", "");

	RS = ResourceManager::BuildGameObject(id, "BitmapCorazon01", "Bitmap01", "TextureCorazon", "GUIShader", "ColorDamageBitmap", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapCorazon02", "Bitmap02", "TextureCorazon", "GUIShader", "ColorDamageBitmap", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapCorazon03", "Bitmap03", "TextureCorazon", "GUIShader", "ColorDamageBitmap", "");

	RS = ResourceManager::BuildGameObject(id, "BitmapVidas01", "Bitmap04", "TextureVidas", "GUIShader", "ColorDamageBitmap", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapVidas02", "Bitmap05", "TextureVidas", "GUIShader", "ColorDamageBitmap", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapVidas03", "Bitmap06", "TextureVidas", "GUIShader", "ColorDamageBitmap", "");
	
	RS = ResourceManager::BuildGameObject(id, "GO_Asteriode", "SphereMesh", "Asteriode_1", "LambertMaterialShader", "ColorAsteriode", "Luz");
	RS = ResourceManager::BuildGameObject(id, "GO_Bullet", "SphereMesh", "TextureBullet", "LambertMaterialShader", "ColorBullet", "Luz");

	RS = ResourceManager::BuildGameObject(id, "GO_lifes", "Billboard", "TextureLifes", "LambertMaterialShader", "colorBillboard", "Luz");
	RS = ResourceManager::BuildGameObject(id, "GO_meta", "Billboard", "TextureMeta", "LambertMaterialShader", "colorBillboard", "Luz");

	RS = ResourceManager::BuildGameObject(id, "BitmapLoser", "Bitmap07", "TextureLoser", "GUIShader", "ColorBitmapTrans", "");
	RS = ResourceManager::BuildGameObject(id, "BitmapWin", "Bitmap08", "TextureWin", "GUIShader", "ColorBitmapTrans", "");



	//Addtexture
	RS = ResourceManager::AsingTextureToGameObject("SphereMod", "WorldNight");
	RS = ResourceManager::AsingTextureToGameObject("Stage1", "Layer1-Bottom");
	RS = ResourceManager::AsingTextureToGameObject("Stage1", "Layer2-Mid");
	RS = ResourceManager::AsingTextureToGameObject("Stage1", "Layer3-Top");

	GameObject*GoBullet = ResourceManager::GetObjectByName("GO_Bullet");
	GoBullet->Transform->SetScale(vec3(0.3, 0.3, 0.3));
	
	char mode = ResourceManager::GraphicMode;
	int AuxWidth;
	int AuxHeight;
	if (mode == 'W') {
		AuxWidth = -1 * (ResourceManager::ScreenWidth / 2 - 27);
		AuxHeight = ResourceManager::ScreenHeight / 2;
	}
	else
	{
		AuxWidth = -1 * (ResourceManager::ScreenWidthF / 2 - 27);
		AuxHeight = ResourceManager::ScreenHeightF / 2;
	}

	GameObject* loser =  ResourceManager::GetObjectByName("BitmapLoser");
	loser->GetModel()->SetRect(vec4(0,0, -1*AuxWidth, AuxHeight));
	GameObject* win  = ResourceManager::GetObjectByName("BitmapWin");
	win->GetModel()->SetRect(vec4(0, 0, -1 * AuxWidth, AuxHeight));
	ResourceManager::GetMaterial("ColorBitmapTrans")->escalar = 1;

	List_Corazones.push_back(ResourceManager::GetObjectByName("BitmapCorazon01"));
	List_Corazones.push_back(ResourceManager::GetObjectByName("BitmapCorazon02"));
	List_Corazones.push_back(ResourceManager::GetObjectByName("BitmapCorazon03"));

	List_Vidas.push_back(ResourceManager::GetObjectByName("BitmapVidas01"));
	List_Vidas.push_back(ResourceManager::GetObjectByName("BitmapVidas02"));
	List_Vidas.push_back(ResourceManager::GetObjectByName("BitmapVidas03"));
	for (int i = 0; i < List_Corazones.size(); i++)
	{
		GameObject*temp = List_Corazones.at(i);
		temp->GetModel()->SetRect(vec4(AuxWidth + i * 51, AuxHeight - 55, 50, 50));
	}



	for (int i = 0; i < List_Vidas.size(); i++)
	{
		GameObject*temp = List_Vidas.at(i);

		temp->GetModel()->SetRect(vec4(AuxWidth + i * 51, AuxHeight - 110, 60, 50));
	}

	GameObject*terrain = ResourceManager::GetObjectByName("Stage1");
	GameObject*asteriode = ResourceManager::GetObjectByName("GO_Asteriode");
	asteriode->Transform->SetScale(vec3(3.0, 3.0, 3.0));
	int Width = terrain->GetModel()->sizeTerrain.x;
	int Hieght = terrain->GetModel()->sizeTerrain.y;

	for (int i = 0;i<5;i++)
	{
		List_Asteroid.push_back(
			new ActionGravity(
				vec2((float)Width, 0),
				vec2(100, 100),
				vec2((float)Hieght, 0),
				vec4(10, 10, 10, 10),
				asteriode->GetModel()->GetRadio(),
				asteriode->Transform
			));
	}

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
string SceneGameLevel1::Start()
{
	AssaultBullet = false;
	float height = ResourceManager::GetObjectByName("Stage1")->GetModel()->GetPositionHeightMap(vec3(35.0f, 0.0f, 35.0f));
	vector<vec3> Posiciones;
	Posiciones.push_back(vec3(20.0f, height + 4.0f, 20.0f));
	Posiciones.push_back(vec3(40.0f, height + 4.0f, 45.0f));
	Posiciones.push_back(vec3(60.0f, height + 4.0f, 40.0f));
	Posiciones.push_back(vec3(80.0f, height + 4.0f, 75.0f));
	ResourceManager::GetObjectByName("NaveJugador")->Transform->SetScale(vec3(0.1f, 0.1f, 0.1f));
	ResourceManager::GetObjectByName("SphereMod")->Transform->SetRotation(vec3(180,0,0));
	ResourceManager::GetObjectByName("BotEnemy")->Transform->SetTranslation(vec3(50.0f, height + 4.0f, 50.0f));
	ResourceManager::DefinePositionPatrol(Posiciones);
	ResourceManager::GetObjectByName("BotEnemy")->Transform->SetScale(vec3(0.1f, 0.1f, 0.1f));
	ResourceManager::StartEnemy(1, 1, "Patrol", 1);
	ResourceManager::DefineTargetToFight("NaveJugador");
	ResourceManager::GetMyBody("BotEnemy");
	ResourceManager::GetObjectByName("Patrols")->Transform->SetTranslation(vec3(20.0f, height + 4.0f, 20.0f));
	ResourceManager::GetObjectByName("Patrols")->IsPatrol(Posiciones);
	ResourceManager::GetObjectByName("Stars")->Transform->SetScale(vec3(14, 14, 14));
	ResourceManager::GetObjectByName("Stars2")->Transform->SetScale(vec3(14, 14, 14));

	return "S_OK";
}

//Then Process all the changes of everything and every object
string SceneGameLevel1::ProcessScene(double dt)
{
	vec3 rot;
	rot.y = dt;
	ResourceManager::GetObjectByName("SphereMod")->Transform->Rotate(rot);
	GameObject*GoPlayer = ResourceManager::GetObjectByName("NaveJugador");
	GameObject*GOEnemy = ResourceManager::GetObjectByName("BotEnemy");
	if (ResourceManager::Player1->thridPerson)
		GoPlayer->Transform->SetTranslation(vec3(SceneCamera->view.x, SceneCamera->view.y, SceneCamera->view.z));
	else
		GoPlayer->Transform->SetTranslation(ResourceManager::Player1->GetPosition());
	GoPlayer->Transform->SetRotation(vec3(0.0f, ResourceManager::Player1->GetRotation().y - 90, 0.0f));

	//Skydome
	static float ambiente = 195;
	ResourceManager::GetMaterial("ColorBlanco")->escalar = sinf(ambiente*0.0174532925f);
	if (ambiente < 80)
	{
		GoPlayer->CountCorazon = -1;
		loser = true;
	}
	ambiente -= dt * 0.5f;

	//Focus GameObject
	if (ResourceManager::Player1->focus)
	{
		//Calcular angulo
		vec3 posGo = ResourceManager::Player1->focus->Transform->GetTranslation();
		vec3 posCam = vec3(SceneCamera->view.x, SceneCamera->view.y, SceneCamera->view.z);
		ResourceManager::Player1->LookAtObject(posCam, posGo);
	}
	if (!ResourceManager::Player1->ChargeShoot() && ResourceManager::Player1->QueueBullet == true && ResourceManager::Player1->CambioEstado == true)
	{
		GameObject*GoBullet = ResourceManager::GetObjectByName("GO_Bullet");
		CreateShot(GoBullet, ResourceManager::Player1->GetRotation(), ResourceManager::Player1->GetIntensity());
		ResourceManager::Player1->requestBullet = false;
		ResourceManager::Player1->RestoreIntensity();
	}
	if (ResourceManager::Player1->ContinuosShoot())
	{
		GameObject*GoBullet = ResourceManager::GetObjectByName("GO_Bullet");
		CreateShot(GoBullet, ResourceManager::Player1->GetRotation(),ResourceManager::Player1->GetIntensity());
	}

	ResourceManager::UpdateEnemy();
	GameObject*terrain = ResourceManager::GetObjectByName("Stage1");
	UpdateAsteriods(terrain, GoPlayer, dt);
	UpdateShot(terrain, dt, 9.8);
	UpdateItems(terrain, GoPlayer, dt);
	GoPlayer->Collider(dt);
	GoPlayer->Collider(GOEnemy, dt, SceneCamera);


	if (GoPlayer->CountLifes == -1)
	{
		loser = true;
	}

	if (GoPlayer->CountCorazon == 1)
	{
		GameObject*temp = List_Corazones.at(0);
		temp->AnimationDamage(dt, 0.5);
	}

	ResourceManager::UpdateEnemy();
	this->ProcesedSucess = "First frame processed sucessful";
	return "S_OK";
}

//Finally Render it
string SceneGameLevel1::RenderScene()
{
	vec3 posiciones[5];
	XMMATRIX viewMatrix, viewMatrix2D, *orthoMatrix,*projectionMatrix, Patrolmatrix;
	
	// Clear the buffers to begin the scene
	specsDx->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	SceneCamera->Watch();
	SceneCamera->GetViewMatrix(viewMatrix);
	SceneCamera->GetViewMatrix2D(viewMatrix2D);
	orthoMatrix = SceneCamera->GetOrthoMatrix();
	projectionMatrix = SceneCamera->GetProjectionMatrix();

	//Fijar el Skydome a la camrara
	ResourceManager::GetObjectByName("SphereMod")->Transform->SetTranslation(vec3(SceneCamera->pos.x, SceneCamera->pos.y, SceneCamera->pos.z));

	GameObject* GoStage = ResourceManager::GetObjectByName("Stage1");
	GameObject* GObjModel = ResourceManager::GetObjectByName("SphereMod");
	GameObject* GOjugador = ResourceManager::GetObjectByName("NaveJugador");
	GameObject* GOPatrol = ResourceManager::GetObjectByName("Patrols");


	GameObject*GOEnemy = ResourceManager::GetObjectByName("BotEnemy");
	GameObject*sphereColision = ResourceManager::GetObjectByName("Colision");
	GameObject*asteriodes = ResourceManager::GetObjectByName("GO_Asteriode");
	GameObject*GoBullet = ResourceManager::GetObjectByName("GO_Bullet");

	specsDx->TurnOnAlphaBlending();
	specsDx->TurnOffCulling();
		specsDx->TurnZBufferOff();
			GObjModel->Draw(GObjModel->Transform->ToMatrix(), viewMatrix, *projectionMatrix);
		specsDx->TurnZBufferOn();
	specsDx->TurnOnCulling();

	GoStage->Draw(GoStage->Transform->ToMatrix(),viewMatrix,*projectionMatrix);

	

	GOEnemy->Draw(GOEnemy->Transform->ToMatrix(), viewMatrix, *projectionMatrix);
	DrawSphereColsion(sphereColision, GOEnemy, viewMatrix, *projectionMatrix);
	

	specsDx->TurnOffCulling();
		specsDx->TurnZBufferOff();
			GOPatrol->DrawPatrol(viewMatrix, *projectionMatrix);
		specsDx->TurnZBufferOn();
	specsDx->TurnOnCulling();
	//Draw Bullets and Asteriods
	specsDx->TurnOffCulling();
		specsDx->TurnZBufferOff();
			DrawsAsteriods(asteriodes, viewMatrix, *projectionMatrix);
		specsDx->TurnZBufferOn();
	specsDx->TurnOnCulling();

	BulletsIndicator();
	BulletsChangeType();


	DrawsBullets(GoBullet, viewMatrix, *projectionMatrix);
	DrawsItems(viewMatrix, *projectionMatrix);

	ConvertBillPosToGameObjectAndDraw(ResourceManager::GetObjectByName("Stars"), ResourceManager::GetObjectByName("Stars2"), viewMatrix, projectionMatrix);

	GOjugador->Draw(GOjugador->Transform->ToMatrix(), viewMatrix, *projectionMatrix);
	DrawSphereColsion(sphereColision, GOjugador, viewMatrix, *projectionMatrix);

	specsDx->TurnZBufferOff();

		for (int i = 0; i < GOjugador->CountCorazon; i++)
		{
			GameObject*temp = List_Corazones.at(i);
			temp->Draw(XMMatrixIdentity(), viewMatrix2D, *orthoMatrix);
		}

		for (int i = 0; i < GOjugador->CountLifes; i++)
		{
			GameObject*temp = List_Vidas.at(i);
			temp->Draw(XMMatrixIdentity(), viewMatrix2D, *orthoMatrix);
		}

		if (win)
		{
			GameObject* winGo = ResourceManager::GetObjectByName("BitmapWin");
			winGo->Draw(XMMatrixIdentity(), viewMatrix2D, *orthoMatrix);
		}
		if (loser)
		{
			GameObject* loserGo = ResourceManager::GetObjectByName("BitmapLoser");
			loserGo->Draw(XMMatrixIdentity(), viewMatrix2D, *orthoMatrix);
		}

	specsDx->TurnZBufferOn();

	// Present the rendered scene to the screen.
	specsDx->EndScene();
	this->RenderedSucess = "First frame rendered sucessful";
	return "S_OK";
}

//Destroys all the objects that were created
void SceneGameLevel1::DestroyScene()
{
	List_Corazones.clear();
	List_Vidas.clear();
	List_Asteroid.clear();
	RB = DownloadResources();
	if (RB == true)
		_RPT1(0, "SceneCodeName: %s Destroyed!\n", this->GetNameCodeScene().c_str());
	else
		_RPT1(0, "SceneCodeName: %s Error when try destroy!\n", this->GetNameCodeScene().c_str());
}

bool SceneGameLevel1::DownloadResources()
{
	bool Sucess = ResourceManager::DestroyItemsFromScene(id);
	if (Sucess == true)
		return true;
	else
		return false;
}

void SceneGameLevel1::BulletsIndicator() {
	if (AssaultBullet == true)
	{
		
	}
	else
	{
	}
}

void SceneGameLevel1::BulletsChangeType() {
	if (ResourceManager::Player1->ChangeWeapon)
		AssaultBullet = true;
	else
		AssaultBullet = false;
}

void SceneGameLevel1::UpdateAsteriods(GameObject*terrain,GameObject*go,double dt)
{
	for (int i = 0;i <List_Asteroid.size();i++)
	{
		ActionObject*gravity = List_Asteroid.at(i);
		gravity->Update(dt);

		if (terrain->GetColision()->ColsionTerrain(terrain->GetModel(),gravity->transform))
		{
			gravity->death = true;
		}
		
		if (go->GetColision()->ColisionSphere(go->Transform, gravity->transform))
		{
			if (go->DamageColision())
				ResourceManager::Player1->SetPosition(go->RespawnPlayer(terrain->GetModel()));
			gravity->destroy = true;
		}


		if (gravity->destroy)
		{
			gravity->Shutdown();
			List_Asteroid.erase(List_Asteroid.begin() +i);
			GameObject*asteriode = ResourceManager::GetObjectByName("GO_Asteriode");
			int Width = terrain->GetModel()->sizeTerrain.x;
			int Hieght = terrain->GetModel()->sizeTerrain.y;
			List_Asteroid.push_back(
				new ActionGravity(
					vec2((float)Width, 0),
					vec2(100, 100),
					vec2((float)Hieght, 0),
					vec4(10, 10, 10, 10),
					asteriode->GetModel()->GetRadio(),
					asteriode->Transform
				));
		}
	}

}

void SceneGameLevel1::DrawsAsteriods(GameObject*go,XMMATRIX view, XMMATRIX proj)
{
	for (int i = 0;i <List_Asteroid.size();i++)
	{
		ActionObject*gravity = List_Asteroid.at(i);
		go->Draw(gravity->transform->ToMatrix(),view,proj);
	}
}

void SceneGameLevel1::CreateShot(GameObject*bullet, vec3 rot, float intensity)
{
	List_Bullet.push_back
	(
		new ActionShot
		(
			vec3(SceneCamera->view.x, SceneCamera->view.y, SceneCamera->view.z),
			rot,
			bullet->GetModel()->GetRadio(),
			bullet->Transform,
			intensity
		)
	);
}
void SceneGameLevel1::UpdateShot(GameObject*terrain, double dt,float gravityforce)
{
	for (int i = 0;i <List_Bullet.size();i++)
	{
		ActionObject*bullet = List_Bullet.at(i);
		bullet->Update(dt);

		if (terrain->GetColision()->ColsionTerrain(terrain->GetModel(), bullet->transform) || 
			bullet->transform->GetTranslation().x > terrain->GetModel()->Heightmap.width  ||
			bullet->transform->GetTranslation().x < 0 ||
			bullet->transform->GetTranslation().z > terrain->GetModel()->Heightmap.height ||
			bullet->transform->GetTranslation().z < 0 ||
			bullet->transform->GetTranslation().y > 300 ||
			bullet->transform->GetTranslation().y < -10)
		{
			bullet->death = true;
		}
		if (bullet->death == false)
		{
			//Tiro Parabolico
			vec3 Parabolico = bullet->transform->GetTranslation();
			Parabolico.y = Parabolico.y - (gravityforce * dt * bullet->ciclos);
			bullet->ciclos += .01;
			bullet->transform->SetTranslation(Parabolico);
		}

		for (int j = 0;j < List_Asteroid.size();j++)
		{
			ActionObject*gravity = List_Asteroid.at(j);
			if (terrain->GetColision()->ColisionSphere(gravity->transform,bullet->transform))
			{
				GameObject*goBill = ResourceManager::GetObjectByName("GO_meta");
				CreateItems(goBill, gravity->transform->GetTranslation());
				gravity->Shutdown();
				List_Asteroid.erase(List_Asteroid.begin() + j);
				GameObject*asteriode = ResourceManager::GetObjectByName("GO_Asteriode");
				int Width = terrain->GetModel()->sizeTerrain.x;
				int Hieght = terrain->GetModel()->sizeTerrain.y;
				List_Asteroid.push_back(
					new ActionGravity(
						vec2((float)Width, 0),
						vec2(100, 100),
						vec2((float)Hieght, 0),
						vec4(10, 10, 10, 10),
						asteriode->GetModel()->GetRadio(),
						asteriode->Transform
					));
				bullet->destroy = true;
				
			}
		}
		
		if (bullet->destroy)
		{
			bullet->Shutdown();
			List_Bullet.erase(List_Bullet.begin() + i);
		}
	}
}
void SceneGameLevel1::DrawsBullets(GameObject*go, XMMATRIX view, XMMATRIX proj)
{
	for (int i = 0;i <List_Bullet.size();i++)
	{
		ActionObject*bullet = List_Bullet.at(i);
		go->Draw(bullet->transform->ToMatrix(), view, proj);
	}
}

void SceneGameLevel1::CreateItems(GameObject*go, vec3 pos)
{
	List_BillboardItems.push_back(
									new ActionBillboard(pos,go->GetModel()->GetRadio(),go->Transform)
	);
}
void SceneGameLevel1::UpdateItems(GameObject*terrain,GameObject*go, double dt)
{
	for (int i = 0;i <List_BillboardItems.size();i++)
	{
		ActionObject*bill = List_BillboardItems.at(i);
		bill->Update(dt);

		if (terrain->GetColision()->ColsionTerrain(terrain->GetModel(), bill->transform))
		{
			bill->death = true;
		}

		if (go->GetColision()->ColisionSphere(go->Transform, bill->transform))
		{
			bill->destroy = true;
			if (bill->item == 5 || bill->item == 8)
				win = true;
			else
			{
				if(go->CountCorazon<3)
				go->CountCorazon++;
			}
		}


		if (bill->destroy)
		{
			bill->Shutdown();
			List_BillboardItems.erase(List_BillboardItems.begin() + i);
		}
	}
}
void SceneGameLevel1::DrawsItems( XMMATRIX view, XMMATRIX proj)
{
	GameObject*item2 = ResourceManager::GetObjectByName("GO_lifes");
	GameObject*item = ResourceManager::GetObjectByName("GO_meta");
	for (int i = 0;i <List_BillboardItems.size();i++)
	{
		ActionObject*bill = List_BillboardItems.at(i);
		
		if (bill->item == 5 || bill->item == 8)
		{
			item->Draw(bill->transform->ToMatrix(),view,proj);
		}
		else
		{
			item2->Draw(bill->transform->ToMatrix(), view, proj);
		}
	}


}


void SceneGameLevel1::CalculateAllPositionsOfBillboards() {
	for (int i = 0; i < Totalbillboards; i++)
	{
		vec3 temp = vec3(((rand() % (int)rangeXBillboards.y) - rangeXBillboards.x), (rand() % (int)rangeYBillboards.y + rangeYBillboards.x), ((rand() % (int)rangeZBillboards.y) - rangeZBillboards.x));
		int mesh = rand() % 3;
		PosBill.push_back(vec4(temp.x, temp.y, temp.z, mesh));
	}
}

void SceneGameLevel1::ConvertBillPosToGameObjectAndDraw(GameObject* GOStars, GameObject* GOStars2, XMMATRIX view, XMMATRIX* projectionMatrix)
{
	for (int i = 0; i < Totalbillboards; i++)
	{
		if (PosBill.at(i).w == 1)
		{
			GOStars->Transform->LookCamera(vec3(SceneCamera->view.x, SceneCamera->view.y, SceneCamera->view.z));
			GOStars->Transform->SetTranslation(vec3(PosBill.at(i).x, PosBill.at(i).y, PosBill.at(i).z)/*vec3(4+i,10+i,4+i)*/);
			GOStars->Draw(GOStars->Transform->ToMatrix(), view, *projectionMatrix);
		}
		else
		{
			GOStars2->Transform->LookCamera(vec3(SceneCamera->view.x, SceneCamera->view.y, SceneCamera->view.z));
			GOStars2->Transform->SetTranslation(vec3(PosBill.at(i).x, PosBill.at(i).y, PosBill.at(i).z)/*vec3(4+i,10+i,4+i)*/);
			GOStars2->Draw(GOStars2->Transform->ToMatrix(), view, *projectionMatrix);
		}
	}
}