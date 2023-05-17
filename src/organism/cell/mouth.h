//
// Created by Emmet on 4/27/2023.
//

#ifndef ALIFE_MOUTH_H
#define ALIFE_MOUTH_H

#include "util/types.h"
#include "organism/organism.h"
#include "environment/environment.h"
#include "environment/organismGrid.h"

namespace Mouth {
	auto tick(i32 x, i32 y, Organism & organism, i32 index, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void;

	auto tryEat(i32 x, i32 y, Organism & organism, i32 index, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms,  Settings & settings) -> void;

	auto eatCell(Organism & organism, Body::Cell & cell, Settings & settings) -> void;
}

#endif //ALIFE_MOUTH_H
