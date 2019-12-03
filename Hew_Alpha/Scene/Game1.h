#pragma once
#include "SceneMgr.h"
#define PLAYER_STATE Game1_sprite[0]

class Game1 :
	public SceneMgr
{
public:
	bool Set();
	bool Update();
	bool Draw();
private:
	PLAYER player;
	PLAYER recPlayer;
};

