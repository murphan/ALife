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
#include "sense.h"

class Phenome {
public:
	Body body;

	/**
	 * these are not the actual mutation rates
	 * the mutations rates are calculated later
	 * these are just ints some distance away from 0
	 * negative for a smaller rate, positive for a larger rate
	 */
	i32 insertionModifier;
	i32 selectionModifier;
	i32 substitutionModifier;

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
