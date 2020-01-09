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
	void Send(CLIENT_MSG *data);
	void Send(SERVER_MSG *p);

	bool IfMsgFromServer();
	SERVER_MSG GetMsgFromServer();

	bool IfMsgFromClient();
	CLIENT_MSG GetMsgFromClient();


private:
	void ProcessClient();
	addrinfo *result = NULL, hints, *ptr;
	SOCKET server_socket =  INVALID_SOCKET;

	bool NewMsg = false;
	SERVER_MSG message;

};

