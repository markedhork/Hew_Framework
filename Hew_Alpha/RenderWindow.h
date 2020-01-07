#pragma once
#include <Windows.h>
#include <string>

#define WINDOW_WIDTH	(1600)
#define WINDOW_HEIGHT	(900)

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer * pWindowContainer, HINSTANCE hInstance,
		std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND handle = NULL;										//Handle to this window
	HINSTANCE hInstance = NULL;								//Handle to application instance
	std::string window_title = "";
	std::string window_class = "";
	int width = 0;
	int height = 0;

};
