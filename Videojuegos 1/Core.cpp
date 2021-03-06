// Videojuegos 1.cpp: define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "Core.h"
#include <string>

#define WIN32_LEAN_AND_MEAN				 //Descripción: Mejora el uso de las librerias reduciendo su tamaño

#define MAX_LOADSTRING 100

#define FRAME_RATE 60.0f

// Variables globales:
HINSTANCE hInst;                                // Instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // Nombre de clase de la ventana principal


using namespace std;


// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance, char, int, int);
BOOL                InitInstance(HINSTANCE, int, char, int, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND				hWnd, hwndDx;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int posX = 0, posY = 0;

	if (MessageBoxA(NULL, (LPCSTR) "¿Desea iniciar en pantalla completa?", (LPCSTR) "Videojuegos 1", MB_YESNO + MB_ICONASTERISK) == IDYES)
		ResourceManager::GraphicMode = 'F';
	else
		ResourceManager::GraphicMode = 'W';



	// Inicializar cadenas globales
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VIDEOJUEGOS1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, ResourceManager::GraphicMode, posX, posY);

	// Realizar la inicialización de la aplicación:
	if (!InitInstance(hInstance, nCmdShow, ResourceManager::GraphicMode, posX, posY))
	{
		return FALSE;
	}
	
	
	//Definir bitmpas del sitsema y billboards
	ResourceManager::AddBitmap("Core", "Bitmap00", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap01", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap02", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap03", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap04", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap05", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap06", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap07", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "Bitmap08", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "TitleBit", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "BitmapStart", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "BitmapClose", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "BitmapArrow", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Core", "BulletsBit", vec4(0, 0, 200, 200));
	/*ResourceManager::AddTexture("Core", "assests/Abecedario10x10/A.jpg", "A");
	ResourceManager::AddTexture("Core", "assests/Abecedario10x10/H.jpg", "H");
	ResourceManager::AddTexture("Core", "assests/Abecedario10x10/L.jpg", "L");
	ResourceManager::AddTexture("Core", "assests/Abecedario10x10/O.jpg", "O");*/
	/*ResourceManager::AddBitmap("Bitmap02", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Bitmap03", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Bitmap04", vec4(0, 0, 200, 200));
	ResourceManager::AddBitmap("Bitmap05", vec4(0, 0, 200, 200));*/
	ResourceManager::AddBillboard("Core","Billboard", vec2(1.0f,1.0f),vec2(1.0f,1.0f));

	//Iniciar teclado
	if (ResourceManager::Player1 == nullptr)
		ResourceManager::Player1 = new KeyHandler(vec3(10, 15, 10), vec3(0, 0, 0));

	// Agregar las escenas 
	int SceneIndex=0;
	GameManager* GameGraphics = new GameManager();
	

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VIDEOJUEGOS1));

	MSG msg;




	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	bool Exit = false;
	string FrameResult = "";

	// Bucle principal de mensajes:
	double t = 0.0;
	const double dt = 1.0f / FRAME_RATE;
	double currentTime = (double)std::clock() / (double)CLOCKS_PER_SEC;
	double accumulator = 0.0;

	while (!Exit)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			Exit = true;
		}
		else
		{
			double newTime = (double)std::clock() / (double)CLOCKS_PER_SEC;
			double frameTime = newTime - currentTime;
			currentTime = newTime;

			accumulator += frameTime;
			//Inputs
			if(ResourceManager::Player1!=nullptr)
				ResourceManager::Player1->Update(&SceneIndex);
			//Rutina para saber si el usuario queria salir
			if (ResourceManager::Player1->RequestQuitGame != true)
			{
				if (SceneIndex != GameGraphics->GetCurrentIdScene())
				{
					GameGraphics->ChangeScene(SceneIndex);
				}
				while (accumulator >= dt)
				{
					FrameResult = GameGraphics->FrameProcess(dt);
					//integrate(state, t, dt);
					accumulator -= dt;
					t += dt;
				}
				FrameResult = GameGraphics->FrameRender();
				if (FrameResult != "S_OK")
				{
					Exit = true;
				}
			}
			else
				Exit = true;
		}
	}
	GameGraphics->Shutdown();
	ResourceManager::Shutdown();
	return (int)msg.wParam;
}

//  FUNCIÓN: MyRegisterClass()
//  PROPÓSITO: registrar la clase de ventana.
ATOM MyRegisterClass(HINSTANCE hInstance, char GraphicMode, int posX, int posY)
{
	WNDCLASSEXW wcex;
	DEVMODE dmScreenSettings;


	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VIDEOJUEGOS1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VIDEOJUEGOS1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.cbSize = sizeof(WNDCLASSEX);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (GraphicMode == 'F')
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)ResourceManager::ScreenWidthF;
		dmScreenSettings.dmPelsHeight = (unsigned long)ResourceManager::ScreenHeightF;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - ResourceManager::ScreenWidthF) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - ResourceManager::ScreenHeightF) / 2;
	}

	return RegisterClassExW(&wcex);

}

//   FUNCIÓN: InitInstance(HINSTANCE, int)
//   PROPÓSITO: guardar el identificador de instancia y crear la ventana principal
//   COMENTARIOS:
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, char GraphicMode, int posX, int posY)
{
	hInst = hInstance; // Almacenar identificador de instancia en una variable global
	SpecsDx* info;
	if (GraphicMode == 'F')
	{
		hWnd = CreateWindowW(szWindowClass, szTitle, WS_EX_TOPMOST | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, ResourceManager::ScreenWidthF, ResourceManager::ScreenHeightF, nullptr, nullptr, hInstance, nullptr);
		info = SpecsDx::Initialize(ResourceManager::ScreenWidthF, ResourceManager::ScreenHeightF, hWnd, true);
	}

	else
	{
		hWnd = CreateWindowW(szWindowClass, szTitle, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPEDWINDOW,
			posX, posY, ResourceManager::ScreenWidth, ResourceManager::ScreenHeight, nullptr, nullptr, hInstance, nullptr);
		info = SpecsDx::Initialize(ResourceManager::ScreenWidth, ResourceManager::ScreenHeight, hWnd, false);
	}

	//Ventana preconstruida para separar de winapi a directx
	//hwndDx = CreateWindowW( L"STATIC",
	// NULL,
	// WS_VISIBLE | WS_CHILD ,
	// 0, 0,
	// ResourceManager::ScreenWidth, ResourceManager::ScreenHeight,
	// hWnd, nullptr, hInstance, nullptr);

	if (info == nullptr)
		_RPT0(0, "Directx doesn't initialized\n");

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PROPÓSITO:  procesar mensajes de la ventana principal.
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	string infoDx;
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analizar las selecciones de menú:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Agregar cualquier código de dibujo que use hDC aquí...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
