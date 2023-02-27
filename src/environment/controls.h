//
// Created by Emmet on 2/23/2023.
//

#ifndef ALIFE_CONTROLS_H
#define ALIFE_CONTROLS_H

#include <json.hpp>

#include "types.h"

using json = nlohmann::json;

class Controls {
public:
	bool playing;
	i32 fps;
	bool updateDisplay;

	auto serialize() const -> json;

	auto updateFromSerialized(json & input) -> void;

	auto unlimitedFPS() const -> bool;
};

#endif //ALIFE_CONTROLS_H
