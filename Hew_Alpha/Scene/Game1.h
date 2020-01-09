#pragma once
#include "SceneMgr.h"
#include "..//Handhold.h"

#define PLAYER_POS		Game1_mesh[0].pos

class Game1 :
	public SceneMgr
{
public:
	bool Set();
	int Update();
	bool Draw();
private:
	D3DXVECTOR3 player;
	D3DXVECTOR3 recPlayer;
	Handhold handhold;
};

