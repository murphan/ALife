//
// Created by Emmet on 3/9/2023.
//

#include "settings.h"

#include "factor.h"

inline auto parseNoise(Settings::json & body, Noise & noise) -> void {
	noise.useNoise = body["useNoise"].get<bool>();
	noise.center = body["center"].get<f32>();
	noise.speed = body["speed"].get<f32>();
	noise.scale = body["scale"].get<f32>();
	noise.amplitude = body["amplitude"].get<f32>();
}

auto Settings::handleSettingsMessage(Settings::json & body) -> void {
	auto && settings = body["settings"];

	if (settings.contains("factors")) {
		auto && inFactors = settings["factors"];

		for (auto it = inFactors.begin(); it != inFactors.end(); ++it) {
			auto index = it - inFactors.begin();
			parseNoise(*it, this->factors[index]);
		}
	}
}

auto Settings::serialize() const -> json {
	return {
		{ "factors", {
			factors[Factor::LIGHT].serialize()
		} },
		{ "lifetimeFactor", lifetimeFactor },
	};
}
