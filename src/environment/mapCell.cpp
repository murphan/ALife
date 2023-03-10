
#include "mapCell.h"

MapCell::MapCell() : factors { 0, 0, 0 }, hasWall(false), food { std::nullopt } {}

MapCell::MapCell(i8 factor0, i8 factor1, i8 factor2) :
	factors { factor0, factor1, factor2 },
	hasWall { false },
	food { std::nullopt } {}

MapCell::MapCell(i8 factors[3]) :
	factors { factors[0], factors[1], factors[2] },
	hasWall { false },
	food { std::nullopt } {}

auto MapCell::getFactor(Factor factor) const -> i8 {
	return factors[factor];
}

auto MapCell::setFood(Food newFood) -> void {
	this->food = newFood;
}

auto MapCell::getHasWall() const -> bool {
	return hasWall;
}

auto MapCell::getHasFood() const -> bool {
	return food.has_value();
}

auto MapCell::getFood() -> Food & {
	return food.value();
}

/**
 * Removes food from mapCell
 */
auto MapCell::removeFood() -> void {
    food.reset();
}

auto MapCell::setFactor(Factor factor, i8 value) -> void {
	factors[factor] = value;
}
