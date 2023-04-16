//
// Created by Emmet on 3/5/2023.
//

#include "gene/bodyGene.h"
#include "initialGenome.h"

auto InitialGenome::create() -> Genome {
	auto baseGenome = Genome();

	Gene::write5(baseGenome, BodyPart::MOUTH);
	Gene::write4(baseGenome, Food::FOOD0);
	baseGenome.writeGarbage(7, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::PHOTOSYNTHESIZER, Food::FOOD0).write(baseGenome);
	baseGenome.writeGarbage(13, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::PHOTOSYNTHESIZER, Food::FOOD0).write(baseGenome);
	baseGenome.writeGarbage(13);

	return baseGenome;
}
