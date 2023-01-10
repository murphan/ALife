//
// Created by Emmet on 11/10/2022.
//

#include "genomeCreator.h"





/*
 * which pairs to encode numbers as chosen arbitrarily from
 * the possible range of pairs that can produce that number
 */



GenomeCreator::GenomeCreator(i32 filler): filler(filler), genome() {}


auto GenomeCreator::addBodyGene(BodyInstruction && instruction) -> void {
	/* body gene header */
	genome.write(Genome::A);
	genome.write(Genome::A);

	write5(genome, instruction.direction.normalized() + 2);

	/* modifier */
	if (instruction.usesAnchor()) {
		genome.write(Genome::B);
		genome.write((Genome::Base)instruction.anchor);

	} else if (instruction.setsAnchor()) {
		genome.write(Genome::C);
		genome.write((Genome::Base)instruction.setAnchor);

	} else if (instruction.duplicate) {
		genome.write(Genome::D);
		genome.write(Genome::A);

	} else {
		genome.write(Genome::A);
		genome.write(Genome::A);
	}

	write16(genome, instruction.type - 1);
}

auto GenomeCreator::addEyeGene(EyeGene && reaction) -> void {
	genome.write(Genome::B);
	genome.write(Genome::B);


}

auto GenomeCreator::addEnvironmentGene(EnvironmentGene && reaction) -> void {
	genome.write(Genome::B);
	genome.write(Genome::B);


}

auto GenomeCreator::addFoodGene(FoodType type, FoodStats::Type digestion0, FoodStats::Type digestion1) -> void {
	genome.write(Genome::C);
	genome.write(Genome::C);

	write4(genome, type);
	write2(genome, digestion0);
	write2(genome, digestion1);
}

auto GenomeCreator::addMutationRateGene(Phenome::MutationType type0, i32 strength0, Phenome::MutationType type1, i32 strength1) -> void {
	genome.write(Genome::D);
	genome.write(Genome::D);

	write4(genome, type0);
	write2(genome, strength0 == -1 ? 0 : 1);
	write4(genome, type1);
	write2(genome, strength1 == -1 ? 0 : 1);
}

auto GenomeCreator::get() const -> const Genome & {
	return genome;
}
