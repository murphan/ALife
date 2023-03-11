//
// Created by Emmet on 3/9/2023.
//

#include "settings.h"

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
		auto && factors = settings["factors"];

		for (auto it = factors.begin(); it != factors.end(); ++it) {
			auto index = it - factors.begin();
			parseNoise(*it, this->factorNoises[index]);
		}
	}
}
