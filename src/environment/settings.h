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

	/*
	 * how many ticks and organism lives multiplied by its number of cells
	 * suggested range: 1-128
	 */
	i32 lifetimeFactor;
	/*
	 * the fundamental unit of energy, how much energy each cell is "worth"
	 * "body energy" refers to the number of cells * the energy factor
	 * suggested range: 1-128
	 */
	i32 energyFactor;
	/*
	 * how much energy a photosynthesis cell generates each tick when it receives light
	 * suggseted range: 1-16
	 */
	i32 photosynthesisFactor;
	/*
	 * additional cells worth of energy that an organism is born with
	 * suggested range: 1-8
	 */
	i32 startingEnergy;
	/*
	 * additional cells worth of energy that are expended on reproduction
	 * suggested range: 1-8
	 */
	i32 reproductionCost;
	/*
	 * additional cells worth of energy that must be kept after reproducing
	 * suggested range: 1-8
	 */
	i32 reproductionThreshold;
	/*
	 * percentage of each cell's energy that gets transferred to the food when an organism dies
	 * suggested range: 0-1
	 */
	f32 foodEfficiency;
	/*
	 * how long before a food is removed
	 * suggested range: 1-512
	 */
	i32 maxFoodAge;
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

	i32 bodyPartCosts[7];
	/* not all parts can be upgraded, but list them anyway */
	i32 upgradedPartCosts[7];

	Noise factorNoises[3];

	auto handleSettingsMessage(json & body) -> void;

	auto serialize() const -> json;
};

#endif //ALIFE_SETTINGS_H
