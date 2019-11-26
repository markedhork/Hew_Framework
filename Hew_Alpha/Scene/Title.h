#pragma once
#include "SceneMgr.h"

class Title :
	public SceneMgr
{
public:
	bool Set();
	bool Update();
	bool Draw();
};

