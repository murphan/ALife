//
// Created by Emmet on 3/9/2023.
//

#ifndef ALIFE_NOISE_H
#define ALIFE_NOISE_H

#include <json.hpp>

#include "util/types.h"
#include "OpenSimplex2S.hpp"

class Noise {
private:
	static OpenSimplex2S noiseSource;

	i32 id;

public:
	using json = nlohmann::json;

	Noise(i32 id, bool useNoise, f32 center, f32 speed, f32 scale, f32 amplitude);

	f64 along;

	bool useNoise;
	f32 center;
	f32 speed;
	f32 scale;
	f32 amplitude;

	auto tick() -> void;
	auto getValue(i32 x, i32 y) const -> i8;

	auto serialize() const -> json;
};

#endif //ALIFE_NOISE_H
