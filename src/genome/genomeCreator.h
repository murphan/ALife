//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_GENOMECREATOR_H
#define ALIFE_GENOMECREATOR_H

#include "types.h"
#include "direction.h"
#include "genome.h"

class BodyInstruction {
public:
	Direction direction;
	/** 1 - 16 */
	i32 type;
	i32 anchor;
	i32 setAnchor;
	bool duplicate;

	auto usesAnchor() const -> bool;
	auto setsAnchor() const -> bool;

	static auto create(Direction, i32) -> BodyInstruction;
	static auto createDuplicate(Direction, i32) -> BodyInstruction;
	static auto createUseAnchor(Direction, i32, i32) -> BodyInstruction;
	static auto createSetAnchor(Direction, i32, i32) -> BodyInstruction;
};

class GenomeCreator {
private:
	std::vector<BodyInstruction> bodyInstructions;

public:
	GenomeCreator();

	auto addBodyInstruction(BodyInstruction &&) -> void;

	auto create() -> Genome;
};

#endif //ALIFE_GENOMECREATOR_H
