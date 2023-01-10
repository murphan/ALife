//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_REACTIONGENE_H
#define ALIFE_REACTIONGENE_H

#include "types.h"
#include "../gene.h"

class ReactionGene : public Gene {
public:
	enum ActionType {
		TOWARD,
		WANDER,
		STOP,
		AWAY,
	};

	static const char * actionNames[3];

	i32 priority;
	ActionType actionType;

	auto headerBase() -> Genome::Base override;

	explicit ReactionGene(GenomeView & view);
	ReactionGene(i32, ActionType);

	auto writeBody(Genome & genome) -> void override;
};

class EyeGene : public ReactionGene {
private:
	/**
	 * internally stored as an int 0-3
	 * if the seeingThing is food, each corresponds to a specific food type
	 * if the seeingThing is creature, 0-1: friendly, 2-3: hostile
	 */
	i32 modifier;

public:
	enum SeeingThing {
		WALL,
		FOOD,
		CREATURE,
	};

	SeeingThing seeingThing;
	bool specific;

	explicit EyeGene(GenomeView & view);
	EyeGene(SeeingThing, bool, i32, i32, ActionType);

	auto writeBody(Genome & genome) -> void final;

	auto getFoodType() const -> i32;
	auto getFriendly() const -> bool;
};

class EnvironmentGene : public ReactionGene {
private:
	bool above;
	i32 threshold;

public:
	i32 factor;

	explicit EnvironmentGene(GenomeView & view);
	EnvironmentGene(i32, bool, i32, i32, ActionType);

	auto writeBody(Genome & genome) -> void final;

	auto triggers(f32 level) const -> bool;
};

#endif //ALIFE_REACTIONGENE_H
