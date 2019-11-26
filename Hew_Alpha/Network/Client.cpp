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
	iResult = getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &result);
	
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		client.socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			std::cout << "socket() failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			system("pause");
			return 1;
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

int Client::Process(client_type &new_client)
{
	while (1)
	{
		memset(&new_client.rec, 0, sizeof(new_client.rec));

		if (new_client.socket != 0)
		{
			int iResult = recv(new_client.socket, (char*)new_client.rec, DEFAULT_BUFLEN, 0);

			if (iResult != SOCKET_ERROR)
				std::cout << new_client.rec << std::endl;
			else
			{
				std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
				break;
			}
		}
	}

	if (WSAGetLastError() == WSAECONNRESET)
		std::cout << "The server has disconnected" << std::endl;

	return 0;
}