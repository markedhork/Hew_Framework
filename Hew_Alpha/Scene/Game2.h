#pragma once
#include "SceneMgr.h"
#include "..//Ball.h"
#define CLICK_ZONE_POS_X			(float)800
#define CLICK_ZONE_POS_Y			(float)750
#define CLICK_ZONE_POS_RADIUS		(float)150


class Game2 :
	public SceneMgr
{
public:
	bool Set();
	int Update();
	bool Draw();
private:
	bool CheckClickInZone();
	PLAYER player;
	Ball ball;
	bool RdyToKick = false;
};

