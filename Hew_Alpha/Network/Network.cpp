#include "Network.h"
#include "..\\debug_printf.h"

Network::Network()
{
	this->Initialize();
}

Network::~Network()
{
	this->Uninitialize();
}

bool Network::Uninitialize()
{
	WSACleanup();
	return true;
}

bool Network::Initialize()
{
	// winsock2の初期化
	int error;
	error = WSAStartup(MAKEWORD(2, 0), &wsaData);			//0: SUCCESS ELSE: FAIL
	if (error)
	{
		switch (error)
		{
		case WSASYSNOTREADY:
			DebugPrintf("WSASYSNOTREADY");
			break;
		case WSAVERNOTSUPPORTED:
			DebugPrintf("WSAVERNOTSUPPORTED");
			break;
		case WSAEINPROGRESS:
			DebugPrintf("WSAEINPROGRESS");
			break;
		case WSAEPROCLIM:
			DebugPrintf("WSAEPROCLIM");
			break;
		case WSAEFAULT:
			DebugPrintf("WSAEFAULT");
			break;
		}
		return false;
	}

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) 
	{
		DebugPrintf("error: %d", WSAGetLastError());
		return false;
	}
}
