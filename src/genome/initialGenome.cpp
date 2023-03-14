//
// Created by Emmet on 3/5/2023.
//

#include "gene/bodyGene.h"
#include "initialGenome.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	BodyGene::create(Direction::RIGHT, BodyPart::WEAPON).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::PHOTOSYNTHESIZER).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::createUseAnchor(Direction::UP, BodyPart::MOVER, 0).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::UP, BodyPart::ARMOR).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT_UP, BodyPart::WEAPON).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);

	return baseGenome;
}
