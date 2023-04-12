#ifndef ALIFE_FOOD_H
#define ALIFE_FOOD_H

#include "types.h"

/**
 * Food is consumed by organisims to get energy. It spawns
 * in mapCells when the game is started and then is generated
 * by other organisims when they die.
 */
class Food {
public:
	enum Type {
		FOOD0 = 0,
		FOOD1 = 1,
		FOOD2 = 2,
		FOOD3 = 3,
	};

private:
	Type type;
    u8 energy;
public:
	u32 age;

	Food(Type type, u8 energy);
    Food(Type type, u8 energy, u32 age);

    auto getType() const -> Type;
	auto getEnergy() const -> u8;

	auto addEnergy(i32 energy) -> void;
};

#endif //ALIFE_FOOD_H
