//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_BODYGENE_H
#define ALIFE_BODYGENE_H

#include "../gene.h"
#include "../direction.h"
#include "../bodyPart.h"

class BodyGene : public Gene {

public:
	static const i32 LENGTH;

	BodyPart bodyPart;
	Direction direction;
	i32 usingAnchor;
	i32 setAnchor;

	auto usesAnchor() const -> bool;
	auto setsAnchor() const -> bool;

	BodyGene(Direction direction, BodyPart bodyPart, i32 usingAnchor, i32 setAnchor);
	explicit BodyGene(GenomeView & view);

	/* helper factories */

	static auto create(Direction, BodyPart) -> BodyGene;
	static auto createDuplicate(Direction, BodyPart) -> BodyGene;
	static auto createUseAnchor(Direction, BodyPart, i32) -> BodyGene;
	static auto createSetAnchor(Direction, BodyPart, i32) -> BodyGene;

	auto writeBody(Genome & genome) -> void override;
};

#endif //ALIFE_BODYGENE_H
