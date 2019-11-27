#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512            
#define IP_ADDRESS "10.192.129.188"
#define DEFAULT_PORT "3504"

struct client_type
{
	//============================
	//Game Param
	//BYTE rec;

	//============================
	SOCKET socket;
};

class Network
{
public:
	Network();
	~Network();
	virtual bool Set() = 0;
	virtual void Process() = 0;
protected :
	bool Initialize();
	bool Uninitialize();
	WSAData wsa_data;
	thread target_thread;
};

