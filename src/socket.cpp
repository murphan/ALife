//
// Created by Emmet on 1/30/2023.
//

#include <exception>
#include <iostream>

#include "socket.h"

auto Socket::wsaData = WSAData {0,};

auto Socket::listenSocket = INVALID_SOCKET;
auto Socket::clientSocket = INVALID_SOCKET;

auto Socket::readingMessage = false;
auto Socket::messageLengthIndex = 0;
auto Socket::currentMessageLength = 0_u32;

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
auto Socket::init(const char * address, const char * port) -> void {
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

	if(getaddrinfo(address, port, &hints, &addressInfo)) {
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
}

auto Socket::isConnected() -> bool {
	return clientSocket != INVALID_SOCKET;
}

auto Socket::close() -> void {
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
		storage.push_back(std::move(currentMessage));
		resetCurrentMessage();
	}
}

auto Socket::poll() -> void {
	if (clientSocket == INVALID_SOCKET) {
		clientSocket = accept(listenSocket, nullptr, nullptr);
	}

	if (clientSocket == INVALID_SOCKET) return;

	char tempBuffer[BUFFER_LENGTH];

	auto bytesReceived = recv(clientSocket, tempBuffer, BUFFER_LENGTH, 0);

	/* negative numbers are error codes */
	if (bytesReceived < 0) {
		resetCurrentMessage();
		std::cerr << "socket receive error code " << bytesReceived << std::endl;

	} else {
		for (auto i = 0; i < bytesReceived; readIntoMessage(tempBuffer, bytesReceived, i));
	}
}

auto Socket::queueMessage() -> std::optional<std::vector<char>> {
	if (storage.empty()) return std::nullopt;

	auto ret = std::make_optional(std::move(storage.front()));

	storage.pop_front();

	return ret;
}

auto Socket::send(std::vector<char> & data) -> void {
	char tempBuffer[BUFFER_LENGTH];

	auto i = 0;
	while (i < data.) {

	}

	if (::send(clientSocket, tempBuffer, 3, 0) == SOCKET_ERROR) {
		std::cerr << "socket send error " << std::endl;
	}
}