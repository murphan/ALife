//
// Created by Rosa on 1/9/2023.
//

#include "bodyGene.h"

auto BodyGene::usesAnchor() const -> bool {
	return usingAnchor != -1;
}

auto BodyGene::setsAnchor() const -> bool {
	return setAnchor != -1;
}

auto BodyGene::headerBase() -> Genome::Base {
	return Genome::A;
}

BodyGene::BodyGene(Direction direction, BodyPart bodyPart, i32 usingAnchor, i32 setAnchor, bool duplicate) :
	direction(direction), bodyPart(bodyPart), usingAnchor(usingAnchor), setAnchor(setAnchor), duplicate(duplicate) {}

BodyGene::BodyGene(GenomeView & view) :
	direction(read5(view, 0) - 2),
	usingAnchor(-1),
	setAnchor(-1),
	duplicate(false),
	bodyPart((BodyPart)(read16(view, 4) + 1))
{
	if (view[2] == Genome::B) {
		usingAnchor = view[3];
	} else if (view[2] == Genome::C) {
		setAnchor = view[3];
	} else if (view[2] == Genome::D) {
		duplicate = true;
	}
}

auto BodyGene::writeBody(Genome & genome) -> void {
	write5(genome, direction.normalized() + 2);

	/* modifier */
	if (usesAnchor()) {
		genome.write(Genome::B);
		genome.write((Genome::Base)usingAnchor);

	} else if (setsAnchor()) {
		genome.write(Genome::C);
		genome.write((Genome::Base)setAnchor);

	} else if (duplicate) {
		genome.write(Genome::D);
		genome.write(Genome::A);

	} else {
		genome.write(Genome::A);
		genome.write(Genome::A);
	}

	write16(genome, bodyPart - 1);
}

/* factories */

auto BodyGene::create(Direction dir, BodyPart bodyPart) -> BodyGene {
	return { dir, bodyPart, -1, -1, false };
}

auto BodyGene::createUseAnchor(Direction dir, BodyPart bodyPart, i32 anchor) -> BodyGene {
	return { dir, bodyPart, anchor, -1, false };
}

auto BodyGene::createSetAnchor(Direction dir, BodyPart bodyPart, i32 anchor) -> BodyGene {
	return { dir, bodyPart, -1, anchor, false };
}

auto BodyGene::createDuplicate(Direction dir, BodyPart bodyPart) -> BodyGene {
	return { dir, bodyPart, -1, -1, true };
}
