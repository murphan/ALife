//
// Created by Emmet on 11/10/2022.
//

#include "phenome.h"

Phenome::Phenome(Genome & genome):
	body(), width(0), height(0_i32), mutationRate(0._f32), actions()
{
	i32 last = genome.get(0);

	for (int i = 0; i < genome.size(); ++i) {

	}
}

auto Phenome::createGenome() -> Genome {

}