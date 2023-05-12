//
// Created by Emmet on 11/10/2022.
//

#include "eyeGene.h"
#include "../geneWriter.h"

const auto EyeGene::LENGTH = 6;

EyeGene::EyeGene(GenomeView & view) :
	seeing((BodyPart)(GeneWriter::read7(view, 0) + 1)),
	actionType((ActionType)GeneWriter::read2(view, 3)) {}

EyeGene::EyeGene(BodyPart bodyPart, ActionType actionType) :
		seeing(bodyPart),
		actionType(actionType) {}

auto EyeGene::writeBody(Genome & genome) -> void {
	GeneWriter::write7(genome, seeing - 1);
	GeneWriter::write2(genome, actionType);
}
