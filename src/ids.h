//
// Created by Emmet on 4/21/2023.
//

#ifndef ALIFE_IDS_H
#define ALIFE_IDS_H

#include <random>

#include "types.h"

class Ids {
public:
	constexpr static u32 NUM_IDS = 65536;

private:
	i32 numFilled;
	std::default_random_engine & random;

	bool array[NUM_IDS];

public:
	explicit Ids(std::default_random_engine & random);

	auto newId() -> u32;

	auto removeId(u32 id) -> void;
};

#endif //ALIFE_IDS_H
