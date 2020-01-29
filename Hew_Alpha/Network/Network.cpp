#include "Network.h"

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
	//Clean up Winsock
	WSACleanup();
	return true;
}

int Network::GetType()
{
	return this->type;
}

bool Network::IfConected()
{
	return this->Successed;
}

bool Network::Initialize()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		std::string outmsg = "WSAStartup() FAILED: ";
		outmsg += std::to_string(iResult);
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
		return false;
	}
}
