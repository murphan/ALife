//
// Created by Emmet on 11/10/2022.
//

#include "phenome.h"

Phenome::Phenome(Genome & genome):
	body(2, 1), mutationRate(0._f32), actions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder();
	auto last = genome.get(0);

	for (int i = 0; i < genome.size(); ++i) {
		auto current = genome.get(i);
		//TODO read genome
	}


}
