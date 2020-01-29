#pragma once
#include "SceneMgr.h"
#include "..//Handhold.h"

//PARAM FOR MESH
#define PLAYER_POS		Game1_mesh[0].pos
#define CAMERA_SPD		(0.1f)

class Game1 :
	public SceneMgr
{
public:
	bool Set();
	int Update();
	bool Draw();
private:
	D3DXVECTOR3 player;
	D3DXVECTOR3 recPos;
	D3DXVECTOR3 nextPos;
	Handhold handhold;


	LPD3DXFONT			pDXfont;    // the pointer to the font object
	int					CountDown = 90;
	bool				Rdy = true;
	bool				ReDistance = false;
	bool				AlreadySend = false;
};

