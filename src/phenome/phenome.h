//
// Created by Emmet on 11/10/2022.
//

#pragma once

#ifndef ALIFE_PHENOME_H
#define ALIFE_PHENOME_H

#include <vector>

#include "types.h"
#include "src/genome/genome.h"
#include "reaction.h"
#include "body.h"

class Sense {
public:
	i32 x;
	i32 y;
	/* the body part code at this x, y in the organism */
	i32 sense;

	Sense(i32 x, i32 y, i32 sense);
};

class FoodStats {
public:
	i32 digestionBonus;
	i32 absorptionBonus;

	FoodStats();

	enum Type {
		DIGESTION = 0,
		ABSORPTION = 1,
	};
};

class Phenome {
public:
	Body body;

	enum MutationType {
		SUBSTITUTION = 0,
		INSERTION = 1,
		DELETION = 2,
		ALL = 3,
	};

	/**
	 * these are not the actual mutation rates
	 * the mutations rates are calculated later
	 * these are just ints some distance away from 0
	 * negative for a smaller rate, positive for a larger rate
	 */
	i32 mutationModifiers[3] { 0, 0, 0 };

	FoodStats foodStats[4];

	/**
	 * indices of the sense cells within the body for quick lookup
	 */
	std::vector<Sense> senses;

	std::vector<EyeReaction> eyeReactions;
	std::vector<EnvironmentReaction> environmentReactions;

	/**
	 * the phenome is entirely derived from the genome
	 */
	explicit Phenome(Genome &);
};

#endif //ALIFE_PHENOME_H
