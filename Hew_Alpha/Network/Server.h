	#pragma once
	#include "Network.h"

	const char OPTION_VALUE = 1;

	class Server :
		public Network
	{
	public:
		Server();
		~Server();
		bool Set();
		void Process();
		void Connect();
		void Send(SERVER_MSG *data);
		void Send(CLIENT_MSG *p);

		bool IfMsgFromClient();
		CLIENT_MSG GetMsgFromClient();

		bool IfMsgFromServer();
		SERVER_MSG GetMsgFromServer();

	private:
		void ProcessClient();
		addrinfo hints;
		addrinfo *server = NULL;
		SOCKET server_socket = INVALID_SOCKET;
		SOCKET client_socket = INVALID_SOCKET;
		bool NewMsgs = false;
		CLIENT_MSG message;

	};

