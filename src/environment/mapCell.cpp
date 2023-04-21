
#include "mapCell.h"

MapCell::MapCell() : factors { 0, 0, 0 }, hasWall(false) {}

MapCell::MapCell(i8 factor0, i8 factor1, i8 factor2) :
	factors { factor0, factor1, factor2 },
	hasWall { false } {}

MapCell::MapCell(i8 factors[3]) :
	factors { factors[0], factors[1], factors[2] },
	hasWall { false } {}

auto MapCell::getFactor(Factor factor) const -> i8 {
	return factors[factor];
}

auto MapCell::getHasWall() const -> bool {
	return hasWall;
}

auto MapCell::setFactor(Factor factor, i8 value) -> void {
	factors[factor] = value;
}

auto MapCell::setHasWall(bool hasWall) -> void {
	this->hasWall = hasWall;
}
