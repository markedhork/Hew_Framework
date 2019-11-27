#pragma once
#include "Network.h"



class Client :
	public Network
{
public:
	Client();
	bool Set();
	void Process();
private:
	void ProcessClient();
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	std::string sent_message = "";
	client_type client = { INVALID_SOCKET };
	std::string message;
};

