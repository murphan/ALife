//
// Created by Emmet on 2/23/2023.
//

#include "messageReceiver.h"

#include "iostream"

auto MessageReceiver::receive(std::string & message) -> std::optional<Message> {
	auto jsonMessage = json::parse(message);

	if (!jsonMessage.contains("type")) {
		std::cout << "malformed message without type" << std::endl;
		return std::nullopt;
	}

	auto typeField = jsonMessage["type"];
	if (!typeField.is_string()) {
		std::cout << "malformed message without type" << std::endl;
		return std::nullopt;
	}

	std::string && type = typeField.get<std::string>();

	return std::make_optional<Message>(std::move(type), std::move(jsonMessage));
}

MessageReceiver::Message::Message(std::string && type, json && body): type(std::move(type)), body(std::move(body)) {}
