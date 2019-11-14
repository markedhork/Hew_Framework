#pragma once
#include "Network.h"

class Server :
	public Network
{
public:
	Server();
	bool Set();
	int Connection();
private:
	SOCKET sock0;
	int len;
	sockaddr_in addr;
	sockaddr_in client;
};

