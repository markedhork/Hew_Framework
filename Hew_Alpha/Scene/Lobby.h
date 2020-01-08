#pragma once
#include "SceneMgr.h"


class Lobby :
	public SceneMgr
{
public:
	bool Set();
	int Update();
	bool Draw();

private:

	LPD3DXFONT			pDXfont;    // the pointer to the font object
	int					CountDown = 5;
	bool				RdyToGo = false;
};

