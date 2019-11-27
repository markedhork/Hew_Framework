#pragma once
#include "SceneMgr.h"

struct PLAYER
{
	int PX, PY;
};


class Game1 :
	public SceneMgr
{
public:
	bool Set();
	bool Update();
	bool Draw();
private:
};

