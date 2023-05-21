//
// Created by Rosa on 1/12/2023.
//

export module MutationRateGene;

import Types;
import Gene;
import BodyPart;
import GenomeView;
import Genome;
import GeneWriter;

export class MutationRateGene : public Gene {
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

	explicit MutationRateGene(GenomeView & view) :
		change(0)
	{
		change += GeneWriter::read5(view, 0) - 2;
		change += GeneWriter::read5(view, 3) - 2;
	}

	explicit MutationRateGene(i32 change) : change(change) {};

	[[nodiscard]] auto getChange() const -> i32  {
		return change;
	}
};
