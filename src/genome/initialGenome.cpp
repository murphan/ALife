//
// Created by Emmet on 3/5/2023.
//

#include "gene/bodyGene.h"
#include "initialGenome.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	BodyGene::create(Direction::RIGHT, BodyPart::BASIC).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::BASIC).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::createUseAnchor(Direction::UP, BodyPart::UNKNOWN03, 0).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::UP, BodyPart::UNKNOWN03).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT_UP, BodyPart::UNKNOWN03).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);

	return baseGenome;
}
