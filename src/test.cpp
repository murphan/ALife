//
// Created by Emmet on 11/9/2022.
//

#include <iostream>

#include "genome/genomeCreator.h"
#include "genome/genome.h"
#include "types.h"


auto main () -> int {
	auto creator = GenomeCreator();

	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 4));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT_DOWN), 8));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::UP), 9));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT_UP), 12));

	auto genome0 = creator.create();
	std::cout << genome0.toString() << std::endl; // AAADAAADCAADBAAACAAAAAAACCCAABBAABAD

	auto inputString = std::string("ABDACDABBDCABBBCBABCBADBCBDABBBCBDABBABDBADBCBABBDABDDACBCBCDBABC");

	auto genome1 = Genome(inputString);

	std::cout << genome1.toString() << std::endl;
}
