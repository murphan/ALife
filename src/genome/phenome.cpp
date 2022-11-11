//
// Created by Emmet on 11/10/2022.
//

#include "phenome.h"

Phenome::Phenome(Genome & genome):
	body(), mutationRate(0._f32), actions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder(2, 1);

	auto last = genome.get(0);

	for (int i = 0; i < genome.size(); ++i) {
		auto current = genome.get(i);
		//TODO read genome
	}

	body = bodyBuilder.toBody();
}
