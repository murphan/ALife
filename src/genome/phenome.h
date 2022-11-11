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
#include "body.h"

class Phenome {
public:
	Body body;

	f32 mutationRate;

	std::vector<Action> actions;

	explicit Phenome(Genome &);
};

#endif //ALIFE_PHENOME_H
