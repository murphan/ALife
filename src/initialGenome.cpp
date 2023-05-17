//
// Created by Emmet on 3/5/2023.
//

#include "genome/gene/bodyGene.h"
#include "initialGenome.h"
#include "genome/geneWriter.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	GeneWriter::write7(baseGenome, BodyPart::MOUTH - 1);
	baseGenome.writeGarbage(21);

	baseGenome.writeHeader();
	GeneWriter::write5(baseGenome, Gene::BODY);
	BodyGene::create(Direction::RIGHT_UP, BodyPart::PHOTOSYNTHESIZER).writeBody(baseGenome);

	baseGenome.writeGarbage(21);

	baseGenome.writeHeader();
	GeneWriter::write5(baseGenome, Gene::BODY);
	BodyGene::create(Direction::RIGHT_DOWN, BodyPart::PHOTOSYNTHESIZER).writeBody(baseGenome);

	baseGenome.writeGarbage(21);

	return baseGenome;
}
