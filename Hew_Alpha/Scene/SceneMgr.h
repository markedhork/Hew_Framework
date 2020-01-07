#pragma once
#include "..//Network/Network.h"
#include "..//Graphics/Graphics.h"
#include "..//Mouse/MouseClass.h"
#include "..//Keyboard/KeyboardClass.h"

struct PLAYER
{
	int PX, PY;
};
enum SC_NUM
{
	TITLE_NUM,
	LOBBY1_NUM,
	LOBBY2_NUM,
	GAME1_NUM,
	GAME2_NUM,
};
class SceneMgr
{
public:
	bool Initialize(Graphics* pGfx, KeyboardClass* pKeyboard, 
		MouseClass* pMouse, Timer *pTimer,Network *pNetwork)
	{
		this->gfx = pGfx;
		this->keyboard = pKeyboard;
		this->mouse = pMouse;
		this->timer = pTimer;
		this->network = pNetwork;
		return true;
	}
	virtual bool Set() = 0;
	virtual int Update() = 0;
	virtual bool Draw() = 0;
protected:
	Graphics* gfx;
	KeyboardClass* keyboard;
	MouseClass* mouse;
	Timer *timer;
	Network *network;
};
