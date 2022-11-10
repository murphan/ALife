//
// Created by Emmet on 11/9/2022.
//

#include "src/genome/genome.h"
#include "types.h"
#include <iostream>

auto main () -> int {
	auto inputString = std::string("ABDACDABBDCABBBCBABCBADBCBDABBBCBDABBABDBADBCBABBDABDDACBCBCDBABC");

	auto genome = Genome(inputString);

	std::cout << genome.toString() << std::endl;
}
