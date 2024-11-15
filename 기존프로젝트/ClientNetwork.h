#pragma once
#include "stdafx.h"
#define BUFFER_SIZE 1024

class NetWorkManager
{

public:
	
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER_SIZE] = { 0 };

	NetWorkManager();
	~NetWorkManager();

	void Con();
};