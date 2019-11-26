#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512            
#define IP_ADDRESS "10.192.129.188"
#define DEFAULT_PORT "3504"

class Network
{
public:
	Network();
	~Network();
	virtual bool Set() = 0;
	virtual int Process() = 0;
protected :
	bool Initialize();
	bool Uninitialize();
	int iResult = 0;
	WSAData wsa_data;
};

