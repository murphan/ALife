//
// Created by Emmet on 4/19/2023.
//

#ifndef ALIFE_UPGRADEGENE_H
#define ALIFE_UPGRADEGENE_H

#include "../gene.h"
#include "../bodyPart.h"

class UpgradeGene : public Gene {
public:

private:
	i32 bodyPartType;
	i32 modifier;

protected:
	auto writeBody(Genome & genome) -> void override;

public:
	static const i32 LENGTH;

	explicit UpgradeGene(GenomeView & view);

	auto getBodyPart() const -> BodyPart;
	auto getModifier() const -> i32;
};

#endif //ALIFE_UPGRADEGENE_H
