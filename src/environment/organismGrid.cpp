//
// Created by Emmet on 2/10/2023.
//
#include "organismGrid.h"

OrganismGrid::OrganismGrid(i32 width, i32 height) : blankSpace(Space::makeEmpty()), width(width), height(height), grid(width * height, Space::makeEmpty()) {}

auto OrganismGrid::inBounds(i32 x, i32 y) const -> bool {
	return x >= 0 && x < width && y >= 0 && y < height;
}

auto OrganismGrid::indexOf(i32 x, i32 y) const -> i32 {
	return y * width + x;
}

auto OrganismGrid::internalSpaceAvailable(const Body & body, i32 index, i32 centerX, i32 centerY, Direction rotation) -> bool {
	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = centerY + j;
			auto x = centerX + i;

			if (!inBounds(x, y)) return false;

			auto cell = body.access(i, j, rotation).bodyPart();

			if (cell != BodyPart::NONE) {
				auto gridSpace = grid[indexOf(x, y)];
				/* don't encroach onto an existing other organism */
				if (gridSpace.filled() && gridSpace.index() != index) return false;
			}
		}
	}

	return true;
}

auto OrganismGrid::canMoveOrganism(const Organism &organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool {
	return internalSpaceAvailable(
		organism.body(),
		index, organism.x + deltaX,
		organism.y + deltaY,
		organism.rotation.rotate(deltaRotation)
	);
}

auto OrganismGrid::clear() -> void {
	for (auto & space : grid) {
		space = Space::makeEmpty();
	}
}

auto OrganismGrid::placeOrganism(const Organism & organism, i32 index) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {

			auto y = organism.y + j;
			auto x = organism.x + i;

			auto cell = body.access(i, j, rotation);

			if (cell.bodyPart() != BodyPart::NONE) {
				grid[indexOf(x, y)] = Space::makeCell(index, cell);
			}
		}
	}
}

/**
 * modifies the passed organism's position and rotation
 */
auto OrganismGrid::moveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	/* erase the current organism position */
	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j;
			auto x = organism.x + i;

			if (grid[indexOf(x, y)].index() == index) {
				grid[indexOf(x, y)].makeEmpty();
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

			if (cell.bodyPart() != BodyPart::NONE) {
				grid[indexOf(x, y)] = Space::makeCell(index, cell);
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

auto OrganismGrid::getWidth() const -> i32 {
	return width;
}

auto OrganismGrid::getHeight() const -> i32 {
	return height;
}

auto OrganismGrid::access(i32 x, i32 y) -> Space & {
	return grid[indexOf(x, y)];
}

auto OrganismGrid::accessSafe(i32 x, i32 y) const -> const Space & {
	if (x < 0 || x >= width || y < 0 || y >= height) return blankSpace;
	return grid[indexOf(x, y)];
}

OrganismGrid::Space::Space(u32 value, Body::Cell cell) : value(value), internalCell(cell) {}

auto OrganismGrid::Space::makeEmpty() -> Space {
	return Space(0, Body::Cell::makeEmpty());
}

auto OrganismGrid::Space::makeCell(i32 index, Body::Cell cell) -> Space {
	return Space(index + 1, cell);
}

auto OrganismGrid::Space::filled() const -> bool {
	return value != 0;
}

auto OrganismGrid::Space::index() const -> i32 {
	return (i32)(value - 1);
}

auto OrganismGrid::Space::cell() const -> Body::Cell {
	return internalCell;
}
