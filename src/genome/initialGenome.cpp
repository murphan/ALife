//
// Created by Emmet on 3/5/2023.
//

#include "gene/bodyGene.h"
#include "initialGenome.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	Gene::write7(baseGenome, BodyPart::MOUTH - 1);
	Gene::write8(baseGenome, 0);
	baseGenome.writeGarbage(7, Genome::A);
	BodyGene::create(Direction::RIGHT_UP, BodyPart::PHOTOSYNTHESIZER, 0).write(baseGenome);
	baseGenome.writeGarbage(13, Genome::A);
	BodyGene::create(Direction::RIGHT_DOWN, BodyPart::PHOTOSYNTHESIZER, 0).write(baseGenome);
	baseGenome.writeGarbage(13);

	return baseGenome;
}
