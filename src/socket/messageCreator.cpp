//
// Created by Emmet on 2/3/2023.
//

#include "messageCreator.h"

auto MessageCreator::frameMessage(json && frameBody) -> json {
	return {
		{ "type", "frame" },
		{ "frame", std::move(frameBody) },
	};
}

auto MessageCreator::initMessage(json && frameBody, json && controlsBody, json && settingsBody) -> json {
	return {
		{ "type", "init" },
		{ "frame", std::move(frameBody) },
		{ "controls", std::move(controlsBody) },
		{ "settings", std::move(settingsBody) },
	};
}

auto MessageCreator::controlsMessage(json && controlsBody) -> json {
	return {
		{ "type", "controls" },
		{ "controls", std::move(controlsBody) },
	};
}

auto MessageCreator::controlsMessageAndFrame(json && controlsBody, json && frameBody) -> json {
	return {
		{ "type", "controls" },
		{ "controls", std::move(controlsBody) },
		{ "frame", std::move(frameBody) },
	};
}

auto MessageCreator::settingsMessage(json && settingsBody) -> json {
	return {
		{ "type", "settings" },
		{ "settings", std::move(settingsBody) },
	};
}
