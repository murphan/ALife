//
// Created by Rosa on 1/9/2023.
//

#include "bodyGene.h"

const auto BodyGene::LENGTH = 15;

auto BodyGene::usesAnchor() const -> bool {
	return usingAnchor != -1;
}

auto BodyGene::setsAnchor() const -> bool {
	return setAnchor != -1;
}

auto BodyGene::headerBase() -> Genome::Base {
	return Genome::A;
}

BodyGene::BodyGene(Direction direction, BodyPart bodyPart, i32 usingAnchor, i32 setAnchor, Food::Type foodType) :
	direction(direction), bodyPart(bodyPart), usingAnchor(usingAnchor), setAnchor(setAnchor), foodType(foodType) {}

BodyGene::BodyGene(GenomeView & view) :
	direction(read5(view, 3) - 2),
	usingAnchor(-1),
	setAnchor(-1),
	bodyPart((BodyPart)(read7(view, 0) + 1)),
	foodType((Food::Type)read4(view, 12))
{
	auto special = read3(view, 6);

	if (special == 1) {
		usingAnchor = read4(view, 9);
	} else if (special == 2) {
		setAnchor = read4(view, 9);
	}
}

auto BodyGene::writeBody(Genome & genome) -> void {
	write7(genome, bodyPart - 1);
	write5(genome, direction.normalized() + 2);

	if (usesAnchor()) {
		write3(genome, 1);
		write4(genome, usingAnchor);
	} else if (setsAnchor()) {
		write3(genome, 2);
		write4(genome, setAnchor);
	} else {
		write3(genome, 0);
		write4(genome, 0);
	}

	write4(genome, foodType);
}

/* factories */

auto BodyGene::create(Direction dir, BodyPart bodyPart, Food::Type foodType) -> BodyGene {
	return { dir, bodyPart, -1, -1, foodType };
}

auto BodyGene::createUseAnchor(Direction dir, BodyPart bodyPart, i32 anchor, Food::Type foodType) -> BodyGene {
	return { dir, bodyPart, anchor, -1, foodType };
}

auto BodyGene::createSetAnchor(Direction dir, BodyPart bodyPart, i32 anchor, Food::Type foodType) -> BodyGene {
	return { dir, bodyPart, -1, anchor, foodType };
}

auto BodyGene::createDuplicate(Direction dir, BodyPart bodyPart, Food::Type foodType) -> BodyGene {
	return { dir, bodyPart, -1, -1, foodType };
}
