#pragma comment (lib, "Ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>

using namespace std;

#define DEFAULT_PORT "7777"
#define DEFAULT_BUFLEN 512

char recvbuf[DEFAULT_BUFLEN];
int iSendResult;
int recvbuflen = DEFAULT_BUFLEN;

struct addrinfo *result = NULL, *ptr = NULL, hints;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

int main()
{
	int iResult;
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	freeaddrinfo(result);

	listen(ListenSocket, SOMAXCONN);
	ClientSocket = accept(ListenSocket, NULL, NULL);

	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		cout << "Bytes recieved: " << iResult << endl;
		iSendResult = send(ClientSocket, recvbuf, iResult, 0);
		cout << "Bytes sent: " << iSendResult << endl;

	} while (iResult > 0);

	iResult = shutdown(ClientSocket, SD_SEND);

	closesocket(ClientSocket);
	WSACleanup();

    return 0;
}

