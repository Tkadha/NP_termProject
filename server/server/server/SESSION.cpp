#include "SESSION.h"
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ


void SESSION::DoRecv()
{
	int retval;
	retval = recv(sock, recv_buf, BUFSIZE, 0);
}
