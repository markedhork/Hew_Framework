#pragma once
#include "SceneMgr.h"
#include "..//Handhold.h"

//PARAM FOR MESH
#define PLAYER_POS		Game1_mesh[0].pos

//PARAM FOR SPRITE
#define TARGET1_POS		Game1_sprite[1].pos


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
	D3DXVECTOR3 recPlayer;
	Handhold handhold;


	LPD3DXFONT			pDXfont;    // the pointer to the font object
	int					CountDown = 90;
};

