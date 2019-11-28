#pragma once
#include "Network.h"

class Client :
	public Network
{
public:
	Client();
	~Client();
	bool Set();
	void Process();
	void Connect();
	void Send();
private:
	void ProcessClient();
	addrinfo *result = NULL, hints ,*ptr;
	std::string sent_message = "";
	client_type client = { INVALID_SOCKET };
	std::string message;
};

