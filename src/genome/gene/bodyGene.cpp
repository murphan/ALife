//
// Created by Rosa on 1/9/2023.
//

export module BodyGene;

import Types;
import Gene;
import BodyPart;
import GenomeView;
import Genome;
import GeneWriter;
import Direction;

export class BodyGene : public Gene {
public:
	static constexpr i32 LENGTH = 12;

	BodyPart bodyPart;
	Direction direction;
	i32 usingAnchor;
	i32 setAnchor;

	[[nodiscard]] auto usesAnchor() const -> bool {
		return usingAnchor != -1;
	}

	[[nodiscard]] auto setsAnchor() const -> bool {
		return setAnchor != -1;
	}

	BodyGene(Direction direction, BodyPart bodyPart, i32 usingAnchor, i32 setAnchor) :
		direction(direction), bodyPart(bodyPart), usingAnchor(usingAnchor), setAnchor(setAnchor) {};

	explicit BodyGene(const GenomeView & view) :
		bodyPart((BodyPart)(GeneWriter::read7(view, 0) + 1)),
		direction(GeneWriter::read8(view, 3)),
		usingAnchor(-1),
		setAnchor(-1)
	{
		auto special = GeneWriter::read3(view, 6);

		if (special == 1) {
			usingAnchor = GeneWriter::read4(view, 9);
		} else if (special == 2) {
			setAnchor = GeneWriter::read4(view, 9);
		}
	};

	/* helper factories */

	static auto create(Direction dir, BodyPart bodyPart) -> BodyGene {
		return { dir, bodyPart, -1, -1 };
	}

	static auto createUseAnchor(Direction dir, BodyPart bodyPart, i32 anchor) -> BodyGene  {
		return { dir, bodyPart, anchor, -1 };
	}

	static auto createSetAnchor(Direction dir, BodyPart bodyPart, i32 anchor) -> BodyGene {
		return { dir, bodyPart, -1, anchor };
	};

	auto writeBody(Genome & genome) const -> void final {
		GeneWriter::write7(genome, bodyPart - 1);
		GeneWriter::write8(genome, direction.value());

		if (usesAnchor()) {
			GeneWriter::write3(genome, 1);
			GeneWriter::write4(genome, usingAnchor);
		} else if (setsAnchor()) {
			GeneWriter::write3(genome, 2);
			GeneWriter::write4(genome, setAnchor);
		} else {
			GeneWriter::write3(genome, 0);
			GeneWriter::write4(genome, 0);
		}
	}
};
