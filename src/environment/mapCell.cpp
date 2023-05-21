
export module MapCell;

import Types;
import Cell;
import Factor;

export class MapCell {
private:
    i8 factors[1];
	bool wall;

public:
	Cell food;

	constexpr MapCell() : factors { 0 }, wall(false), food(Cell::makeEmpty()) {};

    [[nodiscard]] auto getFactor(Factor factor) const -> i8 {
	    return factors[factor];
    }

	auto setFactor(Factor factor, i8 value) -> void {
		factors[factor] = value;
	}

	auto setHasWall(bool hasWall) -> void {
		this->wall = hasWall;
	}

	[[nodiscard]] auto getHasWall() const -> bool {
		return wall;
	}
};
