//
// Created by Rosa on 1/12/2023.
//

#include "mutationRateGene.h"

MutationRateGene::MutationRateGene(GenomeView & view) :
	types(),
	changes()
{
	for (auto i = 0; i < 4; ++i) {
		types[i] = (MutationType) view[i * 2];
		changes[i] = view[i * 2 + 1] / 2;
	}
}

MutationRateGene::MutationRateGene(std::initializer_list<Entry> && list) :
	types(),
	changes()
{
	for (auto i = 0; i < 4; ++i) {
		auto entry = list.begin() + i;
		if (entry < list.end()) {
			types[i] = entry->type;
			changes[i] = entry->change;
		} else {
			types[i] = MutationType::NONE;
			changes[i] = 0;
		}
	}
}

auto MutationRateGene::headerBase() -> Genome::Base {
	return Genome::D;
}

auto MutationRateGene::writeBody(Genome & genome) -> void {
	for (auto i = 0; i < 4; ++i) {
		genome.write((Genome::Base) types[i]);
		genome.write(changes[i] == 0 ? Genome::A : Genome::C);
	}
}

auto MutationRateGene::getResults() -> Results {
	i32 mutations[3] = { 0, 0, 0 };

	for (auto i = 0; i < 4; ++i) {
		if (types[i] != MutationType::NONE) {
			mutations[types[i]] += changes[i] * 2 - 1;
		}
	}

	return { mutations[0], mutations[1], mutations[2] };
}
