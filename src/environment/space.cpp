//
// Created by Emmet on 5/20/2023.
//

module;

#include <vector>
#include <exception>

export module Space;

import Types;
import Cell;
import Organism;
import Environment;

export class Space {
private:
	constexpr static u32 FOOD_VALUE = ~0;

	i32 x, y;
	u32 value;

	explicit Space(i32 x, i32 y, u32 value) :
		x(x), y(y), value(value) {};

public:
	static auto makeEmpty() -> Space {
		return Space(0, 0, 0);
	}

	static auto makeCell(Cell reference, i32 index) -> Space {
		return Space(reference.x(), reference.y(), index + 1);
	}

	static auto makeFood(i32 x, i32 y) -> Space {
		return Space(x, y, FOOD_VALUE);
	}

	[[nodiscard]] auto isFilled() const -> bool {
		return value != 0;
	}

	[[nodiscard]] auto fromOrganism() const -> bool  {
		return isFilled() && !fromEnvironment();
	}

	[[nodiscard]] auto fromEnvironment() const -> bool  {
		return value == FOOD_VALUE;
	}

	[[nodiscard]] auto index() const -> i32  {
		return (i32)(value - 1);
	}

	[[nodiscard]] auto cell(std::vector<Organism> & organisms) const -> Cell & {
		if (fromEnvironment()) throw std::exception("space is not from organism");
		return organisms[index()].body().directAccess(x, y);
	}

	[[nodiscard]] auto cell(Environment & environment) const -> Cell & {
		if (fromOrganism()) throw std::exception("space is not from environment");
		return environment.accessUnsafe(x, y).food;
	}

	[[nodiscard]] auto cell(std::vector<Organism> & organisms, Environment & environment) const -> Cell & {
		if (fromOrganism()) {
			return cell(organisms);
		} else if (fromEnvironment()) {
			return cell(environment);
		} else {
			throw std::exception("empty space");
		}
	}
};
