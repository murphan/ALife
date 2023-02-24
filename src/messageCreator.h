//
// Created by Emmet on 2/3/2023.
//

#ifndef ALIFE_SERIALIZER_H
#define ALIFE_SERIALIZER_H

#include <vector>

#include <json.hpp>

#include "types.h"

#include "environment/controls.h"
#include "environment/environment.h"
#include "environment/organism.h"

using json = nlohmann::json;

class MessageCreator {
public:
	static auto frameMessage(json && environmentBody) -> json;

	static auto initMessage(json && environmentBody, json && controlsBody) -> json;
};

#endif //ALIFE_SERIALIZER_H
