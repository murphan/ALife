
module;

#include <vector>

export module Environment;

import Types;
import MapCell;
import Util;

/**
 * container for grid squares, environmental factors, food, walls
 */
export class Environment {
    private:
        i32 width, height;
        std::vector<MapCell> map;

		MapCell emptyCell;

    public:
        Environment(i32 width, i32 height) : emptyCell(), width(width), height(height), map(mapSize()) {
	        emptyCell.setHasWall(true);
        };

		[[nodiscard]] auto accessUnsafe(i32 x, i32 y) const -> const MapCell &  {
			return map[y * width + x];
		}

		auto accessUnsafe(i32 x, i32 y) -> MapCell &  {
			return Util::asMutable(Util::asConst(this)->accessUnsafe(x, y));
		}

		auto access(i32 x, i32 y) -> MapCell & {
			if (x < 0 || x >= width || y < 0 || y >= height) return emptyCell;
			return map[y * width + x];
		}

		[[nodiscard]] auto mapSize() const -> i32 {
			return width * height;
		}

		[[nodiscard]] auto getWidth() const -> i32 {
			return width;
		}

		[[nodiscard]] auto getHeight() const -> i32 {
			return height;
		}
};
