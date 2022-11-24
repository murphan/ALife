//
// Created by Emmet on 11/9/2022.
//

#include <iostream>
#include <genome/phenome.h>

#include "genome/genomeCreator.h"
#include "genome/genome.h"
#include "types.h"
#include "util.h"
#include <iomanip>

auto main () -> int {
    auto g0 = Genome("DABDABBBCABDBCBBADBCABDABA");
    auto g1 = Genome("ABCBADBBCDBCBBDAACABAD");
    auto grid = g0.editDistance(g1);

    for (auto y = 0; y <= g1.size(); ++y) {
        for (auto x = 0; x <= g0.size(); ++x) {
            std::cout << std::setfill('0') << std::setw(2) << grid[y * (g0.size() + 1) + x] << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

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
