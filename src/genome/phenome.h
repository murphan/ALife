//
// Created by Emmet on 11/10/2022.
//

#pragma once

#ifndef ALIFE_PHENOME_H
#define ALIFE_PHENOME_H

#include <vector>

#include "types.h"
#include "genome.h"
#include "action.h"

class Phenome {
public:
	std::vector<i32> body;
	i32 width;
	i32 height;

	f32 mutationRate;

	std::vector<Action> actions;

	explicit Phenome(Genome &);

	auto createGenome() -> Genome;
};

#endif //ALIFE_PHENOME_H
