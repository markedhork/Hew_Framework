#pragma once

#define TOTAL_MAP_STYLE		6
#define TOTAL_BUILDS		(11)

#define MAP_X				5
#define MAP_Y				5

#define MIN_FLOAT_X			(-2.5f)
#define MAX_FLOAT_X			(2.5f)

#define MIN_FLOAT_Y			(-0.5f)
#define MAX_FLOAT_Y			(TOTAL_BUILDS*MAP_Y+MIN_FLOAT_Y)		//(TOTAL_BUILD*MAP_Y+MIN_FLOAT_Y)


static BYTE Build[TOTAL_BUILDS] = { 1,2,3,4,4,3,2,3,3,5,6 };

static BYTE map[TOTAL_MAP_STYLE][MAP_Y][MAP_X] = {
	{{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},},

	{{2,1,1,1,2},
	{1,1,2,1,1},
	{1,2,2,2,1},
	{1,1,2,1,1},
	{2,1,1,1,2},},

	{{1,1,1,1,1},
	{2,2,2,2,1},
	{1,1,1,1,1},
	{1,2,2,2,2},
	{1,1,1,1,1},},

	{{2,1,2,1,2},
	{1,1,2,1,1},
	{1,2,2,2,1},
	{1,1,2,1,1},
	{2,1,2,1,2},},

	{{2,1,2,1,2},
	{1,2,1,2,1},
	{2,1,2,1,2},
	{1,2,1,2,1},
	{2,1,2,1,2},},

	{{1,2,1,2,2},
	{1,2,1,2,1},
	{1,1,2,1,2},
	{1,2,1,2,1},
	{1,2,1,1,2},},

};