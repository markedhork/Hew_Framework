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
	std::cout << "Setting up server..." << std::endl;
	getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &server);

	//Create a listening socket for connecting to server
	std::cout << "Creating server socket..." << std::endl;
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

	//Setup socket options
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

	//Assign an address to the server socket.
	std::cout << "Binding socket..." << std::endl;
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);

	//Listen for incoming connections.
	std::cout << "Listening..." << std::endl;
	listen(server_socket, SOMAXCONN);

	//Initialize the client list
	client = { INVALID_SOCKET };


	return true;
}

int Server::Process()
{
	target_thread = std::thread(ProcessClient(client, target_thread));
	return 0;
}

int Server::ProcessClient(client_type &new_client, std::thread &thread)
{
	std::string msg = "";
	char tempmsg[DEFAULT_BUFLEN] = "";

	//Session
	while (1)
	{
		memset(tempmsg, 0, DEFAULT_BUFLEN);

		if (new_client.socket != 0)
		{
			int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFLEN, 0);

			if (iResult != SOCKET_ERROR)
			{
				if (strcmp("", tempmsg))
					msg = "Client # Target";

				std::cout << msg.c_str() << std::endl;

				//Broadcast that message to the other clients
				if (new_client.socket != INVALID_SOCKET)
				iResult = send(new_client.socket, msg.c_str(), strlen(msg.c_str()), 0);

			}
			else
			{
				msg = "Client Target Disconnected";

				std::cout << msg << std::endl;

				closesocket(new_client.socket);
				new_client.socket = INVALID_SOCKET;

				//Broadcast the disconnection message to the other clients

				if (new_client.socket != INVALID_SOCKET)
					iResult = send(new_client.socket, msg.c_str(), strlen(msg.c_str()), 0);


				break;
			}
		}
	} //end while

	thread.detach();
	return 0;
}

