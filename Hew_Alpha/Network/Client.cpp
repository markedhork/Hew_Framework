#include "Client.h"

Client::Client()
{
	this->Set();
}

Client::~Client()
{
	int iResult = shutdown(client.socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		OutputDebugStringA("shutdown() failed with error\n");
		closesocket(client.socket);
	}
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
	if (iResult != 0) {
		OutputDebugStringA("getaddrinfo() failed in Client class");
		return false;
	}
	this->Process();
	return true;
}

void Client::Process()
{
	this->connect_thread = thread(&Client::Connect, this);
	this->process_thread = thread(&Client::ProcessClient, this);
}

void Client::Connect()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		client.socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			int error = WSAGetLastError();
			string outmsg = "socket() failed Error: ";

			outmsg += to_string(error);
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
			WSACleanup();
			return;
		}

		// Connect to server.
		int iResult = connect(client.socket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(client.socket);
			client.socket = INVALID_SOCKET;
			continue;
		}
	}

	freeaddrinfo(result);
	if (client.socket == INVALID_SOCKET) {
		OutputDebugStringA("Unable to connect to server!\n");
	}
	else
	{
		OutputDebugStringA("Successfully Connected\n");
	}
}

void Client::Send()
{
	sent_message += "Hello";
	int iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);
	if (iResult < 0)
	{
		OutputDebugStringA("send() failed");
	}
}

void Client::ProcessClient()
{
	this->connect_thread.join();
	std::string msg = "";
	while (1)
	{
		memset(client.received_message, 0, DEFAULT_BUFLEN);

		if (client.socket != 0)
		{
			int iResult = recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);

			if (iResult != SOCKET_ERROR)
			{
				msg += client.received_message;
				msg += "\n";
				OutputDebugStringA(msg.c_str());
			}
			else
			{
				msg += "recv() failed: ";
				msg += WSAGetLastError();
				msg += "\n";
				OutputDebugStringA(msg.c_str());
				break;
			}
		}
	}

	if (WSAGetLastError() == WSAECONNRESET)
		OutputDebugStringA("The server has disconnected");
}


