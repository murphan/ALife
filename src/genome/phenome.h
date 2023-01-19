//
// Created by Emmet on 11/10/2022.
//

#pragma once

#ifndef ALIFE_PHENOME_H
#define ALIFE_PHENOME_H

#include <vector>

#include "types.h"
#include "src/genome/genome.h"
#include "src/genome/gene/reactionGene.h"
#include "body.h"
#include "src/genome/gene/mutationRateGene.h"

class Sense {
public:
	i32 x;
	i32 y;
	/* the body part code at this x, y in the organism */
	i32 sense;

	Sense(i32 x, i32 y, i32 sense);
};

struct FoodStats {
	i32 digestionBonus = 0;
	i32 absoprtionBonus = 0;
};

class Phenome {
public:
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
	 * indices of the sense cells within the body for quick lookup
	 */
	std::vector<Sense> senses;

	std::vector<EyeGene> eyeReactions;
	std::vector<EnvironmentGene> environmentReactions;

	/**
	 * the phenome is entirely derived from the genome
	 */
	explicit Phenome(Genome &);
};

#endif //ALIFE_PHENOME_H
