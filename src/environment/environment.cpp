#include "environment.h"

/** creates an environment of a certain size */
Environment::Environment(i32 width, i32 height) : width(width), height(height), map() {
    map.reserve(mapSize());

    for (int i = 0; i < mapSize(); ++i)
        map.emplace_back(5, 5, 0, 0);
}

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
