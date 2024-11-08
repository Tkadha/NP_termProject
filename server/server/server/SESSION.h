#pragma once
#define BUFSIZE    512

class SESSION
{
public:
	SOCKET sock;
	int id;
	char recv_buf[BUFSIZE];

public:
	SESSION()
	{
		id = -1;
	}
	void DoRecv();
};


