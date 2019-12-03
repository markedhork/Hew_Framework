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
	void Connect();
	void Send(int *p);
private:
	void ProcessClient();
	addrinfo hints;
	addrinfo *server = NULL;
	SOCKET server_socket = INVALID_SOCKET;
	std::string msg = "";
	std::string new_msg = "";
	client_type client;                  
};

