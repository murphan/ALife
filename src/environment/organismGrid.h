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
		u32 value;
		Body::Cell internalCell;

	public:
		explicit Space(u32 value, Body::Cell cell);

		static auto makeEmpty() -> Space;
		static auto makeCell(i32 index, Body::Cell cell) -> Space;

		auto filled() const -> bool;
		auto index() const -> i32;
		auto cell() const -> Body::Cell;
	};

private:
	Space blankSpace;

	i32 width, height;
	std::vector<Space> grid;

	auto indexOf(i32 x, i32 y) const -> i32;

	auto internalSpaceAvailable(
		const Body & body,
		i32 index,
		i32 centerX,
		i32 centerY,
		Direction rotation
	) -> bool;

public:
	OrganismGrid(i32 width, i32 height);

	auto clear() -> void;

	auto inBounds(i32 x, i32 y) const -> bool;

	auto placeOrganism(const Organism & organism, i32 index) -> void;

	auto canMoveOrganism(const Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool;

	auto moveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void;

	auto isSpaceAvailable(const Body & body, i32 x, i32 y, Direction rotation) -> bool;

	auto access(i32 x, i32 y) -> Space &;
	auto accessSafe(i32 x, i32 y) const -> const Space &;

    auto getWidth() const -> i32;

    auto getHeight() const -> i32;
};

#endif //ALIFE_ORGANISMGRID_H
