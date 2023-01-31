//
// Created by Emmet on 1/29/2023.
//

#ifndef ALIFE_SOCKET_H
#define ALIFE_SOCKET_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <vector>
#include <optional>
#include <deque>

#include "types.h"

class Socket {
private:
	constexpr static usize BUFFER_LENGTH = 1024;

	static WSAData wsaData;

	static SOCKET listenSocket;
	static SOCKET clientSocket;

	static std::deque<std::vector<char>> storage;

	static i32 messageLengthIndex;
	static u32 currentMessageLength;
	static std::vector<char> currentMessage;

	static auto errorCleanup(addrinfo *& addressInfo) -> void;

	static auto resetCurrentMessage() -> void;

	static auto readIntoMessage(char * buffer, i32 bytesReceived, i32 & start) -> void;
public:
	/**
	 * winsock is global for the entire program
	 * do not create instances of this class
	 */
	Socket() = delete;

	static auto init(const char * address, const char * port) -> void;

	static auto poll() -> void;

	static auto isConnected() -> bool;

	static auto close() -> void;

	/**
	 * keep calling to grab new messages until empty is returned
	 */
	static auto queueMessage() -> std::optional<std::vector<char>>;

	static auto send(std::vector<char> & data) -> void;
};

#endif //ALIFE_SOCKET_H
