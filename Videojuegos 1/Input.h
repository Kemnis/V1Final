#pragma once
#include <Windows.h>
class Input
{
public:
	Input();
	~Input();

	bool Initialize(int, int);
	bool Update();

	void GetMouseLocation(int&, int&);

	BOOL KEYS[256];
	BOOL KEYSDOWN[256];
	unsigned char m_keyboardState[256];
	void MouseLibreWindow();
	void MostrarCursor();
	void OcultarCursor();
	bool ReadKeyboard();
	bool ReadMouse();
	// resultX da 1 (el mouse se mueve hacia la derecha), si da -1 el mouse se mueve hacia la izquerda, cero no hace nada
	int resultX;
	//resultY da 1 se mueve hacia abajo, si da -1 se mueve hacia arriba, cero nada
	int resultY;
	//Scroll da > 1 lo mueves hacia adelante, si da < 1 lo mueves hacia atras
	int scroll;
private:
	BOOL KEYSoldDOWN[256];//Guardar la ultima tecla presionada

	void ProcessInput();
	bool EventScroll(MSG);

private:
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	int m_mouseXOld, m_mouseYOld;

	POINT p;
};
class KeyCode
{
public:
	enum Keys
	{
		//Lens Info
		Clic_Izq = 0x01, Clic_Der,
		Clic_Middle = 0x04,
		Backspace = 0x08, Tab,
		Clear = 0x0C, Enter,
		Shift = 0x10, Control, Alt,
		Escape = 0x1B,
		Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
		Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};
};