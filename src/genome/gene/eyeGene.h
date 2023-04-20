//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_EYEGENE_H
#define ALIFE_EYEGENE_H

#include "types.h"
#include "../gene.h"
#include "../../environment/food.h"
#include "../bodyPart.h"

class EyeGene : public Gene {
public:
	enum ActionType {
		TOWARD,
		AWAY,
		STOP,
		WANDER,
	};

	static const i32 LENGTH;

	constexpr static const char * ACTION_NAMES[] = {
		"Move Toward",
		"Move Away",
		"Stand Still",
		"Move Randomly",
	};

	BodyPart seeing;
	ActionType actionType;

	explicit EyeGene(GenomeView & view);
	EyeGene(BodyPart, ActionType);

	auto headerBase() -> Genome::Base override;

	auto writeBody(Genome & genome) -> void final;
};

#endif //ALIFE_EYEGENE_H
