#pragma once
#include "Network.h"
class Client :
	public Network
{
public:
	Client();
	bool Set();
	int Connection();
private:
	struct sockaddr_in server;
	char buf[32];
};

