#pragma once
#include "..//Network/Network.h"
#include "..//Graphics/Graphics.h"
#include "..//Mouse/MouseClass.h"
#include "..//Keyboard/KeyboardClass.h"




class SceneMgr
{
public:
	bool Initialize(Graphics* pGfx, KeyboardClass* pKeyboard, 
		MouseClass* pMouse, Timer *pTimer,Network *network)
	{
		this->gfx = pGfx;
		this->keyboard = pKeyboard;
		this->mouse = pMouse;
		this->timer = pTimer;
		return true;
	}
	virtual bool Set() = 0;
	virtual bool Update() = 0;
	virtual bool Draw() = 0;
protected:
	Graphics* gfx;
	KeyboardClass* keyboard;
	MouseClass* mouse;
	Timer *timer;
	Network *network;
};
