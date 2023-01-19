//
// Created by Emmet on 11/10/2022.
//

#include "reactionGene.h"

const char * ReactionGene::actionNames[3] = {
	"Move Randomly",
	"Move Toward",
	"Move Away",
};

auto ReactionGene::headerBase() -> Genome::Base {
	return Genome::B;
}

ReactionGene::ReactionGene(i32 priority, ActionType actionType) :
	priority(priority),
	actionType(actionType) {}

ReactionGene::ReactionGene(GenomeView & view) :
	priority(read5(view, 8)),
	actionType((ActionType)read4(view, 10)) {}

auto ReactionGene::writeBody(Genome & genome) -> void {
	write5(genome, priority);
	write4(genome, actionType);
}

/* eye */

EyeGene::EyeGene(GenomeView & view) :
	seeingThing((EyeGene::SeeingThing)read3(view, 2)),
	specific(read2(view, 4) == true),
	modifier(read4(view, 6)),
	ReactionGene(view) {}

EyeGene::EyeGene(SeeingThing seeingThing, bool specific, i32 modifier, i32 priority, ActionType actionType) :
		seeingThing(seeingThing), specific(specific), modifier(modifier), ReactionGene(priority, actionType) {}

auto EyeGene::writeBody(Genome & genome) -> void {
	write2(genome, 0);

	write3(genome, seeingThing);
	write2(genome, specific);
	write4(genome, modifier);

	ReactionGene::writeBody(genome);
}

auto EyeGene::getFoodType() const -> i32 {
	return modifier;
}

auto EyeGene::getFriendly() const -> bool {
	return modifier < 2;
}

/* environment */

EnvironmentGene::EnvironmentGene(i32 factor, bool above, i32 threshold, i32 priority, ActionType actionType) :
		factor(factor), above(above), threshold(threshold), ReactionGene(priority, actionType) {}

auto EnvironmentGene::triggers(f32 level) const -> bool {
	/* convert 0 - 4 to -1.0 - 1.0 */
	auto thresholdValue = ((f32)threshold * 0.5_f32) - 1.0_f32;

	return above ? level > thresholdValue : level < thresholdValue;
}

EnvironmentGene::EnvironmentGene(GenomeView & view) :
	factor(read3(view, 2)),
	above(read2(view, 4) == true),
	threshold(read5(view, 6)),
	ReactionGene(view) {}

auto EnvironmentGene::writeBody(Genome & genome) -> void {
	write2(genome, 1);

	write3(genome, factor);
	write2(genome, above);
	write5(genome, threshold);

	ReactionGene::writeBody(genome);
}
