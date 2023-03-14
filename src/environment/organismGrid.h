//
// Created by Emmet on 2/10/2023.
//

#ifndef ALIFE_ORGANISMGRID_H
#define ALIFE_ORGANISMGRID_H

#include <vector>
#include "types.h"

#include "organism.h"

class OrganismGridSpace {
public:
	static auto makeEmpty() -> u32;
	static auto makeBodyPart(i32 tempId, BodyPart bodyPart) -> u32;

	static auto getFilled(u32 space) -> bool;
	static auto getTempId(u32 space) -> i32;
	static auto getBodyPart(u32 space) -> BodyPart;
};

class OrganismGrid {
private:
	i32 width, height;
	std::vector<u32> grid;

	auto inBounds(i32 x, i32 y) const -> bool;
	auto indexOf(i32 x, i32 y) const -> i32;

	auto internalSpaceAvailable(
		const Body & body,
		i32 tempId,
		i32 centerX,
		i32 centerY,
		Direction rotation
	) -> bool;

	template <Util::Function<void, BodyPart, u32> OnHit>
	auto internalSpaceAvailableCollide(
		const Body & body,
		i32 tempId,
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
					auto cell = body.access(i, j, rotation);

					if (cell != BodyPart::NONE) {
						auto gridSpace = grid[indexOf(x, y)];
						/* don't encroach onto an existing other organism */
						if (
							OrganismGridSpace::getFilled(gridSpace) &&
							OrganismGridSpace::getTempId(gridSpace) != tempId
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

	auto placeOrganism(const Organism & organism, i32 tempId) -> void;

	template <Util::Function<void, BodyPart, u32> OnHit>
	auto canMoveOrganism(const Organism & organism, i32 tempId, i32 deltaX, i32 deltaY, i32 deltaRotation, OnHit onHit) -> bool {
		return internalSpaceAvailableCollide(
			organism.body(),
			tempId, organism.x + deltaX,
			organism.y + deltaY,
			organism.rotation.rotate(deltaRotation),
			onHit
		);
	}

	auto moveOrganism(Organism & organism, i32 tempId, i32 deltaX, i32 deltaY, i32 deltaRotation) -> void;

	auto isSpaceAvailable(const Body & body, i32 x, i32 y, Direction rotation) -> bool;

	auto organismInSpace(i32 x, i32 y) const -> bool;

    auto getWidth() const -> i32;

    auto getHeight() const -> i32;
};

#endif //ALIFE_ORGANISMGRID_H
