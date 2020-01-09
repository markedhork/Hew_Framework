#pragma once
#include "SceneMgr.h"
#include "..//Ball.h"
#include "..//Handhold.h"
#define CLICK_ZONE_POS_X			(float)800
#define CLICK_ZONE_POS_Y			(float)750
#define CLICK_ZONE_POS_RADIUS		(float)150

#define PLAYER_POS		Game2_mesh[0].pos


class Game2 :
	public SceneMgr
{
public:
	bool Set();
	int Update();
	bool Draw();
private:
	bool CheckClickInZone();
	Ball ball;
	bool RdyToKick = false;

	NewPoint points[MAX_POINT];
	int pointTotal = 0;

	Handhold	handhold;
};

