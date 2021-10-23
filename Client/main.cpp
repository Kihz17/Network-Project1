#include "Client.h"
#include "PacketManager.h"

#include <stdio.h>

#define DEFAULT_PORT "27777"
#define SERVER "127.0.0.1"	

int main(void)
{
    int result = 0;
    WSADATA wsaData; // Holds winsock data

    // Initialize winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        printf("WSAStartup failed with error: %d\n", result);
        return 1;
    }

	Client client(SERVER, DEFAULT_PORT);
    if (!client.Initialize())
    {
        printf("Server failed to initialize!\n");
        return 1;
    }

    client.Start();

    PacketManager::GetInstance()->CleanUp();
    delete PacketManager::GetInstance();

    WSACleanup();
    return 0;
}