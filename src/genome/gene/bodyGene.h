//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_BODYGENE_H
#define ALIFE_BODYGENE_H

#include "../gene.h"
#include "../direction.h"

class BodyGene : public Gene {
public:
	Direction direction;
	/** 1 - 16 */
	i32 type;
	i32 anchor;
	i32 setAnchor;
	bool duplicate;

	auto usesAnchor() const -> bool;
	auto setsAnchor() const -> bool;

	auto headerBase() -> Genome::Base;

	BodyGene::BodyGene(GenomeView & view);

	auto writeBody(Genome & genome) -> void;
};

#endif //ALIFE_BODYGENE_H
