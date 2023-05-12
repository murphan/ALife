
#ifndef ALIFE_MAPCELL_H
#define ALIFE_MAPCELL_H

#include <vector>
#include <optional>

#include "types.h"

#include "factor.h"
#include "genome/body.h"

/**
 * Map Cells are the grid that cmake up the map
 * they provide locations for the organisims to move through
 * and contain the information that organisims need to make decisions
 */
class MapCell {
private:
    i8 factors[1];
	bool hasWall;

public:
	Body::Cell food;

	MapCell();

    auto getFactor(Factor factor) const -> i8;
	auto setFactor(Factor factor, i8 value) -> void;

	auto setHasWall(bool hasWall) -> void;

	auto getHasWall() const -> bool;
};

#endif //ALIFE_MAPCELL_H
