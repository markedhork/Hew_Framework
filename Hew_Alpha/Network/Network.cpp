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
	//Clean up Winsock
	WSACleanup();
	return true;
}

bool Network::Initialize()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		std::cout << "WSAStartup() failed with error: " << iResult << std::endl;
		return false;
	}
}
