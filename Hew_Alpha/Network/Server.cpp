#include "Server.h"

Server::Server()
{
	this->Set();
}

Server::~Server()
{
	//Close listening socket
	closesocket(server_socket);

	//Close client socket
	target_thread.detach();
	closesocket(client.socket);
}

bool Server::Set()
{
	//Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Setup Server
	OutputDebugStringA("Setting up server...\n");
	getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &server);

	//Create a listening socket for connecting to server
	OutputDebugStringA("Creating server socket...\n");
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

	//Setup socket options
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

	//Assign an address to the server socket.
	OutputDebugStringA("Binding socket...\n");
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);

	//Listen for incoming connections.
	OutputDebugStringA("Listening...\n");
	listen(server_socket, SOMAXCONN);

	//Initialize the client list
	client = { INVALID_SOCKET };

	this->Process();
	return true;
}

void Server::Process()
{
	target_thread = thread(&Server::ProcessClient, this);
}

void Server::ProcessClient()
{

}

