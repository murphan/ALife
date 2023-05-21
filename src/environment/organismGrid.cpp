//
// Created by Emmet on 2/10/2023.
//

module;

#include <vector>
#include <algorithm>

export module OrganismGrid;

import Types;
import Space;
import Organism;
import Body;
import Direction;
import Cell;

export class OrganismGrid {
private:
	Space blankSpace;

	i32 width, height;
	std::vector<Space> grid;

	[[nodiscard]] auto indexOf(i32 x, i32 y) const -> i32 {
		return y * width + x;
	}

	auto internalSpaceAvailable(
		Body & body,
		i32 index,
		i32 centerX,
		i32 centerY,
		Direction rotation
	) -> bool {
		return std::all_of(body.getCells().begin(), body.getCells().end(), [&](const Cell & cell) {
			auto [x, y] = Body::absoluteXY(cell, centerX, centerY, rotation);

			if (!inBounds(x, y)) return false;

			auto && gridSpace = grid[indexOf(x, y)];
			if (gridSpace.isFilled() && gridSpace.index() != index) return false;

			return true;
		});
	}

public:
	OrganismGrid(i32 width, i32 height) :
		blankSpace(Space::makeEmpty()),
		width(width),
		height(height),
		grid(width * height, Space::makeEmpty()) {}

	[[nodiscard]] auto inBounds(i32 x, i32 y) const -> bool {
		return x >= 0 && x < width && y >= 0 && y < height;
	}

	auto clear() -> void {
		for (auto && space : grid) {
			space = Space::makeEmpty();
		}
	}

	auto canMoveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool {
		return internalSpaceAvailable(
			organism.body(),
			index, organism.x + deltaX,
			organism.y + deltaY,
			organism.rotation.rotate(deltaRotation)
		);
	}

	auto placeOrganism(Organism & organism, i32 index) -> void {
		for (auto && cell : organism.body().getCells()) {
			auto [x, y] = organism.absoluteXY(cell);
			if (!cell.broken()) grid[indexOf(x, y)] = Space::makeCell(cell, index);
		}
	}

	auto eraseOrganism(Organism & organism, i32 index) -> void {
		for (auto && cell : organism.body().getCells()) {
			auto [x, y] = organism.absoluteXY(cell);
			if (!cell.broken()) grid[indexOf(x, y)] = Space::makeEmpty();
		}
	}

	/**
	 * modifies the passed organism's position and rotation
	 */
	auto moveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void {
		eraseOrganism(organism, index);

		organism.x += deltaX;
		organism.y += deltaY;
		organism.rotation = organism.rotation.rotate(deltaRotation);

		placeOrganism(organism, index);
	}

	auto isSpaceAvailable(Body & body, i32 x, i32 y, Direction rotation) -> bool {
		return internalSpaceAvailable(body, -1, x, y, rotation);
	}

	[[nodiscard]] auto getWidth() const -> i32 {
		return width;
	}

	[[nodiscard]] auto getHeight() const -> i32 {
		return height;
	}

	auto accessUnsafe(i32 x, i32 y) -> Space & {
		return grid[indexOf(x, y)];
	}

	auto access(i32 x, i32 y) -> Space & {
		if (x < 0 || x >= width || y < 0 || y >= height) return blankSpace;
		return grid[indexOf(x, y)];
	}
};
