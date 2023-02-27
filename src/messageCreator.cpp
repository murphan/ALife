//
// Created by Emmet on 2/3/2023.
//

#include "messageCreator.h"

auto MessageCreator::frameMessage(json && environmentBody) -> json {
	return json {
		{ "type", "frame" },
		{ "environment", std::move(environmentBody) },
	};
}

auto MessageCreator::initMessage(json && environmentBody, json && controlsBody) -> json {
	return json {
		{ "type", "init" },
		{ "environment", std::move(environmentBody) },
		{ "control", std::move(controlsBody) },
	};
}

auto MessageCreator::controlsMessage(json && controlsBody) -> json {
	return json {
		{ "type", "control" },
		{ "control", std::move(controlsBody) },
	};
}
