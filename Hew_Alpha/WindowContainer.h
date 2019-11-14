#pragma once
#include "RenderWindow.h"
#include "Keyboard\KeyboardClass.h"
#include "Graphics\Graphics.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderWindow render_window;
	KeyboardClass keyboard;
	Graphics gfx;
private:
};