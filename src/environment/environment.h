
#ifndef ALIFE_ENVIRONMENT_H
#define ALIFE_ENVIRONMENT_H

#include "environment/mapCell.h"

#include <vector>

#include "types.h"

/**
 * container for grid squares, environmental factors, food, walls
 */
class Environment {
    private:
        i32 width, height;
        std::vector<MapCell> map;

    public:
        Environment(i32 width, i32 height);

        auto getCell(i32 x, i32 y) -> MapCell &;

        auto tick() -> void;

		auto mapSize() const -> i32;
		auto getWidth() const -> i32;
		auto getHeight() const -> i32;
};

#endif //ALIFE_ENVIRONMENT_H
