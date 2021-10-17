#pragma once

#include <string>
#include <winsock2.h>

#include "Buffer.h"

class Client
{
public:
	Client();

    std::string name;
    SOCKET socket = NULL;

    // Buffer info
    netutils::Buffer buffer;
};