#pragma once
#include "Network.h"

struct client_type
{
	//============================
	//Game Param
	BYTE rec;

	//============================
	SOCKET socket;
};

class Client :
	public Network
{
public:
	Client();
	bool Set();
	int Process(client_type &new_client);
private:
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	std::string sent_message = "";
	client_type client = { 0,INVALID_SOCKET };
	std::string message;
};

