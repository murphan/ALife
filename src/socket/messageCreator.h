//
// Created by Emmet on 2/3/2023.
//

#ifndef ALIFE_SERIALIZER_H
#define ALIFE_SERIALIZER_H

#include <vector>

#include <json.hpp>

#include "util/types.h"

#include "controls.h"
#include "environment/environment.h"
#include "organism/organism.h"

using json = nlohmann::json;

class MessageCreator {
public:
	static auto frameMessage(json && frameBody) -> json;

	static auto initMessage(json && frameBody, json && controlsBody, json && settingsBody) -> json;

	static auto controlsMessage(json && controlsBody) -> json;
	static auto controlsMessageAndFrame(json && controlsBody, json && frameBody) -> json;

	static auto settingsMessage(json && settingsBody) -> json;

	static auto emptyOrganismRequestMessage() -> json;
	static auto organismRequestMessage(json && organismBody) -> json;
};

#endif //ALIFE_SERIALIZER_H
