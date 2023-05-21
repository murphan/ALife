//
// Created by Emmet on 3/9/2023.
//

module;

#define _USE_MATH_DEFINES
#include "OpenSimplex2S.hpp"
#include "json.hpp"

export module Noise;

import Types;

using json = nlohmann::json;

OpenSimplex2S noiseSource = OpenSimplex2S(324);

auto floatToByte(f32 in) -> i8 {
	auto converted = (i32)(in * 127);
	if (converted > 127) return 127_i8;
	if (converted < -128) return (i8)(-128);
	return (i8)converted;
}

export class Noise {
private:
	i32 id;

public:
	explicit Noise(i32 id, bool useNoise, f32 center, f32 speed, f32 scale, f32 amplitude) :
		id(id),
		along(0._f64),
		useNoise(useNoise),
		center(center),
		speed(speed),
		scale(scale),
		amplitude(amplitude) {};

	f64 along;

	bool useNoise;
	f32 center;
	f32 speed;
	f32 scale;
	f32 amplitude;

	auto tick() -> void {
		along += speed / 1000.0_f32;
		along = fmod(along, 2._f64 * M_PI);
	}

	[[nodiscard]] auto getValue(i32 x, i32 y) const -> i8  {
		if (!useNoise) return floatToByte(center);

		return floatToByte(
			center + (f32)noiseSource.noise4_XYBeforeZW(
				(f32)x / scale,
				(f32)y / scale,
				sin(along) * 100 + (id * 200),
				cos(along) * 100
			) * amplitude
		);
	}

	[[nodiscard]] auto serialize() const -> json {
		return {
			{ "useNoise", useNoise },
			{ "center", center },
			{ "speed", speed },
			{ "scale", scale },
			{ "amplitude", amplitude },
		};
	}
};
