//
// Created by Emmet on 2/10/2023.
//

#ifndef ALIFE_ORGANISMGRID_H
#define ALIFE_ORGANISMGRID_H

#include <vector>
#include "types.h"

#include "organism.h"

class OrganismGrid {
private:
	i32 width, height;
	std::vector<i32> grid;

	auto inBounds(i32 x, i32 y) -> bool;
	auto indexOf(i32 x, i32 y) -> i32;

	auto internalSpaceAvailable(const Body & body, i32 tempId, i32 centerX, i32 centerY) -> bool;

public:
	OrganismGrid(i32 width, i32 height);

	auto placeOrganism(const Organism & organism, i32 tempId) -> void;

	auto canMoveOrganism(const Organism & organism, i32 tempId, i32 deltaX, i32 deltaY) -> bool;

	auto moveOrganism(Organism & organism, i32 tempId, i32 deltaX, i32 deltaY) -> void;

	auto isSpaceAvailable(const Body & body, i32 x, i32 y) -> bool;
};

#endif //ALIFE_ORGANISMGRID_H
