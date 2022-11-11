//
// Created by Emmet on 11/10/2022.
//

#include "genomeCreator.h"

auto BodyInstruction::usesAnchor() const -> bool {
	return anchor != -1;
}

auto BodyInstruction::setsAnchor() const -> bool {
	return setAnchor != -1;
}

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

GenomeCreator::GenomeCreator(): bodyInstructions() {}

auto GenomeCreator::addBodyInstruction(BodyInstruction && instruction) -> void {
	bodyInstructions.push_back(instruction);
}

/** each of the directions have multiple encodings, only one is chosen here */
struct { i32 a; i32 b; } directionCodes[8] {
	{ Genome::A, Genome::D },//RIGHT
	{ Genome::B, Genome::B },//RIGHT UP
	{ Genome::A, Genome::A },//UP
	{},//unused
	{},//unused
	{},//unused
	{ Genome::C, Genome::D },//DOWN
	{ Genome::D, Genome::B },//DOWN RIGHT
};

/** one of the encodings for a 3-base encoded number 0 through 15 */
struct { i32 a; i32 b; i32 c; } numberCodes[16] {
	{ Genome::A, Genome::A, Genome::A }, //1
	{ Genome::D, Genome::D, Genome::A }, //2
	{ Genome::D, Genome::A, Genome::B }, //3
	{ Genome::A, Genome::D, Genome::C }, //4
	{ Genome::B, Genome::B, Genome::B }, //5
	{ Genome::D, Genome::B, Genome::A }, //6
	{ Genome::B, Genome::C, Genome::B }, //7
	{ Genome::A, Genome::C, Genome::A }, //8
	{ Genome::C, Genome::C, Genome::C }, //9
	{ Genome::B, Genome::A, Genome::C }, //10
	{ Genome::C, Genome::C, Genome::D }, //11
	{ Genome::B, Genome::A, Genome::D }, //12
	{ Genome::A, Genome::B, Genome::C }, //13
	{ Genome::A, Genome::C, Genome::C }, //14
	{ Genome::B, Genome::A, Genome::A }, //15
	{ Genome::B, Genome::D, Genome::A }, //16
};

auto GenomeCreator::create() -> Genome {
	auto genome = Genome();

	for (auto & instruction : bodyInstructions) {
		/* body gene header */
		genome.write(Genome::A);
		genome.write(Genome::A);

		/* direction */
		auto [d0, d1] = directionCodes[instruction.direction.value()];
		genome.write(d0);
		genome.write(d1);

		/* modifier */
		if (instruction.usesAnchor()) {
			genome.write(Genome::B);
			genome.write(instruction.anchor);

		} else if (instruction.setsAnchor()) {
			genome.write(Genome::C);
			genome.write(instruction.setAnchor);

		} else if (instruction.duplicate) {
			genome.write(Genome::D);
			genome.write(Genome::A);

		} else {
			genome.write(Genome::A);
			genome.write(Genome::A);
		}

		/* type */
		auto [t0, t1, t2] = numberCodes[instruction.type - 1];
		genome.write(t0);
		genome.write(t1);
		genome.write(t2);
	}

	return genome;
}
