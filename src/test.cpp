//
// Created by Emmet on 11/9/2022.
//

#include <iostream>
#include <src/phenome/phenome.h>

#include "genome/genomeCreator.h"
#include "genome/genome.h"
#include "types.h"
#include "util.h"
#include <tuple>

auto main () -> int {
    auto g0 = Genome("AAACAAADAAABAAACAAAD");
    auto g1 = Genome("AAADAAABAAABAAACAAAC");

	for (auto i = 0; i < 20; ++i) {
		auto recombined = Genome::mutateCombine(g0, g1, 0.3_f32, 0.01_f32, 0.01_f32, 0.01_f32);
		std::cout << "Combined: " << (recombined.has_value() ? recombined->toString() : "Not compatible") << std::endl;
	}
	return 0;

    //auto [ r0, r1 ] = std::tuple<Genome &, Genome &> { g0, g1 };
    //auto grid = Genome::editDistance(g0, g1);
    //Util::printGrid(grid, g0.size() + 1);

    /* mutate genome test */
    auto ogGenome = Genome(std::string("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
    auto mutatedGenome = ogGenome.mutateCopy(0.01_f32, 0.01_f32, 0.01_f32);

    std::cout << ogGenome.toString() << std::endl;
    std::cout << mutatedGenome.toString() << '\n' << std::endl;

    /* create body test */
	auto creator = GenomeCreator();

	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 2));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 3));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 4));
	creator.addBodyInstruction(BodyInstruction::create(Direction(Direction::RIGHT), 5));

	auto genome0 = creator.create();
	std::cout << genome0.toString() << std::endl; // [AA AD AA ADC] AADBAAACAAAAAAACCCAABBAABAD

    auto phenome = Phenome(genome0);
    std::cout << phenome.body.debugToString() << std::endl;

    /* string decode test */
	auto genome1 = Genome("ABDACDABBDCABBBCBABCBADBCBDABBBCBDABBABDBADBCBABBDABDDACBCBCDBABC");

	std::cout << genome1.toString() << std::endl;
}
