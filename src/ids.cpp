//
// Created by Emmet on 4/21/2023.
//

#include <exception>
#include "ids.h"
#include "util.h"

Ids::Ids() : numFilled(0), array(NUM_IDS, false) {}

auto indexRange = std::uniform_int_distribution<i32>(0, Ids::NUM_IDS - 1);
auto directionRange = std::uniform_int_distribution<i32>(0, 1);

auto Ids::newId(std::default_random_engine & random) -> u32 {
	if (numFilled == NUM_IDS) throw std::exception("IDS IS FULL");

	auto startIndex = indexRange(random);
	auto direction = directionRange(random);

	for (auto i = 0; i < NUM_IDS; ++i) {
		auto index = Util::positiveMod(startIndex + (direction ? i : -i), NUM_IDS);

		if (!array[index]) {
			array[index] = true;
			++numFilled;
			return index;
		}
	}

	throw std::exception("IMPOSSIBLE TO REACH");
}

auto Ids::removeId(u32 id) -> void {
	if (array[id]) {
		array[id] = false;
		--numFilled;
	}
}
