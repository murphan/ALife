#include "environment.h"

/** creates an environment of a certain size */
Environment::Environment(i32 width, i32 height) : width(width), height(height), map(mapSize()) {}

auto Environment::getCell(int x, int y) -> MapCell & {
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

auto Environment::tick() -> void {
    for (auto & cell : map) {
        if (cell.getHasFood()){
            cell.getFood().tick();
        }
    }
}
