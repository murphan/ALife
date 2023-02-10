//
// Created by Emmet on 1/29/2023.
//

#ifndef ALIFE_SOCKET_H
#define ALIFE_SOCKET_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "Ws2_32.lib")

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <vector>
#include <optional>
#include <deque>
#include <mutex>
#include <condition_variable>

#include "types.h"

class Socket {
private:
	constexpr static usize BUFFER_LENGTH = 1024;

	static WSAData wsaData;

	static SOCKET listenSocket;
	static SOCKET clientSocket;

	static std::deque<std::vector<char>> inQueue;
	static std::deque<std::vector<char>> outQueue;

	static i32 messageLengthIndex;
	static u32 currentMessageLength;
	static std::vector<char> currentMessage;

	/* multithreading */

	static bool running;

	static std::thread readerThread;
	static std::thread writerThread;

	static std::mutex inQueueMutex;
	static std::mutex outQueueMutex;
	static std::condition_variable outQueueSignal;

	/* internal utility */

	static auto errorCleanup(addrinfo *& addressInfo) -> void;

	static auto resetCurrentMessage() -> void;

	static auto readIntoMessage(char * buffer, i32 bytesReceived, i32 & start) -> void;

	static auto writeMessage(char * buffer, const std::vector<char> & data) -> void;

	static auto readerLoop() -> void;
	static auto writerLoop() -> void;

public:
	/**
	 * winsock is global for the entire program
	 * do not create instances of this class
	 */
	Socket() = delete;

	static auto init(const char * port) -> void;

	static auto isConnected() -> bool;

	static auto close() -> void;

	/**
	 * keep calling to grab new messages until empty is returned
	 */
	static auto queueMessage() -> std::optional<std::vector<char>>;

	template<typename T, std::enable_if_t<std::_Is_iterator_v<T>, int> = 0>
	static auto send(T begin, T end) -> void {
		auto lockGuard = std::lock_guard(outQueueMutex);

		outQueue.emplace_back();
		auto & message = outQueue.back();
		message.assign(begin, end);

		outQueueSignal.notify_all();
	}
};

#endif //ALIFE_SOCKET_H
