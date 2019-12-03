#include "Client.h"

Client::Client()
{
	this->Set();
}

Client::~Client()
{
	int iResult = shutdown(server.socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		OutputDebugStringA("shutdown() failed with error\n");
	}
	process_thread.detach();
	closesocket(server.socket);

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
	this->connect_thread = std::thread(&Client::Connect, this);
}

void Client::Connect()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		server.socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (server.socket == INVALID_SOCKET) {
			int error = WSAGetLastError();
			std::string outmsg = "socket() failed Error: ";

			outmsg += std::to_string(error);
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
			WSACleanup();
			return;
		}

		// Connect to server.
		int iResult = connect(server.socket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(server.socket);
			server.socket = INVALID_SOCKET;
			continue;
		}
	}

	freeaddrinfo(result);
	if (server.socket == INVALID_SOCKET) {
		OutputDebugStringA("Unable to connect to server!\n");
	}
	else
	{
		OutputDebugStringA("Successfully Connected\n");
	}
	connect_thread.detach();
	this->process_thread = std::thread(&Client::ProcessClient, this);
}

void Client::Send(int *p)
{
	sent_message = "";
	sent_message += std::to_string(p[0]);
	newMsg = true;

	if (newMsg && server.socket != 0)
	{
		int iResult = send(server.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);
		if (iResult < 0)
		{
			OutputDebugStringA("Client send() to server failed\n");
		}
		newMsg = false;
	}
}

void Client::ProcessClient()
{
	while (1)
	{
		memset(server.received_message, 0, DEFAULT_BUFLEN);

		if (server.socket != 0)
		{
			int iResult = recv(server.socket, server.received_message, DEFAULT_BUFLEN, 0);

			if (iResult != SOCKET_ERROR)
			{
				std::string Outmsg = "";

				Outmsg += server.received_message;
				Outmsg += "\n";
				OutputDebugStringA(Outmsg.c_str());
			}
			else
			{
				std::string Outmsg = "";
				Outmsg += "client recv() from server failed: ";
				Outmsg += std::to_string(WSAGetLastError());
				Outmsg += "\n";
				OutputDebugStringA(Outmsg.c_str());
				break;
			}
		}
	}

	if (WSAGetLastError() == WSAECONNRESET)
		OutputDebugStringA("The server has disconnected\n");

	process_thread.detach();
}


