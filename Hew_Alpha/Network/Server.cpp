#include "Server.h"

Server::Server()
{

}

bool Server::Set()
{
	// ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock0, (struct sockaddr *)&addr, sizeof(addr));
	// TCPクライアントからの接続要求を待てる状態にする
	listen(sock0, 5);
	return true;
}

int Server::Connection()
{
	// TCPクライアントからの接続要求を受け付ける
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);
	send(sock, "HELLO", 5, 0);
	return 0;
}

