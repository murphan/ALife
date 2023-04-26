//
// Created by Rosa on 1/9/2023.
//

#include "bodyGene.h"
#include "genome/geneWriter.h"

const auto BodyGene::LENGTH = 15;

auto BodyGene::usesAnchor() const -> bool {
	return usingAnchor != -1;
}

auto BodyGene::setsAnchor() const -> bool {
	return setAnchor != -1;
}

BodyGene::BodyGene(Direction direction, BodyPart bodyPart, i32 usingAnchor, i32 setAnchor, i32 data) :
	direction(direction), bodyPart(bodyPart), usingAnchor(usingAnchor), setAnchor(setAnchor), data(data) {}

BodyGene::BodyGene(GenomeView & view) :
	bodyPart((BodyPart)(GeneWriter::read7(view, 0) + 1)),
	direction(GeneWriter::read8(view, 3)),
	usingAnchor(-1),
	setAnchor(-1),
	data(GeneWriter::read8(view, 12))
{
	auto special = GeneWriter::read3(view, 6);

	if (special == 1) {
		usingAnchor = GeneWriter::read4(view, 9);
	} else if (special == 2) {
		setAnchor = GeneWriter::read4(view, 9);
	}
}

auto BodyGene::writeBody(Genome & genome) -> void {
	GeneWriter::write7(genome, bodyPart - 1);
	GeneWriter::write8(genome, direction.value());

	if (usesAnchor()) {
		GeneWriter::write3(genome, 1);
		GeneWriter::write4(genome, usingAnchor);
	} else if (setsAnchor()) {
		GeneWriter::write3(genome, 2);
		GeneWriter::write4(genome, setAnchor);
	} else {
		GeneWriter::write3(genome, 0);
		GeneWriter::write4(genome, 0);
	}

	GeneWriter::write8(genome, data);
}

/* factories */

auto BodyGene::create(Direction dir, BodyPart bodyPart, i32 data) -> BodyGene {
	return { dir, bodyPart, -1, -1, data };
}

auto BodyGene::createUseAnchor(Direction dir, BodyPart bodyPart, i32 anchor, i32 data) -> BodyGene {
	return { dir, bodyPart, anchor, -1, data };
}

auto BodyGene::createSetAnchor(Direction dir, BodyPart bodyPart, i32 anchor, i32 data) -> BodyGene {
	return { dir, bodyPart, -1, anchor, data };
}

auto BodyGene::createDuplicate(Direction dir, BodyPart bodyPart, i32 data) -> BodyGene {
	return { dir, bodyPart, -1, -1, data };
}
