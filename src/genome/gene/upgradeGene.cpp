//
// Created by Emmet on 4/19/2023.
//
export module UpgradeGene;

import Types;
import Gene;
import BodyPart;
import GenomeView;
import Genome;
import GeneWriter;

constexpr BodyPart bodyPartMap[] = {
	WEAPON, /* 0 */
	ARMOR, /* 1 */
	SCAFFOLD, /* 2 */
};

export class UpgradeGene : public Gene {
public:

private:
	i32 bodyPartType;
	i32 modifier;

protected:
	auto writeBody(Genome & genome) const -> void final {
		GeneWriter::write3(genome, bodyPartType);

		if (bodyPartType == 0 || bodyPartType == 1) {
			GeneWriter::write3(genome, modifier);
		} else if (bodyPartType == 3) {
			GeneWriter::write2(genome, 0);
		}
	}

public:
	static constexpr i32 LENGTH = 6;

	explicit UpgradeGene(GenomeView & view) : bodyPartType(GeneWriter::read3(view, 0)), modifier(0) {
		if (bodyPartType == 0 || bodyPartType == 1) {
			modifier = GeneWriter::read3(view, 3);
		}
	};

	auto getBodyPart() const -> BodyPart  {
		return bodyPartMap[bodyPartType];
	}

	auto getModifier() const -> i32  {
		return modifier;
	}
};
