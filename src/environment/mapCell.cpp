
#include "mapCell.h"

MapCell::MapCell() : factors { 0 }, hasWall(false), food(Body::Cell::makeEmpty()) {}

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
