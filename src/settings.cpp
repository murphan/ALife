
module;

#include "json.hpp"

export module Settings;

import Types;
import Factor;
import Noise;

using json = nlohmann::json;

auto parseNoise(json & body, Noise & noise) -> void {
	noise.useNoise = body["useNoise"].get<bool>();
	noise.center = body["center"].get<f32>();
	noise.speed = body["speed"].get<f32>();
	noise.scale = body["scale"].get<f32>();
	noise.amplitude = body["amplitude"].get<f32>();
}

export class Settings {
public:
	/*
	 * how many ticks and organism lives multiplied by its number of cells
	 * suggested range: 1-128
	 */
	i32 lifetimeFactor;
	/*
	 * how much energy a photosynthesis cell generates each tick when it receives light
	 * suggseted range: 1-16
	 */
	i32 photosynthesisFactor;
	/*
	 * energy that an organism is born with
	 * suggested range: 1-128
	 */
	i32 startingEnergy;
	/*
	 * energy expended on reproduction
	 * suggested range: 1-128
	 */
	i32 reproductionCost;
	/*
	 * energy that must be kept after reproducing
	 * suggested range: 1-128
	 */
	i32 reproductionThreshold;
	/*
	 * percentage of each cell's energy that gets transferred to the food when an organism dies
	 * suggested range: 0-1
	 */
	f32 foodEfficiency;
	/*
	 * chance that a mutation happens
	 * suggested range: 0-0.1
	 */
	f32 baseMutationRates[3];
	/*
	 * change to mutation rate based on each point from mutation rate genes
	 * formula: mutationRate = baseMutationRate * (mutationFactor)^points
	 * suggested range: 1.0-2.0
	 */
	f32 mutationFactor;
	i32 sightRange;
	i32 weaponDamage;
	i32 armorPrevents;
	i32 moveCost;
	i32 baseMoveLength;
	bool needEnergyToMove;
	i32 crushTime;

	i32 bodyPartCosts[7];
	/* not all parts can be upgraded, but list them anyway */
	i32 upgradedPartCosts[7];

	Noise factors[1];

	auto handleSettingsMessage(json & body) -> void {
		auto && settings = body["settings"];

		if (settings.contains("factors")) {
			auto && inFactors = settings["factors"];

			for (auto it = inFactors.begin(); it != inFactors.end(); ++it) {
				auto index = it - inFactors.begin();
				parseNoise(*it, this->factors[index]);
			}
		}
	}

	[[nodiscard]] auto serialize() const -> json {
		return {
			{ "factors", { factors[Factor::LIGHT].serialize() } },
			{ "lifetimeFactor", lifetimeFactor },
		};
	}
};
