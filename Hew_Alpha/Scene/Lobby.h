#pragma once
#include "SceneMgr.h"


class Lobby :
	public SceneMgr
{
public:
	bool Set();
	bool Update();
	bool Draw();
};

