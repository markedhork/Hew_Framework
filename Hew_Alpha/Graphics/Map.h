#pragma once

#define TOTAL_MAP_STYLE		4
#define MAP_X				5
#define MAP_Y				5
#define MAX_BUILD			100

#define MIN_FLOAT_X			(-2.5f)
#define MAX_FLOAT_X			(2.5f)

#define MIN_FLOAT_Y			(-0.5f)


#define MAX_FLOAT_Y			(44.5f)		//(TOTAL_BUILD*MAP_Y+MIN_FLOAT_Y)

#define TOTAL_BUILDS		(9)
static BYTE Build[MAX_BUILD] = { 1,2,3,4,4,3,2,3,3 }; //9
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
};