//
// Created by Emmet on 11/9/2022.
//

#include <iostream>
#include <genome/phenome.h>

#include "genome/genomeCreator.h"
#include "genome/genome.h"
#include "types.h"


auto main () -> int {
	auto creator = GenomeCreator();

	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 2));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 3));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 4));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 5));

	auto genome0 = creator.create();
	std::cout << genome0.toString() << std::endl; // [AA AD AA ADC] AADBAAACAAAAAAACCCAABBAABAD

    auto phenome = Phenome(genome0);
    std::cout << phenome.body.debugToString() << std::endl;

	auto inputString = std::string("ABDACDABBDCABBBCBABCBADBCBDABBBCBDABBABDBADBCBABBDABDDACBCBCDBABC");

	auto genome1 = Genome(inputString);

	std::cout << genome1.toString() << std::endl;
}
