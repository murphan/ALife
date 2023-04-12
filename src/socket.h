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
#include <span>
#include <iostream>

#include "util.h"
#include "types.h"

struct BackSocket {
	static WSAData wsaData;

	static SOCKET listenSocket;
	static SOCKET clientSocket;
};

class Socket {
private:
	constexpr static usize BUFFER_LENGTH = 1024;

	std::deque<std::vector<char>> outQueue;

	i32 currentMessageIndex;
	u32 currentMessageLength;
	std::vector<char> currentMessage;

	/* multithreading */

	bool running;

	std::thread readerThread;
	std::thread writerThread;

	std::mutex outQueueMutex;
	std::condition_variable outQueueSignal;

	/* internal utility */

	auto errorCleanup(addrinfo *& addressInfo) -> void {
		if (addressInfo != nullptr) freeaddrinfo(addressInfo);
		addressInfo = nullptr;

		if (BackSocket::listenSocket != INVALID_SOCKET) closesocket(BackSocket::listenSocket);
		BackSocket::listenSocket = INVALID_SOCKET;

		WSACleanup();
	}

	auto resetCurrentMessage() -> void {
		currentMessageIndex = 0;
		currentMessageLength = 0;
		currentMessage.clear();
	}

	template<Util::Function<void, std::vector<char> &> OnReadFunction>
	auto readIntoMessage(char * buffer, i32 bytesReceived, i32 & bufferIndex, OnReadFunction onRead) -> void {
		auto headerBytesToRead = min(4 - currentMessageIndex, bytesReceived);

		auto start = bufferIndex;
		for (; bufferIndex < start + headerBytesToRead; ++bufferIndex) {
			currentMessageLength |= buffer[bufferIndex] << ((3 - currentMessageIndex) * 8);
			++currentMessageIndex;
		}

		auto bodyBytesToRead = min(bytesReceived - bufferIndex, (i32)currentMessageLength);

		currentMessage.insert(currentMessage.end(), buffer + bufferIndex, buffer + bufferIndex + bodyBytesToRead);
		currentMessageIndex += bodyBytesToRead;
		bufferIndex += bodyBytesToRead;

		if (currentMessage.size() == currentMessageLength) {
			onRead(currentMessage);
			resetCurrentMessage();
		}
	}

	enum SendResult {
		GOOD = false,
		BAD = true,
	};

	inline auto writePartIntoBuffer(std::span<char> buffer, i32 & bufferIndex, std::span<const char> data) -> i32 {
		auto leftInBuffer = buffer.size() - bufferIndex;
		auto writeSize = (i32)min(data.size(), leftInBuffer);

		memcpy_s(buffer.data() + bufferIndex, leftInBuffer, data.data(), writeSize);

		bufferIndex += writeSize;
		return writeSize;
	}

	template<Util::Function<SendResult, std::span<const char>> T>
	inline auto shipIntoBuffer(std::span<char> buffer, i32 & bufferIndex, std::span<const char> data, T onBufferFilled) -> SendResult {
		auto dataIndex = 0;

		while (dataIndex < data.size()) {
			dataIndex += writePartIntoBuffer(buffer, bufferIndex, data.subspan(dataIndex, data.size() - dataIndex));

			if (bufferIndex == buffer.size()) {
				if (onBufferFilled(buffer.subspan(0, buffer.size()))) return SendResult::BAD;

				bufferIndex = 0;
			}
		}

		return SendResult::GOOD;
	}

	template<Util::Function<SendResult, std::span<const char>> T>
	inline auto shipRemainingInBuffer(std::span<char> buffer, i32 bufferIndex, T onBufferFilled) -> SendResult {
		if (bufferIndex == 0) return SendResult::BAD;
		return onBufferFilled(buffer.subspan(0, bufferIndex));
	}

	auto writeMessage(char * buffer, const std::vector<char> & data) -> SendResult {
		auto bufferIndex = 0;
		auto bufferView = std::span { buffer, BUFFER_LENGTH };

		auto send = [&](std::span<const char> buffer) {
			if (::send(BackSocket::clientSocket, buffer.data(), buffer.size(), 0) == SOCKET_ERROR)
				return SendResult::BAD;
			return SendResult::GOOD;
		};

		char header[4] = {
			(char)(data.size() >> 24),
			(char)((data.size() >> 16) & 0xff),
			(char)((data.size() >> 8) & 0xff),
			(char)(data.size() & 0xff),
		};

		if (shipIntoBuffer(bufferView, bufferIndex, { header, 4 }, send)) return SendResult::BAD;

		if (shipIntoBuffer(bufferView, bufferIndex, { data.data(), data.size() }, send)) return SendResult::BAD;

		return shipRemainingInBuffer(bufferView, bufferIndex, send);
	}

public:
	explicit Socket() :
		currentMessageIndex(0),
		currentMessageLength(0_u32),
		outQueue(),
		currentMessage(),
		running(false),
		readerThread(),
		writerThread(),
		outQueueMutex(),
		outQueueSignal()
	{}

	template<Util::Function<void, std::vector<char> &> OnReadFunction>
	auto init(const char * port, OnReadFunction onRead) -> void {
		if (WSAStartup(MAKEWORD(2, 2), &BackSocket::wsaData) != 0)
			throw std::exception("Could not start socket service");

		BackSocket::listenSocket = INVALID_SOCKET;

		addrinfo * addressInfo = nullptr;

		auto hints = addrinfo {
			.ai_flags = AI_PASSIVE,
			.ai_family = AF_INET,
			.ai_socktype = SOCK_STREAM,
			.ai_protocol = IPPROTO_TCP,
		};

		if(getaddrinfo(nullptr, port, &hints, &addressInfo)) {
			errorCleanup(addressInfo);
			throw std::exception("Could not resolve address");
		}

		BackSocket::listenSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

		if (BackSocket::listenSocket == INVALID_SOCKET) {
			errorCleanup(addressInfo);
			throw std::exception("could not initialize socket");
		}

		if (bind(BackSocket::listenSocket, addressInfo->ai_addr, addressInfo->ai_addrlen) == SOCKET_ERROR) {
			errorCleanup(addressInfo);
			throw std::exception("could not initialize socket");
		}

		freeaddrinfo(addressInfo);

		if (listen(BackSocket::listenSocket, SOMAXCONN) == SOCKET_ERROR) {
			errorCleanup(addressInfo);
			throw std::exception("could not listen on socket");
		}

		running = true;

		readerThread = std::thread([&, this]() {
			while (running) {
				if (BackSocket::clientSocket == INVALID_SOCKET) {
					BackSocket::clientSocket = accept(BackSocket::listenSocket, nullptr, nullptr);
				}

				if (BackSocket::clientSocket == INVALID_SOCKET) {
					std::cout << WSAGetLastError() << std::endl;
					return;
				}

				char tempBuffer[BUFFER_LENGTH];

				auto bytesReceived = recv(BackSocket::clientSocket, tempBuffer, BUFFER_LENGTH, 0);

				/* negative numbers are error codes */
				if (bytesReceived < 0) {
					resetCurrentMessage();
					std::cerr << "socket receive error code " << bytesReceived << std::endl;

					/* client disconnected */
					if (bytesReceived == -1) BackSocket::clientSocket = INVALID_SOCKET;

				} else if (bytesReceived > 0) {
					std::cout << "received " << bytesReceived << " bytes" << std::endl;

					for (auto i = 0; i < bytesReceived; readIntoMessage(tempBuffer, bytesReceived, i, onRead));
				}
			}
		});
		writerThread = std::thread([this]() {
			char tempBuffer[BUFFER_LENGTH];

			while (running) {
				auto lock = std::unique_lock(outQueueMutex);
				outQueueSignal.wait(lock, [this] { return !outQueue.empty(); });

				while (!outQueue.empty()) {
					if (writeMessage(tempBuffer, outQueue.front()) == SendResult::BAD) {
						BackSocket::clientSocket = INVALID_SOCKET;
						outQueue.clear();
					} else {
						outQueue.pop_front();
					}
				}
			}
		});
	}

	auto isConnected() -> bool {
		return BackSocket::clientSocket != INVALID_SOCKET;
	}

	auto close() -> void {
		running = false;

		readerThread.join();
		writerThread.join();

		closesocket(BackSocket::listenSocket);
	}


	template<Util::IsIterator<char> Iterator>
	auto send(Iterator begin, Iterator end) -> void {
		auto lockGuard = std::lock_guard(outQueueMutex);

		outQueue.emplace_back();
		auto & message = outQueue.back();
		message.assign(begin, end);

		outQueueSignal.notify_all();
	}
};

#endif //ALIFE_SOCKET_H
