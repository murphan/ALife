//
// Created by Emmet on 2/14/2023.
//

#ifndef ALIFE_ORGANISMSEEDER_H
#define ALIFE_ORGANISMSEEDER_H

#include <vector>
#include <random>

#include "organism.h"
#include "environment.h"
#include "settings.h"
#include "organismGrid.h"
#include "ids.h"

class OrganismSeeder {
public:
	static auto insertInitialOrganisms(
		std::vector<Organism> & insertList,
		const Environment & environment,
		const Phenome & initialPhenome,
		const Settings & settings,
		i32 count,
		std::default_random_engine & random,
		Ids & ids
	) -> void;
};

#endif //ALIFE_ORGANISMSEEDER_H
