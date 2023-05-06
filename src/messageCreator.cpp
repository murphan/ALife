//
// Created by Emmet on 2/3/2023.
//

#include "messageCreator.h"

auto MessageCreator::frameMessage(json && environmentBody) -> json {
	return {
		{ "type", "frame" },
		{ "environment", std::move(environmentBody) },
	};
}

auto MessageCreator::initMessage(json && environmentBody, json && controlsBody, json && settingsBody) -> json {
	return {
		{ "type", "init" },
		{ "environment", std::move(environmentBody) },
		{ "control", std::move(controlsBody) },
		{ "settings", std::move(settingsBody) },
	};
}

auto MessageCreator::controlsMessage(json && controlsBody) -> json {
	return {
		{ "type", "control" },
		{ "control", std::move(controlsBody) },
	};
}

auto MessageCreator::emptyOrganismRequestMessage() -> json {
	return {
		{ "type", "request" },
		{ "organism", nullptr },
	};
}

auto MessageCreator::organismRequestMessage(json && organismBody) -> json {
	return {
		{ "type", "request" },
		{ "organism", organismBody },
	};
}

auto MessageCreator::settingsMessage(json && settingsBody) -> json {
	return {
		{ "type", "settings" },
		{ "settings", settingsBody },
	};
}

auto MessageCreator::treeMessage(json && treeBody) -> json {
	return {
		{ "type", "tree" },
		{ "tree", treeBody },
	};
}
