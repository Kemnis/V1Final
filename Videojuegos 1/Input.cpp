#include "stdafx.h"

Input::Input()
{
	scroll = 0;
}

Input::~Input() {}

bool Input::Initialize(int screenWidth, int screenHeight)
{
	HRESULT result;
	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseXOld = 0;
	m_mouseYOld = 0;
	for (int i = 0; i<256; i++) {
		KEYS[i] = 0;
		KEYSoldDOWN[i] = false;
		KEYSDOWN[i] = false;
	}

	return true;
}

bool Input::Update()
{
	BOOL result;
	//Este arreglo tiene que tecla se presiono
	char keystate[256];
	//Instanciar el mouse con esta funcion
	result = GetKeyboardState((PBYTE)keystate);

	//Si manda 0 hay un error en el teclado
	if (result = 0)
		return false;

	//Pasar el buffer del teclado a un arreglo boleano
	//for (int i = 0; i<256; i++) {
	//	KEYS[i] = keystate[i] & 0x8000;
	//}

	for (int i = 0; i<256; i++) {
		KEYS[i] = keystate[i] & 0x8000;

		if (KEYS[i] && KEYSDOWN[i] == 0 && KEYSoldDOWN[i] == 0)
		{
			KEYSDOWN[i] = KEYS[i];
			KEYSoldDOWN[i] = KEYS[i];
		}
		else if (KEYS[i] && KEYSDOWN[i] && KEYSoldDOWN[i])
		{
			KEYSDOWN[i] = 0;
		}
		else if (KEYS[i] == 0 && KEYSDOWN[i] == 0 && KEYSoldDOWN[i])
		{
			KEYSoldDOWN[i] = 0;
		}
	}
}
bool Input::ReadMouse()
{

	//Obtiene la posicion del mouse
	GetCursorPos(&p);

	// Process the changes in the mouse
	ProcessInput();

	return true;
}
//Procesa todo la funcionalidad del teclado
void Input::ProcessInput()
{

	//le pasa el punto las coordenadas del teclado
	m_mouseX = p.x;
	m_mouseY = p.y;

	//Cuando son diferentes se movio el teclado
	if (m_mouseX != m_mouseXOld)
	{
		//Se movio hacia la derecha
		if (m_mouseX > m_mouseXOld)
			resultX = 1;
		else //Se movio a la izquerda
			resultX = -1;

		//Actualia la variable anterior para saber cuando mueva de nuevo el mouse
		m_mouseXOld = m_mouseX;
	}
	else //Si no se movio cerro
		resultX = 0;

	//Cuando son diferentes se movio el teclado
	if (m_mouseY != m_mouseYOld)
	{
		//Se movio hacia abajo
		if (m_mouseY > m_mouseYOld)
			resultY = 1;
		else //Se movio hacia arriba
			resultY = -1;
		//Actualia la variable anterior para saber cuando mueva de nuevo el mouse
		m_mouseYOld = m_mouseY;
	}
	else //Si no se movio cero
		resultY = 0;


	return;
}
//Retornar las ubicaciones del mouse
void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}
//Hace que el mouse no se bloquea cuando topa la pantalla
void Input::MouseLibreWindow()
{
	//Rango para cambiar de posicion del mouse
	int inc = 10;
	//Obtener el tamaño de la resolucion de la pantalla
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// el mouse en X esta topando a la izquerda
	if (m_mouseX < 10) {
		//Cambiar la posicion hacia la derecha
		SetCursorPos(screenWidth - inc, m_mouseY);
	}
	// el mouse en X esta topando a la derecha
	else if (m_mouseX > screenWidth - 10)
	{
		//Cambiar la posicion hacia la izquerda
		SetCursorPos(0 + inc, m_mouseY);
	}
	//El mouse esta topando hacia arriba
	if (m_mouseY < 10)
	{
		//Mover el mouse hasta abajo
		SetCursorPos(m_mouseX, screenHeight - inc);
	}
	//El mouse esta topando hacia abajo
	else if (m_mouseY > screenHeight - 10)
	{
		//Mover el mouse hasta arriba
		SetCursorPos(m_mouseX, 0 + inc);
	}

}
//Muestra el cursor
void Input::MostrarCursor()
{
	ShowCursor(TRUE);
	return;
}
//Oculta el cursor
void Input::OcultarCursor()
{
	ShowCursor(FALSE);
	return;
}
//Evento para dectectar el scorll
bool Input::EventScroll(MSG msg)
{
	//la variable MSG tiene todo lo que hace la ventana
	if (msg.message == WM_MOUSEWHEEL)
	{
		scroll = (short)HIWORD(msg.wParam);
		scroll /= WHEEL_DELTA;
		return true;
	}
	scroll = 0;
	return true;
}