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

	public:
		Space(u32 value);

		auto makeEmpty() -> void;
		auto makeBodyPart(i32 tempId, BodyPart bodyPart) -> void;

		auto getFilled() const -> bool;
		auto getIndex() const -> i32;
		auto getBodyPart() const -> BodyPart;
	};

private:
	Space blankSpace;

	i32 width, height;
	std::vector<Space> grid;

	auto inBounds(i32 x, i32 y) const -> bool;
	auto indexOf(i32 x, i32 y) const -> i32;

	auto internalSpaceAvailable(
		const Body & body,
		i32 index,
		i32 centerX,
		i32 centerY,
		Direction rotation
	) -> bool;

	template <Util::Function<void, BodyPart, u32> OnHit>
	auto internalSpaceAvailableCollide(
		const Body & body,
		i32 index,
		i32 centerX,
		i32 centerY,
		Direction rotation,
		OnHit onHit
	) -> bool {
		auto spaceAvailable = true;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = centerY + j;
				auto x = centerX + i;

				if (!inBounds(x, y)) {
					spaceAvailable = false;
				} else {
					auto cell = body.access(i, j, rotation).bodyPart();

					if (cell != BodyPart::NONE) {
						auto gridSpace = grid[indexOf(x, y)];
						/* don't encroach onto an existing other organism */
						if (
							gridSpace.getFilled() &&
								gridSpace.getIndex() != index
						) {
							spaceAvailable = false;
							onHit(cell, gridSpace);
						}
					}
				}
			}
		}

		return spaceAvailable;
	};

public:
	OrganismGrid(i32 width, i32 height);

	auto clear() -> void;

	auto placeOrganism(const Organism & organism, i32 index) -> void;

	template <Util::Function<void, BodyPart, Space> OnHit>
	auto canMoveOrganism(const Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation, OnHit onHit) -> bool {
		return internalSpaceAvailableCollide(
			organism.body(),
			index, organism.x + deltaX,
			organism.y + deltaY,
			organism.rotation.rotate(deltaRotation),
			onHit
		);
	}

	auto moveOrganism(Organism & organism, i32 index, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void;

	auto isSpaceAvailable(const Body & body, i32 x, i32 y, Direction rotation) -> bool;

	auto access(i32 x, i32 y) -> Space &;
	auto accessSafe(i32 x, i32 y) const -> const Space &;

    auto getWidth() const -> i32;

    auto getHeight() const -> i32;
};

#endif //ALIFE_ORGANISMGRID_H
