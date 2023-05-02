//
// Created by Emmet on 2/10/2023.
//
#include "organismGrid.h"
#include "../genome/rotation.h"

OrganismGrid::OrganismGrid(i32 width, i32 height) :
	blankSpace(Space::makeEmpty()),
	width(width),
	height(height),
	grid(width * height, Space::makeEmpty()) {}

auto OrganismGrid::inBounds(i32 x, i32 y) const -> bool {
	return x >= 0 && x < width && y >= 0 && y < height;
}

auto OrganismGrid::indexOf(i32 x, i32 y) const -> i32 {
	return y * width + x;
}

auto OrganismGrid::clear() -> void {
	for (auto && space : grid) {
		space = Space::makeEmpty();
	}
}

auto OrganismGrid::internalSpaceAvailable(Body & body, i32 index, i32 centerX, i32 centerY, Direction rotation) -> bool {
	return std::all_of(body.cells.begin(), body.cells.end(), [&](Body::Cell * cell) {
		auto [x, y] = Body::absoluteXY(*cell, centerX, centerY, rotation);

		if (!inBounds(x, y)) return false;

		auto && gridSpace = grid[indexOf(x, y)];
		if (gridSpace.isFilled() && gridSpace.index() != index) return false;

		return true;
	});
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
	for (auto && cell : organism.body().cells) {
		auto [x, y] = organism.absoluteXY(*cell);
		if (!cell->broken()) grid[indexOf(x, y)] = Space::makeCell(cell, index);
	}
}

auto OrganismGrid::eraseOrganism(Organism & organism, i32 index) -> void {
	for (auto && cell : organism.body().cells) {
		auto [x, y] = organism.absoluteXY(*cell);
		grid[indexOf(x, y)] = Space::makeEmpty();
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

OrganismGrid::Space::Space(Body::Cell * reference, u32 value):
    reference(reference), value(value) {}

auto OrganismGrid::Space::makeEmpty() -> OrganismGrid::Space {
	return OrganismGrid::Space(nullptr, 0);
}

auto OrganismGrid::Space::makeCell(Body::Cell * reference, i32 index) -> OrganismGrid::Space {
	return OrganismGrid::Space(reference, index + 1);
}

auto OrganismGrid::Space::makeFood(Body::Cell * reference) -> OrganismGrid::Space {
	return OrganismGrid::Space(reference, FOOD_VALUE);
}

auto OrganismGrid::Space::isFilled() const -> bool {
	return reference != nullptr && value != 0;
}

/** is filled AND is part of a living organism (even if dead) */
auto OrganismGrid::Space::fromOrganism() const -> bool {
	return reference != nullptr && value != 0 && value != FOOD_VALUE;
}

/** is filled AND is dropped as food, not part of an organism */
auto OrganismGrid::Space::fromEnvironment() const -> bool {
	return reference != nullptr && value == FOOD_VALUE;
}

auto OrganismGrid::Space::index() const -> i32 {
	return (i32)(value - 1);
}

auto OrganismGrid::Space::cell() const -> Body::Cell & {
	return *reference;
}
