#include "Client.h"

Client::Client()
{

}

bool Client::Set()
{
	// �ڑ���w��p�\���̂̏���
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	return true;
}

int Client::Connection()
{
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	return 0;
}
