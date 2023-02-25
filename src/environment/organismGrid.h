//
// Created by Emmet on 2/10/2023.
//

#ifndef ALIFE_ORGANISMGRID_H
#define ALIFE_ORGANISMGRID_H

#include <vector>
#include "types.h"

#include "organism.h"

class OrganismGridSpace {
public:
	static auto makeEmpty() -> u32;
	static auto makeBodyPart(i32 tempId, BodyPart bodyPart) -> u32;

	static auto getFilled(u32 space) -> bool;
	static auto getTempId(u32 space) -> i32;
	static auto getBodyPart(u32 space) -> BodyPart;
};

class OrganismGrid {
private:
	i32 width, height;
	std::vector<u32> grid;

	auto inBounds(i32 x, i32 y) const -> bool;
	auto indexOf(i32 x, i32 y) const -> i32;

	auto internalSpaceAvailable(const Body & body, i32 tempId, i32 centerX, i32 centerY, Direction rotation) -> bool;

public:
	OrganismGrid(i32 width, i32 height);

	auto clear() -> void;

	auto placeOrganism(const Organism & organism, i32 tempId) -> void;

	auto canMoveOrganism(const Organism & organism, i32 tempId, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool;

	auto moveOrganism(Organism & organism, i32 tempId, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void;

	auto isSpaceAvailable(const Body & body, i32 x, i32 y, Direction rotation) -> bool;

    auto organismInSpace(i32 x, i32 y) const -> bool;

    auto getWidth() const -> i32;

    auto getHight() const -> i32;
};

#endif //ALIFE_ORGANISMGRID_H
