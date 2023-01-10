//
// Created by Emmet on 11/10/2022.
//

#include "genomeCreator.h"



auto BodyInstruction::create(Direction dir, i32 type) -> BodyInstruction {
	return {
		.direction = dir,
		.type = type,
		.anchor = -1,
		.setAnchor = -1,
		.duplicate = false
	};
}

auto BodyInstruction::createUseAnchor(Direction dir, i32 type, i32 anchor) -> BodyInstruction {
	return {
		.direction = dir,
		.type = type,
		.anchor = anchor,
		.setAnchor = -1,
		.duplicate = false
	};
}

auto BodyInstruction::createSetAnchor(Direction dir, i32 type, i32 anchor) -> BodyInstruction {
	return {
		.direction = dir,
		.type = type,
		.anchor = -1,
		.setAnchor = anchor,
		.duplicate = false
	};
}

auto BodyInstruction::createDuplicate(Direction dir, i32 type) -> BodyInstruction {
	return {
		.direction = dir,
		.type = type,
		.anchor = -1,
		.setAnchor = -1,
		.duplicate = true
	};
}

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

auto GenomeCreator::addEyeGene(EyeReaction && reaction) -> void {
	genome.write(Genome::B);
	genome.write(Genome::B);

	write2(genome, 0);

	write3(genome, reaction.seeingThing);
	write2(genome, reaction.specific);
	write4(genome, reaction.getFoodType());
}

auto GenomeCreator::addEnvironmentGene(EnvironmentReaction && reaction) -> void {
	genome.write(Genome::B);
	genome.write(Genome::B);

	write2(genome, 1);

	write3(genome, reaction.factor);
	write2(genome, reaction.above);
	write5(genome, (i32)((reaction.threshold + 1.0_f32) * 2.0_f32));
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
