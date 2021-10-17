#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#include "Buffer.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512	

#define DEFAULT_PORT "27777"	

struct Client
{
    Client()
        : buffer(DEFAULT_BUFLEN)
    {
    }

    std::string name;
    SOCKET socket = NULL;

    // Buffer info
    netutils::Buffer buffer;
};

std::vector<Client*> clients;

int main(void)
{
    WSADATA wsaData; // Holds winsock data
    int result;
    struct addrinfo* info = NULL;
    struct addrinfo hints;

    // Initialize winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        printf("WSAStartup failed with error: %d\n", result);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); // Fills the hints block of memory with 0's
    hints.ai_family = AF_INET;  // Specify address fmaily
    hints.ai_socktype = SOCK_STREAM; // Assign socket type
    hints.ai_protocol = IPPROTO_TCP; // Use TCP for protocol
    hints.ai_flags = AI_PASSIVE;

    // Resolve port & address
    result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &info);
    if (result != 0)
    {
        printf("getaddrinfo failed with error: %d\n", result);
        WSACleanup();
        return 1;
    }

    SOCKET connectionSocket = INVALID_SOCKET; // This socket is used to listen for incoming connections
    connectionSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol); // Assign our socket with our address info properties
    if (connectionSocket == INVALID_SOCKET)
    { // We failed...
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(info);
        WSACleanup();
        return 1;
    }

    // Attempt to bind our connection socket
    result = bind(connectionSocket, info->ai_addr, (int)info->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(info);
        closesocket(connectionSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(info); // Socket was bound, we don't need this anymore.

    // Listen for activing on our connection socket
    result = listen(connectionSocket, SOMAXCONN); // Puts the connection socket in a state where it is listening for an incoming connection
    if (result == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(connectionSocket);
        WSACleanup();
        return 1;
    }

    DWORD nonBlock = 1;
    result = ioctlsocket(connectionSocket, FIONBIO, &nonBlock);
    if (result == SOCKET_ERROR)
    {
        printf("Failed to make connection socket non-blocking! %d\n", WSAGetLastError());
        closesocket(connectionSocket);
        WSACleanup();
        return 1;
    }

    FD_SET readSet;
    int total;
    DWORD flags;
    DWORD bytesReceived;
    SOCKET acceptSocket = INVALID_SOCKET; // Will hold a new connection

    printf("Starting server...\n");
    while (true)
    {
        timeval timeoutValue = { 0 };
        timeoutValue.tv_sec = 2; // 2 second timeout value

        FD_ZERO(&readSet); // Wipe out our read set
        FD_SET(connectionSocket, &readSet); // Add connection socket to the read set (AKA: Keep listening for connections)
        
        // Add all client sockets to the read set
        for (Client* client : clients)
        {
            FD_SET(client->socket, &readSet);
        }

        // Find the sockets that need updating
        total = select(0, &readSet, NULL, NULL, &timeoutValue);
        if (total == SOCKET_ERROR) {
            printf("select() has failed! %d\n", WSAGetLastError());
            return 1;
        }

        // Handle incoming connections
        if (FD_ISSET(connectionSocket, &readSet))
        {
            total--;
            acceptSocket = accept(connectionSocket, NULL, NULL); // Permits the incoming connection
            if (acceptSocket == INVALID_SOCKET)
            {
                printf("Failed to accept socket! %d\n", WSAGetLastError());
                return 1;
            }

            // Make the newly accept socket non-blocking
            result = ioctlsocket(acceptSocket, FIONBIO, &nonBlock);
            if (result == SOCKET_ERROR)
            {
                printf("Failed to make accepted socket non-blocking! %d\n", WSAGetLastError());
            }
            else
            {
                Client* client = new Client();
                client->socket = acceptSocket;
                clients.push_back(client);
                printf("New client has connected on socket %d.\n", (int)acceptSocket);
            }
        }

        for (Client* client : clients)
        {
            // Check if we are in the read set (AKA: Check if socket sent some new data)
            if (FD_ISSET(client->socket, &readSet))
            {
                total--;
                DWORD flags = 0;
                bytesReceived = recv(client->socket, client->buffer.data, client->buffer.Length(), flags); // Recieve the data (THIS IS BLOCKING, which is we we only call it when the socket has new data)

                printf("We got a message!\n");
                // TODO: Read data from buffer. Will probably go something like:
                // TODO: client->buffer.ReadShort(); This will be used to read the packet header to decide what we should do for the rest of the data
                // TODO: Handle packet accordingly


            }
        }
    }

    for (Client* client : clients)
    {
        delete client;
    }

    clients.clear();

	return 0;
}