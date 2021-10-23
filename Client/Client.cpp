#include "Client.h"
#include "Packets.h"
#include "PacketManager.h"

#include <stdio.h>
#include <conio.h>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512 // Default buffer length of our buffer in characters

Client::Client(PCSTR serverIp, PCSTR port)
	: buffer(DEFAULT_BUFLEN)
{
	this->serverIp = serverIp;
	this->serverPort = port;
	this->serverSocket = INVALID_SOCKET;
	this->running = false;
}

Client::~Client()
{

}

bool Client::Initialize()
{
	std::cout << "Initializing client..." << std::endl;
	struct addrinfo* infoResult = NULL; // Holds the address information of our server
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	int result;								

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve server
	result = getaddrinfo(this->serverIp, this->serverPort, &hints, &infoResult);
	if (result != 0)
	{
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return 1;
	}

	// Try to connect to an address
	for (ptr = infoResult; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		this->serverSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (this->serverSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		result = connect(this->serverSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			closesocket(this->serverSocket);
			this->serverSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(infoResult); // Done connecting, free from memory

	if (this->serverSocket == INVALID_SOCKET)
	{
		printf("Failed to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Force our connection socket to be non-blocking
	DWORD nonBlock = 1;
	result = ioctlsocket(this->serverSocket, FIONBIO, &nonBlock);
	if (result == SOCKET_ERROR)
	{
		printf("Failed to make connection socket non-blocking! %d\n", WSAGetLastError());
		closesocket(this->serverSocket);
		WSACleanup();
		return false;
	}

	printf("Connection successful!\n");
	return true;
}

void Client::Start()
{
	std::cout << "Please enter your name: ";
	std::getline(std::cin, this->name);

	std::cout << "Please enter the room you'd like to enter: ";
	std::getline(std::cin, this->currentRoom);

	netutils::PacketJoinRoom packetJoinRoom;
	packetJoinRoom.roomNameLength = this->currentRoom.size();
	packetJoinRoom.roomName = this->currentRoom;
	packetJoinRoom.nameLength = this->name.size();
	packetJoinRoom.name = this->name;

	this->buffer.WriteInt(packetJoinRoom.header.packetType);
	this->buffer.WriteInt(packetJoinRoom.roomNameLength);
	this->buffer.WriteString(packetJoinRoom.roomName);
	this->buffer.WriteInt(packetJoinRoom.nameLength);
	this->buffer.WriteString(packetJoinRoom.name);

	this->SendToServer(this->buffer.data, this->buffer.Length());
	this->buffer.Clear();

	std::vector<char> message;

	int total;
	int result;

	DWORD flags;
	DWORD bytesReceived;

	FD_SET readSet;

	this->running = true;
	bool sendEnterMessage = true;
	bool roomChange = false;
	while (this->running)
	{
		if (sendEnterMessage)
		{
			std::cout << "Press 'esc' to Change Room OR Enter Message: " << std::endl;
			std::cout << std::endl;
			sendEnterMessage = false;
		}

		if (_kbhit())
		{
			char key = _getch();
			if (key == 8 && !message.empty()) // Backspace
			{
				message.pop_back();
				std::string msg(message.begin(), message.end());
				std::cout << "\r" << msg;
			}
			else if (key == 13) // Enter
			{
				std::string msg(message.begin(), message.end());
				message.clear();

				if (roomChange)
				{
					roomChange = false;

					if (msg == "leave")
					{
						netutils::PacketLeaveRoom packet;
						packet.roomNameLength = this->currentRoom.size();
						packet.roomName = this->currentRoom;
						packet.namelength = this->name.size();
						packet.name = this->name;

						netutils::Buffer buffer(packet.GetSize());
						buffer.WriteInt(packet.header.packetType);
						buffer.WriteInt(packet.roomNameLength);
						buffer.WriteString(packet.roomName);
						buffer.WriteInt(packet.namelength);
						buffer.WriteString(packet.name);

						this->running = this->SendToServer(buffer.data, buffer.Length());
						if (!this->running)
						{
							break;
						}

						std::cout << std::endl;
						std::cout << std::endl;
						this->running = false; // Stop client
					}
					else 
					{ 
						netutils::PacketJoinRoom packet;
						packet.roomNameLength = msg.size();
						packet.roomName = msg;
						packet.nameLength = name.size();
						packet.name = name;

						netutils::Buffer buffer(packet.GetSize());
						buffer.WriteInt(packet.header.packetType);
						buffer.WriteInt(packet.roomNameLength);
						buffer.WriteString(packet.roomName);
						buffer.WriteInt(packet.nameLength);
						buffer.WriteString(packet.name);

						this->running = this->SendToServer(buffer.data, buffer.Length());
						if (!this->running)
						{
							break;
						}

						this->currentRoom = packet.roomName;
						std::cout << std::endl;
						std::cout << std::endl;
						sendEnterMessage = true;
					}
				}
				else
				{
					netutils::PacketSendMessage packet;
					packet.messageLength = msg.size();
					packet.message = msg;

					netutils::Buffer buffer(packet.GetSize());
					buffer.WriteInt(packet.header.packetType);
					buffer.WriteInt(packet.messageLength);
					buffer.WriteString(packet.message);
		
					this->running = this->SendToServer(buffer.data, buffer.Length());
					if (!this->running)
					{
						break;
					}

					std::cout << std::endl;
					std::cout << std::endl;
					std::cout << "[" << name << "]: " << msg << std::endl;

					sendEnterMessage = true;
				}	
			}
			else if (key == 27) // escape key
			{
				message.clear();
				std::cout << "Enter Room Name:" << std::endl;
				roomChange = true;
			}
			else
			{
				message.push_back(key);
				std::string msg(message.begin(), message.end());
				std::cout << "\r" << msg;
			}
		}

		FD_ZERO(&readSet); // Wipe out our read set
		FD_SET(this->serverSocket, &readSet); // Add connection socket to the read set (AKA: Keep listening for connections)

		timeval timeoutValue = { 0 };

		// Find the sockets that need updating
		total = select(0, &readSet, NULL, NULL, &timeoutValue);
		if (total == SOCKET_ERROR) {
			printf("select() has failed! %d\n", WSAGetLastError());
			this->running = false;
			break;
		}

		// Handle incoming connections
		if (FD_ISSET(this->serverSocket, &readSet))
		{
			total--;
			DWORD flags = 0;

			bytesReceived = recv(this->serverSocket, this->buffer.data, this->buffer.Length(), flags); // Recieve the data (THIS IS BLOCKING, which is we we only call it when the socket has new data)
			if (bytesReceived == SOCKET_ERROR)
			{
				printf("recv() has failed!");

				if (WSAGetLastError() == 10054)
				{
					printf("Disconnected from server!\n");
					this->running = false;
					break;
				}

			}
			else if (bytesReceived == 0)
			{
				printf("Disconnected from server!\n");
				this->running = false;
				break;
			}

			int packetHeader = buffer.ReadInt();
			PacketManager::GetInstance()->HandlePacket(*this, this->serverSocket, packetHeader);
			this->buffer.Clear();
		}
	}

	this->ShutDown();
}

void Client::ShutDown()
{
	std::cout << "Client shutting down..." << std::endl;

	int result;
	// Shut down the connection 
	result = shutdown(this->serverSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(this->serverSocket);
		return;
	}

	// Cleanup
	closesocket(this->serverSocket);
}

bool Client::SendToServer(char* data, int dataLength)
{
	int result;
	result = send(this->serverSocket, data, dataLength, 0);
	if (result == SOCKET_ERROR)
	{
		printf("Failed to send message: %d\n", WSAGetLastError());
		closesocket(this->serverSocket);
		return false;
	}

	return true;
}