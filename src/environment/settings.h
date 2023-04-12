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

	i32 lifetimeFactor;
	i32 energyFactor;
	i32 photosynthesisFactor;

	/* percentage of body energy that organism's start with */
	f32 startingEnergy;
	/* percentage of body energy additionally expended to reproduce */
	f32 reproductionCost;
	/* percentage of body energy that must be left after reproduction */
	f32 reproductionThreshold;

	/* how much of a body's energy gets converted into food */
	f32 foodEfficiency;
	/* how long before a food is removed */
	i32 maxFoodAge;

	f32 baseMutationRate;
	f32 mutationFactor;

	Noise factorNoises[3];

	auto handleSettingsMessage(json & body) -> void;

	auto serialize() const -> json;
};

#endif //ALIFE_SETTINGS_H
