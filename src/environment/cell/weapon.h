//
// Created by Emmet on 4/27/2023.
//

#ifndef ALIFE_WEAPON_H
#define ALIFE_WEAPON_H

#include "types.h"
#include "environment/organism.h"
#include "environment/environment.h"
#include "environment/organismGrid.h"

namespace Weapon {
	enum BlockResult {
		BLOCK_NONE = 0x0,
		BLOCK_PART = 0x1,
		BLOCK_FULL = 0x2,
		BLOCK_FULL_BUT_COOLER = 0x2,
	};

	auto tick(i32 x, i32 y, i32 index, Body::Cell & weapon, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void;

	auto dealDamage(i32 x, i32 y, i32 superAttack, i32 index, Environment & environment, OrganismGrid & organismGrid, std::vector<Organism> & organisms, Settings & settings) -> void;

	auto armorDoesBlock(i32 x, i32 y, i32 superAttack, bool directAttack, i32 defenderIndex, OrganismGrid & organismGrid) -> BlockResult;

	auto killCell(Organism & organism, Body::Cell & cell) -> void;

	auto detachCell(Organism & organism, Environment & environment, Body::Cell & cell) -> void;
}

#endif //ALIFE_WEAPON_H
