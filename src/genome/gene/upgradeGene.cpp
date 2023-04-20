//
// Created by Emmet on 4/19/2023.
//
#include "upgradeGene.h"

auto const UpgradeGene::LENGTH = 6;

constexpr BodyPart bodyPartMap[] = {
	WEAPON, /* 0 */
	ARMOR, /* 1 */
	SCAFFOLD, /* 2 */
};

auto UpgradeGene::writeBody(Genome & genome) -> void {
	write3(genome, bodyPartType);

	if (bodyPartType == 0 || bodyPartType == 1) {
		write3(genome, modifier);
	} else if (bodyPartType == 3) {
		write2(genome, 0);
	}
}

UpgradeGene::UpgradeGene(GenomeView & view) : bodyPartType(read3(view, 0)), modifier(0) {
	if (bodyPartType == 0 || bodyPartType == 1) {
		modifier = read3(view, 3);
	}
}

auto UpgradeGene::headerBase() -> Genome::Base {
	return Genome::C;
}

auto UpgradeGene::getBodyPart() const -> BodyPart {
	return bodyPartMap[bodyPartType];
}

auto UpgradeGene::getModifier() const -> i32 {
	return modifier;
}
