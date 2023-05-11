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
#include "genome/gene/eyeGene.h"
#include "body.h"
#include "genome/gene/mutationRateGene.h"
#include "../environment/settings.h"

class Phenome {
public:
	Genome genome;
	Body body;

	/** cells this organism had originally */
	i32 numAliveCells;
	/** how much energy it takes to produce this organism */
	i32 baseBodyEnergy;

	/**
	 * these are not the actual mutation rates
	 * the mutations rates are calculated later
	 * these are just ints some distance away from 0
	 * negative for a smaller rate, positive for a larger rate
	 */
	i32 mutationModifiers[3];

	/**
	 * how many mover cells this organism has,
	 * determines how many tries it gets in a turn to find a valid move
	 */
	i32 moveTries;

	i32 moveLength;
	i32 regenFrequency;

	std::vector<EyeGene> reactions;

	explicit Phenome(Genome && inGenome, Body && inBody, Settings & settings);

	Phenome(const Phenome & other) = default;
	Phenome(Phenome && other) = default;
	auto operator=(Phenome && other) noexcept -> Phenome & = default;

	auto onAddCell(Body::Cell & cell) -> void;
	auto onKilledCell(Body::Cell & cell) -> void;
};

#endif //ALIFE_PHENOME_H
