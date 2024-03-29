//
// Created by Emmet on 4/27/2023.
//

#ifndef ALIFE_PHOTOSYNTHESIZER_H
#define ALIFE_PHOTOSYNTHESIZER_H

#include "util/types.h"
#include "organism/organism.h"
#include "environment/environment.h"
#include "environment/organismGrid.h"
#include <random>

namespace Photosynthesizer {
	auto tick(i32 x, i32 y, Organism & organism, Environment & environment, OrganismGrid & organismGrid, Settings & settings, std::vector<Organism> & organisms, std::default_random_engine & random) -> void;
}

#endif //ALIFE_PHOTOSYNTHESIZER_H
