//
// Created by Emmet on 11/10/2022.
//

#include "eyeGene.h"

const auto EyeGene::LENGTH = 6;

auto EyeGene::headerBase() -> Genome::Base {
	return Genome::B;
}

EyeGene::EyeGene(GenomeView & view) :
	seeing((BodyPart)read7(view, 0)),
	actionType((ActionType)read2(view, 3)) {}

EyeGene::EyeGene(BodyPart bodyPart, ActionType actionType) :
		seeing(bodyPart),
		actionType(actionType) {}

auto EyeGene::writeBody(Genome & genome) -> void {
	write7(genome, seeing);
	write2(genome, actionType);
}
