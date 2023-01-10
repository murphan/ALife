//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_BODYGENE_H
#define ALIFE_BODYGENE_H

#include "../gene.h"
#include "../direction.h"

class BodyGene : Gene {
public:
	Direction direction;
	/** 1 - 16 */
	i32 type;
	i32 usingAnchor;
	i32 setAnchor;
	bool duplicate;

	auto usesAnchor() const -> bool;
	auto setsAnchor() const -> bool;

	auto headerBase() -> Genome::Base override;

	BodyGene(Direction direction, i32 type, i32 usingAnchor, i32 setAnchor, bool duplicate);
	explicit BodyGene(GenomeView & view);

	auto writeBody(Genome & genome) -> void override;

	/* helper factories */

	static auto create(Direction, i32) -> BodyGene;
	static auto createDuplicate(Direction, i32) -> BodyGene;
	static auto createUseAnchor(Direction, i32, i32) -> BodyGene;
	static auto createSetAnchor(Direction, i32, i32) -> BodyGene;
};

#endif //ALIFE_BODYGENE_H
