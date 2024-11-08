#include "SESSION.h"
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크


void SESSION::DoRecv()
{
	int retval;
	retval = recv(sock, recv_buf, BUFSIZE, 0);
}
