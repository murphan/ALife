//
// Created by Emmet on 2/10/2023.
//
#include "organismGrid.h"

OrganismGrid::OrganismGrid(i32 width, i32 height) : width(width), height(height), grid(width * height, 0) {}

auto OrganismGrid::inBounds(i32 x, i32 y) const -> bool {
	return x >= 0 && x < width && y >= 0 && y < height;
}

auto OrganismGrid::indexOf(i32 x, i32 y) const -> i32 {
	return y * width + x;
}

auto OrganismGrid::internalSpaceAvailable(const Body & body, i32 tempId, i32 centerX, i32 centerY, Direction rotation) -> bool {
	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = centerY + j;
			auto x = centerX + i;

			if (!inBounds(x, y)) return false;

			auto cell = body.access(i, j, rotation);

			if (cell != BodyPart::NONE) {
				auto gridSpace = grid[indexOf(x, y)];
				/* don't encroach onto an existing other organism */
				if (
					OrganismGridSpace::getFilled(gridSpace) &&
					OrganismGridSpace::getTempId(gridSpace) != tempId
				) return false;
			}
		}
	}

	return true;
}

auto OrganismGrid::clear() -> void {
	for (auto & space : grid) {
		space = OrganismGridSpace::makeEmpty();
	}
}

auto OrganismGrid::placeOrganism(const Organism & organism, i32 tempId) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {

			auto y = organism.y + j;
			auto x = organism.x + i;

			auto cell = body.access(i, j, rotation);

			if (cell != BodyPart::NONE) {
				grid[indexOf(x, y)] = OrganismGridSpace::makeBodyPart(tempId, cell);
			}
		}
	}
}

auto OrganismGrid::canMoveOrganism(const Organism & organism, i32 tempId, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool {
	return internalSpaceAvailable(
		organism.body(),
		tempId, organism.x + deltaX,
		organism.y + deltaY,
		organism.rotation.rotate(deltaRotation)
		);
}

/**
 * modifies the passed organism's position and rotation
 */
auto OrganismGrid::moveOrganism(Organism & organism, i32 tempId, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	/* erase the current organism position */
	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j;
			auto x = organism.x + i;

			if (grid[indexOf(x, y)] == tempId) {
				grid[indexOf(x, y)] = OrganismGridSpace::makeEmpty();
			}
		}
	}

	auto newRotation = rotation.rotate(deltaRotation);

	/* place the organism again */
	for (auto j = body.getDown(newRotation); j <= body.getUp(newRotation); ++j) {
		for (auto i = body.getLeft(newRotation); i <= body.getRight(newRotation); ++i) {
			auto y = organism.y + j + deltaY;
			auto x = organism.x + i + deltaX;

			auto cell = body.access(i, j, newRotation);

			if (cell != BodyPart::NONE) {
				grid[indexOf(x, y)] = OrganismGridSpace::makeBodyPart(tempId, cell);
			}
		}
	}

	organism.x += deltaX;
	organism.y += deltaY;
	organism.rotation = newRotation;
}

auto OrganismGrid::isSpaceAvailable(const Body & body, i32 x, i32 y, Direction rotation) -> bool {
	return internalSpaceAvailable(body, -1, x, y, rotation);
}

auto OrganismGrid::organismInSpace(i32 x, i32 y) const -> bool {
	return OrganismGridSpace::getFilled(grid[indexOf(x, y)]);
}

auto OrganismGridSpace::makeEmpty() -> u32 {
	return 0;
}

auto OrganismGridSpace::makeBodyPart(i32 tempId, BodyPart bodyPart) -> u32 {
	return 0x1 | (tempId << 1) | ((bodyPart - 1) << 29);
}

auto OrganismGridSpace::getFilled(u32 space) -> bool {
	return (space & 0x1) == 0x1;
}

auto OrganismGridSpace::getTempId(u32 space) -> i32 {
	return (i32)((space >> 1) & ~(~0 << 28));
}

auto OrganismGridSpace::getBodyPart(u32 space) -> BodyPart {
	return (BodyPart)((space >> 29) + 1);
}
