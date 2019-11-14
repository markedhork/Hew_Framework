#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer * pWindowContainer, HINSTANCE hInstance, 
	std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_class = window_class;

	this->RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 ;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 ;

	RECT wr;	//Window Rectangle
	wr.left = centerScreenX - this->width / 2;
	wr.top = centerScreenY - this->height / 2;
	wr.right = wr.left + this->width;
	wr.bottom = wr.top + this->height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);


	this->handle = CreateWindowEx(0,							//Extended Windows style
		this->window_class.c_str(),						//Window class name
		this->window_title.c_str(),						//Window Title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,				//Windows style
		wr.left,														//Window X Position
		wr.top,														//Window Y Postiton
		wr.right - wr.left,											//Window Width
		wr.bottom - wr.top,											//Window Height
		NULL,													//Handle to parent of this window.Since this is the first window, it has no parent window.
		NULL,						  							//Handle to menu or child window idetifier.
		this->hInstance,										//Handle to the instance of module to be used with this window
		pWindowContainer);												//Param to create window

	if (this->handle == NULL)
	{
		return false;
	}

	//Bring the window up on the screen and set it as main focus.
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	//Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));	//Initialize the message structure.

	while (PeekMessage(&msg,		//Where to store message(if exist)
		this->handle,				//Handle to window we are checking messages for
		0,							//Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used
		0,							//Maximum Filter Msg Value
		PM_REMOVE))					//Remove message after capturing it via PeekMessage. 
	{
		TranslateMessage(&msg);		//Translate message from virtual key messages into character messages so we can dispatching it.
		DispatchMessage(&msg);		//Dispatch message to our Window Proc for this window.
	}

	//Check if the windwo was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;	//Message processing loop takes care of destroying this window
			UnregisterClass(this->window_class.c_str(), this->hInstance);
			return false;
		}
	}
	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//ALL other messages
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		//retrieve ptr to window class
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		//forward message to window class handle
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		/*case WM_CHAR:
		{
			unsigned char letter = static_cast<unsigned char>(wParam);
			return 0;
		}
		case WM_KEYDOWN:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);
			return 0;
		}*/
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer*pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		//OutputDebugStringA("The window was created.\n");
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->window_class.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}