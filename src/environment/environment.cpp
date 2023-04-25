#include "environment.h"

/** creates an environment of a certain size */
Environment::Environment(i32 width, i32 height) : emptyCell(), width(width), height(height), map(mapSize()) {
	emptyCell.setHasWall(true);
}

auto Environment::accessUnsafe(int x, int y) -> MapCell & {
    return map[y * width + x];
}

auto Environment::access(i32 x, i32 y) -> MapCell & {
	if (x < 0 || x >= width || y < 0 || y >= height) return emptyCell;
	return map[y * width + x];
}

auto Environment::mapSize() const -> i32 {
	return width * height;
}

auto Environment::getWidth() const -> i32 {
	return width;
}

auto Environment::getHeight() const -> i32 {
	return height;
}
