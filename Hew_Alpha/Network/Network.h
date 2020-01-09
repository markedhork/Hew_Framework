#pragma once
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <thread>
//using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define IP_ADDRESS "192.168.11.2"
#define DEFAULT_PORT "3504"

struct CLIENT_MSG
{
	int number;
	float x;
	float y;
};

struct SERVER_MSG
{
	float px;
	float py;
};


enum NET_TYPE
{
	SERVER_TYPE,
	CLIENT_TYPE,
};

class Network
{
public:
	Network();
	~Network();
	virtual bool Set() = 0;
	virtual void Process() = 0;
	virtual void Send(CLIENT_MSG *p) = 0;
	virtual void Send(SERVER_MSG *p) = 0;
	virtual bool IfMsgFromClient() = 0;
	virtual CLIENT_MSG GetMsgFromClient() = 0;

	virtual bool IfMsgFromServer() = 0;
	virtual SERVER_MSG GetMsgFromServer() = 0;

	int GetType();
	bool IfConected();
protected:
	bool Initialize();
	bool Uninitialize();
	WSAData wsa_data;
	std::thread connect_thread;
	std::thread process_thread;
	std::thread receive_thread;
	int type;
	bool Successed = false;
};

