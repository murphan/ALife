//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_BODYGENE_H
#define ALIFE_BODYGENE_H

#include "../gene.h"
#include "../direction.h"
#include "../bodyPart.h"

class BodyGene : public Gene {
protected:
	auto writeBody(Genome & genome) -> void override;

public:
	static const i32 LENGTH;

	BodyPart bodyPart;
	Direction direction;
	i32 usingAnchor;
	i32 setAnchor;
	i32 data;

	auto usesAnchor() const -> bool;
	auto setsAnchor() const -> bool;

	auto headerBase() -> Genome::Base override;

	BodyGene(Direction direction, BodyPart bodyPart, i32 usingAnchor, i32 setAnchor, i32 data);
	explicit BodyGene(GenomeView & view);

	/* helper factories */

	static auto create(Direction, BodyPart, i32 data) -> BodyGene;
	static auto createDuplicate(Direction, BodyPart, i32 data) -> BodyGene;
	static auto createUseAnchor(Direction, BodyPart, i32, i32 data) -> BodyGene;
	static auto createSetAnchor(Direction, BodyPart, i32, i32 data) -> BodyGene;
};

#endif //ALIFE_BODYGENE_H
