#pragma once

#define TOTAL_MAP_STYLE		3
#define MAP_X				5
#define MAP_Y				5
#define MAX_BUILD			100

//STYLE A
static BYTE mapA[MAP_Y][MAP_X] =
{
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
};

//STYLE B
static BYTE mapB[MAP_Y][MAP_X] =
{
	{2,1,1,1,2},
	{1,1,2,1,1},
	{1,2,2,2,1},
	{1,1,2,1,1},
	{2,1,1,1,2},
};

//STYLE C
static BYTE mapC[MAP_Y][MAP_X] =
{
	{1,1,1,1,1},
	{2,2,2,2,1},
	{1,1,1,1,1},
	{1,2,2,2,2},
	{1,1,1,1,1},
};

static BYTE Build[MAX_BUILD] = {1};