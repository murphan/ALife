//
// Created by Emmet on 1/30/2023.
//

#include "socket.h"

auto BackSocket::wsaData = WSAData {0,};

auto BackSocket::listenSocket = INVALID_SOCKET;
auto BackSocket::clientSocket = INVALID_SOCKET;
