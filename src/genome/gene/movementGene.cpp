//
// Created by Emmet on 4/26/2023.
//

#include "movementGene.h"
#include "../geneWriter.h"

const auto MovementGene::LENGTH = 6;

MovementGene::MovementGene(GenomeView & view) :
	change(0)
{
	change += GeneWriter::read5(view, 0) - 2;
	change += GeneWriter::read5(view, 3) - 2;
}

MovementGene::MovementGene(i32 change) : change(change) {}

auto MovementGene::writeBody(Genome & genome) -> void {
	auto half = change / 2;
	auto otherHalf = change - half;

	GeneWriter::write5(genome, half + 2);
	GeneWriter::write5(genome, otherHalf + 2);
}

auto MovementGene::getChange() const -> i32 {
	return change;
}
