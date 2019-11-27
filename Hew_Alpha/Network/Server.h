#pragma once
#include "Network.h"

const char OPTION_VALUE = 1;


class Server :
	public Network
{
public:
	Server();
	~Server();
	bool Set();
	void Process();

private:
	void ProcessClient();
	addrinfo hints;
	addrinfo *server = NULL;
	SOCKET server_socket = INVALID_SOCKET;
	std::string msg = "";
	client_type client;                  
};

