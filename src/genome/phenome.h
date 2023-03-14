//
// Created by Emmet on 11/10/2022.
//

#pragma once

#ifndef ALIFE_PHENOME_H
#define ALIFE_PHENOME_H

#include <vector>

#include "types.h"
#include "genome/genome.h"
#include "genome/gene/reactionGene.h"
#include "body.h"
#include "genome/gene/mutationRateGene.h"

class Sense {
public:
	i32 x;
	i32 y;
	BodyPart senseCell;

	Sense(i32 x, i32 y, BodyPart senseCell);
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
	i32 mutationModifiers[3] { 0, 0, 0 };

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

	auto maxAge(i32 ageFactor) const -> i32;
};

#endif //ALIFE_PHENOME_H
