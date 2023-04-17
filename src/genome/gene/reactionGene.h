//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_REACTIONGENE_H
#define ALIFE_REACTIONGENE_H

#include "types.h"
#include "../gene.h"
#include "../../environment/food.h"
#include "../bodyPart.h"

class ReactionGene : public Gene {
protected:
	auto writeBody(Genome & genome) -> void override;

public:
	static const i32 LENGTH;

	enum ActionType {
		TOWARD,
		WANDER,
		STOP,
		AWAY,
	};

	static const char * actionNames[4];

	i32 priority;
	ActionType actionType;

	explicit ReactionGene(GenomeView & view);
	ReactionGene(i32, ActionType);

	auto headerBase() -> Genome::Base override;
};

class EyeGene : public ReactionGene {
public:
	enum SeeingThing {
		WALL,
		FOOD,
		CREATURE,
	};

	constexpr static const char * SEEING_THING_NAMES[3] = {
		"Wall",
		"Food",
		"Creature"
	};

	SeeingThing seeingThing;
	bool specific;
	i32 modifier;

	explicit EyeGene(GenomeView & view);
	EyeGene(SeeingThing, bool, i32, i32, ActionType);

	auto writeBody(Genome & genome) -> void final;

	auto getFoodType() const -> Food::Type;
	auto getBodyPart() const -> BodyPart;
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

	auto getAbove() const -> bool;
	auto getThreshold() const -> f32;
};

#endif //ALIFE_REACTIONGENE_H
