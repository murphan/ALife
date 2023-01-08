//
// Created by Emmet on 11/10/2022.
//

#include "reaction.h"

Reaction::Reaction(i32 priority, ActionType actionType) :
	priority(priority), actionType(actionType) {}

const char * Reaction::actionNames[3] = {
	"Move Randomly",
	"Move Toward",
	"Move Away",
};

EyeReaction::EyeReaction(SeeingThing seeingThing, bool specific, i32 modifier, i32 priority, ActionType actionType) :
	seeingThing(seeingThing), specific(specific), modifier(modifier), Reaction(priority, actionType) {}

auto EyeReaction::getFoodType() const -> i32 {
	return modifier;
}

auto EyeReaction::getFriendly() const -> bool {
	return modifier < 2;
}

EnvironmentReaction::EnvironmentReaction(i32 factor, bool above, f32 threshold, i32 priority, ActionType actionType) :
	factor(factor), above(above), threshold(threshold), Reaction(priority, actionType) {}

auto EnvironmentReaction::triggers(f32 level) const -> bool {
	return above ? level > threshold : level < threshold;
}
