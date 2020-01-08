#pragma once

struct Holders
{
	bool use = false;
	float px, py;
	int type;
};

class Handhold
{
public:
	
private:
	Holders holders[500];
	int activedTotal;
};

