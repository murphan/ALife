//
// Created by Emmet on 2/10/2023.
//

#ifndef ALIFE_ORGANISMGRID_H
#define ALIFE_ORGANISMGRID_H

#include <vector>
#include "util/types.h"

#include "organism/organism.h"
#include "environment.h"

class OrganismGrid {
public:
	class Space {
	private:
		constexpr static u32 FOOD_VALUE = ~0;

		i32 x, y;
		u32 value;

		explicit Space(i32 x, i32 y, u32 value);

	public:
		static auto makeEmpty() -> Space;
		static auto makeCell(Body::Cell reference, i32 index) -> Space;
		static auto makeFood(i32 x, i32 y) -> Space;

		[[nodiscard]] auto isFilled() const -> bool;
		[[nodiscard]] auto fromOrganism() const -> bool;
		[[nodiscard]] auto fromEnvironment() const -> bool;

		[[nodiscard]] auto index() const -> i32;

		[[nodiscard]] auto cell(std::vector<Organism> & organisms) const -> Body::Cell &;
		[[nodiscard]] auto cell(Environment & environment) const -> Body::Cell &;
		[[nodiscard]] auto cell(std::vector<Organism> & organisms, Environment & environment) const -> Body::Cell &;
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
