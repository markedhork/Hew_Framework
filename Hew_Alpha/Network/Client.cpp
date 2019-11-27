#include "Client.h"

Client::Client()
{
	this->Set();
}

bool Client::Set()
{
	//Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	int iResult = getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &result);
	
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		client.socket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			int error = WSAGetLastError();
			string outmsg = "socket() failed Error: ";
			outmsg += to_string(error);
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(client.socket);
			client.socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	return true;
}

void Client::Process()
{
	this->target_thread = thread(&Client::ProcessClient, this);
}

void Client::ProcessClient()
{
}
