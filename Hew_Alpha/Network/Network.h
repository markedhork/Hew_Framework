#pragma once
#include <WinSock2.h>

class Network
{
public:
	Network();
	~Network();
	virtual bool Set() = 0;
	virtual int Connection() = 0;
protected :
	bool Initialize();
	bool Uninitialize();
	WSADATA wsaData;
	SOCKET sock;
};

