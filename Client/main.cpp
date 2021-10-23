#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <bitset>

#include "Buffer.h"
#include "Packets.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512						// Default buffer length of our buffer in characters
#define DEFAULT_PORT "27777"					// The default port to use
#define SERVER "127.0.0.1"	

int main(void)
{
	std::string name;
	std::cout << "Please enter your name: ";
	std::getline(std::cin, name);

	WSADATA wsaData;

	struct addrinfo* infoResult = NULL;			// Holds the address information of our server
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	char recvbuf[DEFAULT_BUFLEN];				// The maximum buffer size of a message to send
	int result;									// code of the result of any command we use
	int recvbuflen = DEFAULT_BUFLEN;			// The length of the buffer we receive from the server

	// Initialize WinSock
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed with error: %d\n", result);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve server
	result = getaddrinfo(SERVER, DEFAULT_PORT, &hints, &infoResult);
	if (result != 0)
	{
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return 1;
	}

	// Try to connect to an address
	SOCKET connectionSocket = INVALID_SOCKET;
	for (ptr = infoResult; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		connectionSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (connectionSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		result = connect(connectionSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			closesocket(connectionSocket);
			connectionSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(infoResult); // Done connecting, free from memory

	if (connectionSocket == INVALID_SOCKET)
	{
		printf("Failed to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// TODO: Send packet to server with our client name that we entered
	printf("Connection successful!\n");

	// TODO: Listen for incoming data from server (MUST BE NON-BLOCKING)

	std::string input;
	while (true)
	{
		std::cout << "Enter Message: ";
		std::getline(std::cin, input);

		netutils::PacketSendMessage packet;
		packet.header.packetType = 0;
		packet.messageLength = input.size();
		packet.message = input;

		netutils::Buffer buffer(sizeof(int) + sizeof(int) + input.size());
		buffer.WriteInt(packet.header.packetType);
		buffer.WriteInt(packet.messageLength);
		buffer.WriteString(packet.message);

		// TODO: Transform our entered message into out "Message" packet and send it
		result = send(connectionSocket, buffer.data, buffer.Length(), 0);
		if (result == SOCKET_ERROR)
		{
			printf("Failed to send message: %d\n", WSAGetLastError());
			closesocket(connectionSocket);
			WSACleanup();
			return 1;
		}
	}

	// Shut down the connection 
	result = shutdown(connectionSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(connectionSocket);
		WSACleanup();
		return 1;
	}

	// Cleanup
	closesocket(connectionSocket);
	WSACleanup();
	return 0;
}