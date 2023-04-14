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
	 * percentage of body energy that organism's start with in addition to survival energy
	 * suggested range: 0-1
	 */
	f32 startingEnergy;
	/*
	 * percentage of body energy that an organism has to expend on top of expending all the energy that goes into the child
	 * suggested range: 0-1
	 */
	f32 reproductionCost;
	/*
	 * percentage of body energy that the parent has to have remaining in addition to their survival energy
	 * after they produce a child
	 * suggested range: 0-1
	 */
	f32 reproductionThreshold;
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

	Noise factorNoises[3];

	auto handleSettingsMessage(json & body) -> void;

	auto serialize() const -> json;
};

#endif //ALIFE_SETTINGS_H
