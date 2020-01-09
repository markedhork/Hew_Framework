#include "Server.h"

Server::Server()
{
	this->Set();
	this->type = SERVER_TYPE;
}

Server::~Server()
{
	//Close listening socket
	closesocket(server_socket);

	//Close client socket
	process_thread.detach();
	closesocket(client_socket);
}

bool Server::Set()
{
	int iResult;
	//Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Setup Server
	OutputDebugStringA("Setting up server...\n");
	iResult = getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &server);
	if (iResult)
	{
		std::string outmsg = "";
		outmsg += "getaddrinfo() Failed Error: ";
		outmsg += iResult;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}
	else
	{
		OutputDebugStringA("getaddrinfo() Successed!\n");
	}

	//Create a listening socket for connecting to server
	OutputDebugStringA("Creating server socket...\n");
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
	if (server_socket == INVALID_SOCKET)
	{
		OutputDebugStringA("Failed to create socket...\n");
	}
	//Setup socket options
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

	//Assign an address to the server socket.
	OutputDebugStringA("Binding socket...\n");
	iResult = bind(server_socket, (const sockaddr *)server->ai_addr, (int)server->ai_addrlen);
	if (iResult)
	{
		std::string outmsg = "";
		outmsg += "Bind() Failed Error: ";
		outmsg += iResult;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}
	else
	{
		OutputDebugStringA("Bind Successed!\n");
	}

	//Listen for incoming connections.
	OutputDebugStringA("Listening...\n");
	iResult = listen(server_socket, SOMAXCONN);
	if (iResult)
	{
		std::string outmsg = "";
		outmsg += "Listen() Failed Error: ";
		outmsg += iResult;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}
	else
	{
		OutputDebugStringA("Listen Successed!\n");
	}


	this->Process();

	return true;
}

void Server::Process()
{
	this->connect_thread = std::thread(&Server::Connect, this);
}

void Server::Connect()
{
	OutputDebugStringA("Server Waiting for Connection...\n");
	while (1)
	{
		SOCKET incoming = INVALID_SOCKET;
		incoming = accept(server_socket, NULL, NULL);
		if (incoming == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			std::string outmsg = "";
			outmsg += "accept() Failed Error: ";
			outmsg += error;
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
			continue;
		}
		else
		{
			OutputDebugStringA("Client connected!!!\n");

		}
		client_socket = incoming;
		if (client_socket != INVALID_SOCKET)
			break;
	}
	//connect_thread.detach();
	this->process_thread = std::thread(&Server::ProcessClient, this);
}

void Server::Send(SERVER_MSG *data)
{
	//send message from client
	OutputDebugStringA("new msg to send\n");
	//Send message to client
	if (client_socket != INVALID_SOCKET)
	{
		int iResult = send(client_socket, (char*)data, sizeof(SERVER_MSG), 0);
		if (iResult == 0)
		{
			OutputDebugStringA("Failed to send msg to client\n");
		}
		else
		{
			OutputDebugStringA("msg send to client\n");
		}
	}

}

void Server::Send(CLIENT_MSG * p)
{
}

bool Server::IfMsgFromClient()
{
	return this->NewMsgs;
}

CLIENT_MSG Server::GetMsgFromClient()
{
	CLIENT_MSG temp;
	temp = message;
	this->NewMsgs = false;
	return temp;
}

bool Server::IfMsgFromServer()
{
	return false;
}

SERVER_MSG Server::GetMsgFromServer()
{
	return SERVER_MSG();
}

void Server::ProcessClient()
{
	OutputDebugStringA("Server Begin to process...\n");
	this->Successed = true;
	//Session
	while (1)
	{
		CLIENT_MSG tempRecv;

		if (client_socket != 0)
		{
			//recive message from client
			int iResult = recv(client_socket, (char*)&tempRecv, sizeof(CLIENT_MSG), 0);

			if (iResult != SOCKET_ERROR)
			{
				std::string Outmsg = "message from client:";
				Outmsg += (char*)&tempRecv;
				Outmsg += "\n";
				OutputDebugStringA(Outmsg.c_str());

				message = tempRecv;
				this->NewMsgs=true;
			}
			else
			{
				std::string Outmsg = "Client Disconnected\n";
				OutputDebugStringA(Outmsg.c_str());

				closesocket(client_socket);
				client_socket = INVALID_SOCKET;
				break;
			}

		}
	} //end while
	OutputDebugStringA("process_thread.detach\n");
	process_thread.detach();
}

