//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_REACTION_H
#define ALIFE_REACTION_H

#include "types.h"

class Reaction {
public:
	enum ActionType {
		TOWARD,
		WANDER,
		STOP,
		AWAY,
	};

	Reaction(i32, ActionType);

	static const char * actionNames[3];

	i32 priority;
	ActionType actionType;
};

class EyeReaction : public Reaction {
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

	EyeReaction(SeeingThing, bool, i32, i32, ActionType);

	SeeingThing seeingThing;
	bool specific;

	auto getFoodType() const -> i32;
	auto getFriendly() const -> bool;
};

class EnvironmentReaction : public Reaction {
public:
	bool above;
	f32 threshold;
	i32 factor;

	EnvironmentReaction(i32, bool, f32, i32, ActionType);

	auto triggers(f32 level) const -> bool;
};

#endif //ALIFE_REACTION_H
