//
// Created by Emmet on 2/23/2023.
//

#ifndef ALIFE_MESSAGERECEIVER_H
#define ALIFE_MESSAGERECEIVER_H

#include <string>
#include <optional>
#include <json.hpp>

using json = nlohmann::json;

class MessageReceiver {
public:
	struct Message {
		std::string type;
		json body;

		Message(std::string && type, json && body);
	};

	static auto receive(std::string & message) -> std::optional<Message>;
};

#endif //ALIFE_MESSAGERECEIVER_H
