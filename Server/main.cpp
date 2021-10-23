#define WIN32_LEAN_AND_MEAN

#include "Server.h"
#include "PacketManager.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "27777"	

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

    Server server(DEFAULT_PORT);
    if (!server.Initialize())
    {
        printf("Server failed to initialize!\n");
        return 1;
    }

    server.Start();

    PacketManager::GetInstance()->CleanUp();
    delete PacketManager::GetInstance();

    WSACleanup();
	return 0;
}