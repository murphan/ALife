//
// Created by Emmet on 11/10/2022.
//

export module EyeGene;

import Types;
import Gene;
import BodyPart;
import GenomeView;
import Genome;
import GeneWriter;

export class EyeGene : public Gene {
public:
	enum ActionType {
		TOWARD,
		AWAY,
		STOP,
		WANDER,
	};

	static constexpr i32 LENGTH = 6;

	constexpr static const char * ACTION_NAMES[] = {
		"Move Toward",
		"Move Away",
		"Stand Still",
		"Move Randomly",
	};

	BodyPart seeing;
	ActionType actionType;

	explicit EyeGene(GenomeView & view) :
		seeing((BodyPart)(GeneWriter::read7(view, 0) + 1)),
		actionType((ActionType)GeneWriter::read2(view, 3)) {};

	EyeGene(BodyPart bodyPart, ActionType actionType) :
		seeing(bodyPart),
		actionType(actionType) {};

	auto writeBody(Genome & genome) const -> void final {
		GeneWriter::write7(genome, seeing - 1);
		GeneWriter::write2(genome, actionType);
	}
};
