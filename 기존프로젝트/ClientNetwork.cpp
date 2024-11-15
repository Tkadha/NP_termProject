#include "ClientNetwork.h"

#pragma comment(lib, "ws2_32")

#define PORT 6112
#define BUFFER_SIZE 1024
Connect::Connect()
{
    WSADATA wsa;

    SOCKET sock;

    struct sockaddr_in serv_addr;

    char buffer[BUFFER_SIZE] = { 0 };
    const char* server_ip = "127.0.0.1";

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {

        err_quit("WSAStartup()");

    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {

        err_quit("socket()");

    }

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {

        err_display("inet_pton()");

        closesocket(sock);

        WSACleanup();

    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {

        err_display("connect()");

        closesocket(sock);

        WSACleanup();

    }
}


Connect::~Connect()
{
}
