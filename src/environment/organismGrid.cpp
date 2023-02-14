//
// Created by Emmet on 2/10/2023.
//
#include "organismGrid.h"

OrganismGrid::OrganismGrid(i32 width, i32 height) : width(width), height(height), grid(width * height, 0) {}

auto OrganismGrid::inBounds(i32 x, i32 y) -> bool {
	return x >= 0 && x < width && y >= 0 && y < height;
}

auto OrganismGrid::indexOf(i32 x, i32 y) -> i32 {
	return y * width + x;
}

auto OrganismGrid::internalSpaceAvailable(const Body & body, i32 tempId, i32 centerX, i32 centerY) -> bool {
	for (auto j = body.down; j <= body.up; ++j) {
		for (auto i = body.left; i <= body.right; ++i) {
			auto y = centerY + j;
			auto x = centerX + i;

			if (!inBounds(x, y)) return false;

			auto cell = body.access(i, j);

			if (cell != BodyPart::NONE) {
				auto gridCell = grid[indexOf(x, y)];
				/* don't encroach onto an existing other organism */
				if (gridCell != 0 && gridCell != tempId) return false;
			}
		}
	}

	return true;
}

auto OrganismGrid::placeOrganism(Organism & organism, i32 tempId) -> void {
	auto && body = organism.body();

	for (auto j = body.down; j <= body.up; ++j) {
		for (auto i = body.left; i <= body.right; ++i) {
			auto y = organism.y + j;
			auto x = organism.x + i;

			if (!inBounds(x, y)) continue;

			auto cell = body.access(i, j);

			if (cell != BodyPart::NONE) {
				grid[indexOf(x, y)] = tempId;
			}
		}
	}
}

auto OrganismGrid::canMoveOrganism(Organism & organism, i32 tempId, i32 deltaX, i32 deltaY) -> bool {
	auto && body = organism.body();

	return internalSpaceAvailable(organism.body(), tempId, organism.x + deltaX, organism.y + deltaY);
}

/**
 * modifies the passed organism's position
 */
auto OrganismGrid::moveOrganism(Organism & organism, i32 tempId, i32 deltaX, i32 deltaY) -> void {
	auto && body = organism.body();

	/* erase the current organism position */
	for (auto j = body.down; j <= body.up; ++j) {
		for (auto i = body.left; i <= body.right; ++i) {
			auto y = organism.y + j;
			auto x = organism.x + i;

			grid[indexOf(x, y)] = 0;
		}
	}

	/* place the organism again */
	for (auto j = body.down; j <= body.up; ++j) {
		for (auto i = body.left; i <= body.right; ++i) {
			auto y = organism.y + j + deltaY;
			auto x = organism.x + i + deltaX;

			grid[indexOf(x, y)] = tempId;
		}
	}
}

auto OrganismGrid::isSpaceAvailable(const Body & body, i32 x, i32 y) -> bool {
	return internalSpaceAvailable(body, -1, x, y);
}

