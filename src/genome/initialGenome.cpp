//
// Created by Emmet on 3/5/2023.
//

#include "gene/bodyGene.h"
#include "initialGenome.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	Gene::write5(baseGenome, BodyPart::MOUTH);
	baseGenome.writeGarbage(7, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::PHOTOSYNTHESIZER).write(baseGenome);
	baseGenome.writeGarbage(13, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::PHOTOSYNTHESIZER).write(baseGenome);
	baseGenome.writeGarbage(13);

	return baseGenome;
}
