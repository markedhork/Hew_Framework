#pragma once
#include "Network.h"
#include <vector>

const char OPTION_VALUE = 1;


struct client_type
{
	//Game param
	//============================


	//============================
	SOCKET socket;
};

class Server :
	public Network
{
public:
	Server();
	~Server();
	bool Set();
	int Process();

private:
	int ProcessClient(client_type &new_client, std::thread &thread);
	struct addrinfo hints;
	struct addrinfo *server = NULL;
	SOCKET server_socket = INVALID_SOCKET;
	std::string msg = "";
	client_type client;

	std::thread target_thread;                      

};

