//
// Created by Emmet on 2/3/2023.
//

module;

#include "json.hpp"

export module MessageCreator;

using json = nlohmann::json;

export class MessageCreator {
public:
	static auto frameMessage(json && frameBody) -> json  {
		return {
			{ "type", "frame" },
			{ "frame", std::move(frameBody) },
		};
	};

	static auto initMessage(json && frameBody, json && controlsBody, json && settingsBody) -> json {
		return {
			{ "type", "init" },
			{ "frame", std::move(frameBody) },
			{ "controls", std::move(controlsBody) },
			{ "settings", std::move(settingsBody) },
		};
	};

	static auto controlsMessage(json && controlsBody) -> json {
		return {
			{ "type", "controls" },
			{ "controls", std::move(controlsBody) },
		};
	}

	static auto controlsMessageAndFrame(json && controlsBody, json && frameBody) -> json {
		return {
			{ "type", "controls" },
			{ "controls", std::move(controlsBody) },
			{ "frame", std::move(frameBody) },
		};
	}

	static auto settingsMessage(json && settingsBody) -> json {
		return {
			{ "type", "settings" },
			{ "settings", std::move(settingsBody) },
		};
	}
};
