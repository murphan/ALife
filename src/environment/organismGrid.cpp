//
// Created by Emmet on 2/10/2023.
//

#include "organismGrid.h"
#include "util/rotation.h"

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
	return std::all_of(body.getCells().begin(), body.getCells().end(), [&](const Body::Cell & cell) {
		auto [x, y] = Body::absoluteXY(cell, centerX, centerY, rotation);

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
	for (auto && cell : organism.body().getCells()) {
		auto [x, y] = organism.absoluteXY(cell);
		if (!cell.broken()) grid[indexOf(x, y)] = Space::makeCell(cell, index);
	}
}

auto OrganismGrid::eraseOrganism(Organism & organism, i32 index) -> void {
	for (auto && cell : organism.body().getCells()) {
		auto [x, y] = organism.absoluteXY(cell);
		if (!cell.broken()) grid[indexOf(x, y)] = Space::makeEmpty();
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

OrganismGrid::Space::Space(i32 x, i32 y, u32 value):
    x(x), y(y), value(value) {}

auto OrganismGrid::Space::makeEmpty() -> OrganismGrid::Space {
	return OrganismGrid::Space(0, 0, 0);
}

auto OrganismGrid::Space::makeCell(Body::Cell reference, i32 index) -> OrganismGrid::Space {
	return OrganismGrid::Space(reference.x(), reference.y(), index + 1);
}

auto OrganismGrid::Space::makeFood(i32 x, i32 y) -> OrganismGrid::Space {
	return OrganismGrid::Space(x, y, FOOD_VALUE);
}

auto OrganismGrid::Space::isFilled() const -> bool {
	return value != 0;
}

/** is filled AND is part of a living organism (even if dead) */
auto OrganismGrid::Space::fromOrganism() const -> bool {
	return isFilled() && !fromEnvironment();
}

/** is filled AND is dropped as food, not part of an organism */
auto OrganismGrid::Space::fromEnvironment() const -> bool {
	return value == FOOD_VALUE;
}

auto OrganismGrid::Space::index() const -> i32 {
	return (i32)(value - 1);
}

auto OrganismGrid::Space::cell(std::vector<Organism> & organisms) const -> Body::Cell & {
	if (fromEnvironment()) throw std::exception("space is not from organism");
	return organisms[index()].body().directAccess(x, y);
}

auto OrganismGrid::Space::cell(Environment & environment) const -> Body::Cell & {
	if (fromOrganism()) throw std::exception("space is not from environment");
	return environment.accessUnsafe(x, y).food;
}

auto OrganismGrid::Space::cell(std::vector<Organism> & organisms, Environment & environment) const -> Body::Cell & {
	if (fromOrganism()) {
		return cell(organisms);
	} else if (fromEnvironment()) {
		return cell(environment);
	} else {
		throw std::exception("empty space");
	}
}
