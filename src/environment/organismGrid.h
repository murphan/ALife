//
// Created by Emmet on 2/10/2023.
//

#ifndef ALIFE_ORGANISMGRID_H
#define ALIFE_ORGANISMGRID_H

#include <vector>
#include "types.h"

#include "organism.h"


class OrganismGrid {
public:
	class Space {
	private:
		/** points to the direct location of the cell in the organism */
		i8 x, y;
		u32 value;
		/** cell is a copy of the original from the organism */
		Body::Cell internalCell;

		explicit Space(i8 x, i8 y, u32 value, Body::Cell & cell);

	public:
		static auto makeEmpty() -> Space;
		static auto makeCell(i8 x, i8 y, i32 index, Body::Cell & cell) -> Space;

		[[nodiscard]] auto filled() const -> bool;
		[[nodiscard]] auto index() const -> i32;
		[[nodiscard]] auto cell() const -> Body::Cell;
		[[nodiscard]] auto getX() const -> i32;
		[[nodiscard]] auto getY() const -> i32;

		auto getOriginalCell(Organism & organism) -> Body::Cell &;
	};

private:
	Space blankSpace;

	i32 width, height;
	std::vector<Space> grid;

	auto indexOf(i32 x, i32 y) const -> i32;

	auto internalSpaceAvailable(
		Body & body,
		i32 index,
		i32 centerX,
		i32 centerY,
		Direction rotation
	) -> bool;

public:
	OrganismGrid(i32 width, i32 height);

	auto clear() -> void;

	auto inBounds(i32 x, i32 y) const -> bool;

	auto eraseOrganism(Organism & organism, i32 index) -> void;

	auto placeOrganism(Organism & organism, i32 index) -> void;

	auto canMoveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool;

	auto moveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void;

	auto isSpaceAvailable(Body & body, i32 x, i32 y, Direction rotation) -> bool;

	auto accessUnsafe(i32 x, i32 y) -> Space &;
	auto access(i32 x, i32 y) -> Space &;

    auto getWidth() const -> i32;

    auto getHeight() const -> i32;
};

#endif //ALIFE_ORGANISMGRID_H
