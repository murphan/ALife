//
// Created by Emmet on 1/30/2023.
//

#include <exception>
#include <iostream>
#include <span>
#include <functional>

#include "socket.h"

#include "util.h"

auto Socket::wsaData = WSAData {0,};

auto Socket::listenSocket = INVALID_SOCKET;
auto Socket::clientSocket = INVALID_SOCKET;

auto Socket::messageLengthIndex = 0;
auto Socket::currentMessageLength = 0_u32;

auto Socket::inQueue = std::deque<std::vector<char>>();
auto Socket::outQueue = std::deque<std::vector<char>>();

auto Socket::currentMessage = std::vector<char>();

/* multithreading */

auto Socket::running = false;

auto Socket::readerThread = std::thread();
auto Socket::writerThread = std::thread();

auto Socket::inQueueMutex = std::mutex();
auto Socket::outQueueMutex = std::mutex();
auto Socket::outQueueSignal = std::condition_variable();

auto Socket::errorCleanup(addrinfo *& addressInfo) -> void {
	if (addressInfo != nullptr) freeaddrinfo(addressInfo);
	addressInfo = nullptr;

	if (listenSocket != INVALID_SOCKET) closesocket(listenSocket);
	listenSocket = INVALID_SOCKET;

	WSACleanup();
}

/**
 * @throw std::exception if could not be initialized
 */
auto Socket::init(const char * port) -> void {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw std::exception("Could not start socket service");

	listenSocket = INVALID_SOCKET;

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

	listenSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		errorCleanup(addressInfo);
		throw std::exception("could not initialize socket");
	}

	if (bind(listenSocket, addressInfo->ai_addr, addressInfo->ai_addrlen) == SOCKET_ERROR) {
		errorCleanup(addressInfo);
		throw std::exception("could not initialize socket");
	}

	freeaddrinfo(addressInfo);

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		errorCleanup(addressInfo);
		throw std::exception("could not listen on socket");
	}

	running = true;

	readerThread = std::thread(readerLoop);
	writerThread = std::thread(writerLoop);
}

auto Socket::isConnected() -> bool {
	return clientSocket != INVALID_SOCKET;
}

auto Socket::close() -> void {
	running = false;

	readerThread.join();
	writerThread.join();

	closesocket(listenSocket);
}

auto Socket::resetCurrentMessage() -> void {
	messageLengthIndex = 0;
	currentMessageLength = 0;
	currentMessage = {};
}

auto Socket::readIntoMessage(char * buffer, i32 bytesReceived, i32 & index) -> void {
	auto headerBytesToRead = min(4 - messageLengthIndex, bytesReceived);
	auto start = index;

	for (; index < start + headerBytesToRead; ++index) {
		currentMessageLength |= buffer[index] << ((3 - messageLengthIndex) * 8);
		++messageLengthIndex;
	}

	auto bodyBytesToRead = (i32)min(bytesReceived - index, currentMessageLength);

	currentMessage.insert(currentMessage.end(), buffer + index, buffer + bytesReceived);
	index += bodyBytesToRead;

	if (currentMessage.size() == currentMessageLength) {
		inQueue.push_back(std::move(currentMessage));
		resetCurrentMessage();
	}
}

auto Socket::readerLoop() -> void {
	while (running) {
		if (clientSocket == INVALID_SOCKET) {
			clientSocket = accept(listenSocket, nullptr, nullptr);
		}

		if (clientSocket == INVALID_SOCKET) {
			std::cout << WSAGetLastError() << std::endl;
			return;
		}

		char tempBuffer[BUFFER_LENGTH];

		auto bytesReceived = recv(clientSocket, tempBuffer, BUFFER_LENGTH, 0);

		/* negative numbers are error codes */
		if (bytesReceived < 0) {
			resetCurrentMessage();
			std::cerr << "socket receive error code " << bytesReceived << std::endl;

			/* client disconnected */
			if (bytesReceived == -1) clientSocket = INVALID_SOCKET;

		} else if (bytesReceived > 0) {
			std::cout << "received " << bytesReceived << " bytes" << std::endl;

			inQueueMutex.lock();
			for (auto i = 0; i < bytesReceived; readIntoMessage(tempBuffer, bytesReceived, i));
			inQueueMutex.unlock();
		}
	}
}

auto Socket::queueMessage() -> std::optional<std::vector<char>> {
	inQueueMutex.lock();

	if (inQueue.empty()) {
		inQueueMutex.unlock();
		return std::nullopt;
	}

	auto ret = std::make_optional(std::move(inQueue.front()));

	inQueue.pop_front();
	inQueueMutex.unlock();

	return ret;
}

auto Socket::writerLoop() -> void {
	char tempBuffer[BUFFER_LENGTH];

	while (running) {
		auto lock = std::unique_lock(outQueueMutex);
		outQueueSignal.wait(lock, [] { return !outQueue.empty(); });

		while (!outQueue.empty()) {
			writeMessage(tempBuffer, outQueue.front());
			outQueue.pop_front();
		}
	}
}

inline auto writePartIntoBuffer(std::span<char> buffer, i32 & bufferIndex, std::span<const char> data) -> i32 {
	auto leftInBuffer = buffer.size() - bufferIndex;
	auto writeSize = (i32)min(data.size(), leftInBuffer);

	memcpy_s(buffer.data() + bufferIndex, leftInBuffer, data.data(), writeSize);

	bufferIndex += writeSize;
	return writeSize;
}

enum SendResult {
	GOOD = false,
	BAD = true,
};

template<Util::Function<std::span<const char>, SendResult> T>
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

template<Util::Function<std::span<const char>, SendResult> T>
inline auto shipRemainingInBuffer(std::span<char> buffer, i32 bufferIndex, T onBufferFilled) {
	if (bufferIndex == 0) return;
	onBufferFilled(buffer.subspan(0, bufferIndex));
}

auto Socket::writeMessage(char * buffer, const std::vector<char> & data) -> void {
	auto bufferIndex = 0;
	auto bufferView = std::span { buffer, BUFFER_LENGTH };

	auto send = [&](std::span<const char> buffer) {
		if (::send(clientSocket, buffer.data(), buffer.size(), 0) == SOCKET_ERROR) {
			std::cerr << "socket send error " << std::endl;
			return SendResult::BAD;
		}
		return SendResult::GOOD;
	};

	char header[4] = {
		(char)(data.size() >> 24),
		(char)((data.size() >> 16) & 0xff),
		(char)((data.size() >> 8) & 0xff),
		(char)(data.size() & 0xff),
	};

	if (shipIntoBuffer(bufferView, bufferIndex, { header, 4 }, send)) return;

	if (shipIntoBuffer(bufferView, bufferIndex, { data.data(), data.size() }, send)) return;

	shipRemainingInBuffer(bufferView, bufferIndex, send);
}
