//
// Created by Rosa on 1/12/2023.
//

#include "mutationRateGene.h"

const auto MutationRateGene::LENGTH = 6;

MutationRateGene::MutationRateGene(GenomeView & view) :
	change(0)
{
	change += read5(view, 0) - 2;
	change += read5(view, 3) - 2;
}

MutationRateGene::MutationRateGene(i32 change) : change(change) {}

auto MutationRateGene::headerBase() -> Genome::Base {
	return Genome::D;
}

auto MutationRateGene::writeBody(Genome & genome) -> void {
	auto half = change / 2;
	auto otherHalf = change - half;

	write5(genome, half + 2);
	write5(genome, otherHalf + 2);
}

auto MutationRateGene::getChange() -> i32 {
	return change;
}
