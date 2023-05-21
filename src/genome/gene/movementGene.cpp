//
// Created by Emmet on 4/26/2023.
//

export module MovementGene;

import Types;
import Gene;
import BodyPart;
import GenomeView;
import Genome;
import GeneWriter;

export class MovementGene : public Gene {
protected:
	auto writeBody(Genome & genome) const -> void final {
		auto half = change / 2;
		auto otherHalf = change - half;

		GeneWriter::write5(genome, half + 2);
		GeneWriter::write5(genome, otherHalf + 2);
	};

private:
	i32 change;

public:
	static constexpr i32 LENGTH = 6;

	explicit MovementGene(GenomeView & view) :
		change(0)
	{
		change += GeneWriter::read5(view, 0) - 2;
		change += GeneWriter::read5(view, 3) - 2;
	}

	explicit MovementGene(i32 change) : change(change) {};

	[[nodiscard]] auto getChange() const -> i32  {
		return change;
	}
};
