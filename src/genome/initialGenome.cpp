//
// Created by Emmet on 3/5/2023.
//

#include "gene/bodyGene.h"
#include "initialGenome.h"
#include "geneWriter.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	GeneWriter::write7(baseGenome, BodyPart::MOUTH - 1);
	GeneWriter::write8(baseGenome, 0);
	baseGenome.writeGarbage(21);

	baseGenome.writeHeader();
	GeneWriter::write5(baseGenome, Gene::BODY);
	BodyGene::create(Direction::RIGHT_UP, BodyPart::PHOTOSYNTHESIZER, 0).writeBody(baseGenome);

	baseGenome.writeGarbage(21);

	baseGenome.writeHeader();
	GeneWriter::write5(baseGenome, Gene::BODY);
	BodyGene::create(Direction::RIGHT_DOWN, BodyPart::PHOTOSYNTHESIZER, 0).writeBody(baseGenome);

	baseGenome.writeGarbage(21);

	return baseGenome;
}
