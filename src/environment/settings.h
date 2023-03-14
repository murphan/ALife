//
// Created by Emmet on 3/9/2023.
//

#ifndef ALIFE_SETTINGS_H
#define ALIFE_SETTINGS_H

#include <json.hpp>

#include "noise/noise.h"

class Settings {
public:
	using json = nlohmann::json;

	i32 ageFactor;

	Noise factorNoises[3];

	auto handleSettingsMessage(json & body) -> void;

	auto serialize() const -> json;
};

#endif //ALIFE_SETTINGS_H
