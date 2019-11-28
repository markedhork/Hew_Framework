#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = true;
	if (raw_input_initialized == true)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;			//Mouse
		rid.dwFlags = 0;
		rid.hwndTarget = 0;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			exit(-1);
			//registration failed. Call GetLastError for the cause of the error
		}
		raw_input_initialized = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	//OutputDebugStringA("WINDOW PROC FROM WINDOW CONTAINER\n");
	switch (uMsg)
	{
	//Keyboard Messages
	case WM_KEYDOWN:
	{
		unsigned char keycode = (unsigned char)wParam;
		if (keyboard.IsKeysAutoRepeat())
		{
			keyboard.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & (1 << 30);
			if (!wasPressed)
			{
				keyboard.OnKeyPressed(keycode);
				
			}
		}
		if (wParam == VK_ESCAPE) {
			SendMessage(hwnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = (unsigned char)wParam;
		keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = (unsigned char)wParam;
		if (keyboard.IsCharsAutoRepeat())
		{
			keyboard.OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & (1 << 30);
			if (!wasPressed)
			{
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	//Mouse Messages
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.OnWheelDown(x, y);
		}
		break;
	}


	case WM_INPUT:
	{
		UINT dataSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = (RAWINPUT*)rawdata.get();
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowContainer::EnableCursor()
{
	cursorEnable = true;
	ShowCursor();
}

void WindowContainer::DisableCursor()
{
	cursorEnable = false;
	HideCursor();
}

void WindowContainer::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}

void WindowContainer::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}
