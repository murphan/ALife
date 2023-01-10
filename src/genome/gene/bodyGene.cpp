//
// Created by Rosa on 1/9/2023.
//

#include "bodyGene.h"

auto BodyGene::usesAnchor() const -> bool {
	return anchor != -1;
}

auto BodyGene::setsAnchor() const -> bool {
	return setAnchor != -1;
}

auto BodyGene::headerBase() -> Genome::Base {
	return Genome::A;
}

BodyGene::BodyGene(GenomeView & view) {

}

auto BodyGene::writeBody(Genome & genome) -> void {
	write5(genome, direction.normalized() + 2);

	/* modifier */
	if (usesAnchor()) {
		genome.write(Genome::B);
		genome.write((Genome::Base)anchor);

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

	write16(genome, instruction.type - 1);
}