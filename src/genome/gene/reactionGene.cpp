//
// Created by Emmet on 11/10/2022.
//

#include "reactionGene.h"

const auto ReactionGene::LENGTH = 15;

const char * ReactionGene::actionNames[4] = {
	"Move Toward",
	"Move Randomly",
	"Stand Still",
	"Move Away"
};

auto ReactionGene::headerBase() -> Genome::Base {
	return Genome::B;
}

ReactionGene::ReactionGene(i32 priority, ActionType actionType) :
	priority(priority),
	actionType(actionType) {}

ReactionGene::ReactionGene(GenomeView & view) :
	priority(read5(view, 9)),
	actionType((ActionType)read4(view, 12)) {}

auto ReactionGene::writeBody(Genome & genome) -> void {
	write5(genome, priority);
	write4(genome, actionType);
}

/* eye */

EyeGene::EyeGene(GenomeView & view) :
	seeingThing((EyeGene::SeeingThing)read3(view, 0)),
	specific(read2(view, 3) == 1),
	modifier(seeingThing == SeeingThing::CREATURE ? read6(view, 6) : read4(view, 6)),
	ReactionGene(view) {}

EyeGene::EyeGene(SeeingThing seeingThing, bool specific, i32 modifier, i32 priority, ActionType actionType) :
		seeingThing(seeingThing), specific(specific), modifier(modifier), ReactionGene(priority, actionType) {}

auto EyeGene::writeBody(Genome & genome) -> void {
	write3(genome, seeingThing);
	write2(genome, specific);

	if (seeingThing == SeeingThing::CREATURE) {
		write6(genome, modifier);
	} else {
		write4(genome, modifier);
	}

	ReactionGene::writeBody(genome);
}

auto EyeGene::getFoodType() const -> Food::Type {
	return (Food::Type)modifier;
}

auto EyeGene::getBodyPart() const -> BodyPart {
	return (BodyPart)modifier;
}

/* environment */
/* NOT IMPLEMENTED */

EnvironmentGene::EnvironmentGene(i32 factor, bool above, i32 threshold, i32 priority, ActionType actionType) :
		factor(factor), above(above), threshold(threshold), ReactionGene(priority, actionType) {}

auto EnvironmentGene::triggers(f32 level) const -> bool {
	return above ? level > getThreshold() : level < getThreshold();
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

auto EnvironmentGene::getAbove() const -> bool {
	return above;
}

auto EnvironmentGene::getThreshold() const -> f32 {
	/* convert 0 - 4 to -1.0 - 1.0 */
	return ((f32)threshold * .5_f32) - 1._f32;
}
