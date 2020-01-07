#pragma once
#include "SceneMgr.h"

#define CLICK_BT1_POS_X (WINDOW_WIDTH/2)
#define CLICK_BT1_POS_Y 50
#define CLICK_BT2_POS_X (WINDOW_WIDTH/2)
#define CLICK_BT2_POS_Y 50
#define CLICK_BT_WIDTH	50
#define CLICK_BT_HEIGHT 50

class Title :
	public SceneMgr
{
public:
	bool Set();
	int Update();
	bool Draw();
private:
	int ck = 0;

	LPD3DXFONT			pDXfont;    // the pointer to the font object

};

