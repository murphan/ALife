//
// Created by Emmet on 2/10/2023.
//
#include "organismGrid.h"

OrganismGrid::Space::Space(i8 x, i8 y, u32 value, Body::Cell cell) : x(x), y(y), value(value), internalCell(cell) {}

auto OrganismGrid::Space::makeEmpty() -> Space {
	return Space(0, 0, 0, Body::outOfBounds);
}

auto OrganismGrid::Space::makeCell(i8 x, i8 y, i32 index, Body::Cell cell) -> Space {
	return Space(x, y, index + 1, cell);
}

auto OrganismGrid::Space::makeFood(Body::Cell cell) -> OrganismGrid::Space {
	return OrganismGrid::Space(0, 0, FOOD_VALUE, cell);
}

auto OrganismGrid::Space::filled() const -> bool {
	return value != 0;
}

auto OrganismGrid::Space::isFood() const -> bool {
	return value == FOOD_VALUE;
}

auto OrganismGrid::Space::index() const -> i32 {
	return (i32)(value - 1);
}

auto OrganismGrid::Space::cell() -> Body::Cell & {
	return internalCell;
}

auto OrganismGrid::Space::getOriginalCell(Organism & organism) -> Body::Cell & {
	return organism.body().directAccess(x, y);
}

auto OrganismGrid::Space::getX() const -> i32 {
	return x;
}

auto OrganismGrid::Space::getY() const -> i32 {
	return y;
}

OrganismGrid::OrganismGrid(i32 width, i32 height) : blankSpace(Space::makeEmpty()), width(width), height(height), grid(width * height, Space::makeEmpty()) {}

auto OrganismGrid::inBounds(i32 x, i32 y) const -> bool {
	return x >= 0 && x < width && y >= 0 && y < height;
}

auto OrganismGrid::indexOf(i32 x, i32 y) const -> i32 {
	return y * width + x;
}

auto OrganismGrid::internalSpaceAvailable(Body & body, i32 index, i32 centerX, i32 centerY, Direction rotation) -> bool {
	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = centerY + j;
			auto x = centerX + i;

			if (!inBounds(x, y)) return false;

			auto && cell = body.access(i, j, rotation).bodyPart();

			if (cell != BodyPart::NONE) {
				auto gridSpace = grid[indexOf(x, y)];
				/* don't encroach onto an existing other organism */
				if (gridSpace.filled() && gridSpace.index() != index) return false;
			}
		}
	}

	return true;
}

auto OrganismGrid::canMoveOrganism(Organism &organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool {
	return internalSpaceAvailable(
		organism.body(),
		index, organism.x + deltaX,
		organism.y + deltaY,
		organism.rotation.rotate(deltaRotation)
	);
}

auto OrganismGrid::placeOrganism(Organism & organism, i32 index) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j;
			auto x = organism.x + i;

			auto && [cell, accessX, accessY] = body.accessCoord(i, j, rotation);

			if (cell.bodyPart() != BodyPart::NONE) {
				grid[indexOf(x, y)] = Space::makeCell((i8)accessX, (i8)accessY, index, cell);
			}
		}
	}
}

auto OrganismGrid::eraseOrganism(Organism & organism, i32 index) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j;
			auto x = organism.x + i;

			if (grid[indexOf(x, y)].index() == index) {
				grid[indexOf(x, y)] = Space::makeEmpty();
			}
		}
	}
}

/**
 * modifies the passed organism's position and rotation
 */
auto OrganismGrid::moveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void {
	eraseOrganism(organism, index);

	organism.x += deltaX;
	organism.y += deltaY;
	organism.rotation = organism.rotation.rotate(deltaRotation);

	placeOrganism(organism, index);
}

auto OrganismGrid::isSpaceAvailable(Body & body, i32 x, i32 y, Direction rotation) -> bool {
	return internalSpaceAvailable(body, -1, x, y, rotation);
}

auto OrganismGrid::getWidth() const -> i32 {
	return width;
}

auto OrganismGrid::getHeight() const -> i32 {
	return height;
}

auto OrganismGrid::accessUnsafe(i32 x, i32 y) -> Space & {
	return grid[indexOf(x, y)];
}

auto OrganismGrid::access(i32 x, i32 y) -> Space & {
	if (x < 0 || x >= width || y < 0 || y >= height) return blankSpace;
	return grid[indexOf(x, y)];
}
