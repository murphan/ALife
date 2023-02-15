//
// Created by Emmet on 2/14/2023.
//

#ifndef ALIFE_ORGANISMSEEDER_H
#define ALIFE_ORGANISMSEEDER_H

#include <vector>

#include "organism.h"
#include "environment.h"

class OrganismSeeder {
public:
	static auto insertInitialOrganisms(std::vector<Organism> & insertList, const Environment & environment, const Phenome & initialPhenome, i32 count) -> void;
};

#endif //ALIFE_ORGANISMSEEDER_H
