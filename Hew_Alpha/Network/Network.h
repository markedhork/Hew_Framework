#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <thread>
//using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 256            
#define IP_ADDRESS "10.205.102.38"
#define DEFAULT_PORT "3504"

struct client_type
{
	SOCKET socket;
	char received_message[DEFAULT_BUFLEN];
};

class Network
{
public:
	Network();
	~Network();
	virtual bool Set() = 0;
	virtual void Process() = 0;
	virtual void Send(int *p) = 0;
protected:
	bool Initialize();
	bool Uninitialize();
	WSAData wsa_data;
	std::thread connect_thread;
	std::thread process_thread;
	std::thread receive_thread;
};

