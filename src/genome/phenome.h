//
// Created by Emmet on 11/10/2022.
//

#pragma once

#ifndef ALIFE_PHENOME_H
#define ALIFE_PHENOME_H

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "types.h"
#include "genome/genome.h"
#include "genome/gene/reactionGene.h"
#include "body.h"
#include "genome/gene/mutationRateGene.h"
#include "../environment/settings.h"

class Sense {
public:
	i32 x;
	i32 y;
	BodyPart senseCell;
	Direction direction;

	Sense(i32 x, i32 y, BodyPart senseCell, Direction direction);

	static auto determineDirection(i32 x, i32 y) -> Direction {
		return (i32)floor(
			(
				Util::positiveMod(
					(f32)atan2(y, x),
					(f32)M_PI * 2.0_f32
				) + ((1.0_f32 / 8.0_f32) * M_PI)
			) / ((1.0_f32 / 4.0_f32) * M_PI)
		) % 8;
	}
};

struct FoodStats {
	i32 digestionBonus = 0;
	i32 absoprtionBonus = 0;
};

class Phenome {
public:
	Genome genome;
	Body body;

	/**
	 * these are not the actual mutation rates
	 * the mutations rates are calculated later
	 * these are just ints some distance away from 0
	 * negative for a smaller rate, positive for a larger rate
	 */
	i32 mutationModifiers[3];

	FoodStats foodStats[4];

	/**
	 * how many mover cells this organism has,
	 * determines how many tries it gets in a turn to find a valid move
	 */
	i32 moveTries;

	/**
	 * indices of the sense cells within the body for quick lookup
	 */
	std::vector<Sense> senses;

	std::vector<EyeGene> eyeReactions;
	std::vector<EnvironmentGene> environmentReactions;

	explicit Phenome(Genome && inGenome, Body && inBody);

	Phenome(const Phenome & other) = default;
	Phenome(Phenome && other) = default;
	auto operator=(Phenome && other) noexcept -> Phenome & = default;

	auto survivalEnergy(const Settings & settings) const -> i32;

	auto maxAge(i32 lifetimeFactor) const -> i32;
};

#endif //ALIFE_PHENOME_H
