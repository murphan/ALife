//
// Created by Emmet on 3/5/2023.
//

export module InitialGenome;

import Genome;
import GeneWriter;
import BodyPart;
import BodyGene;
import Gene;
import Direction;

export class InitialGenome {
public:
	static auto create() -> Genome {
		auto baseGenome = Genome();

		GeneWriter::write7(baseGenome, BodyPart::MOUTH - 1);
		baseGenome.writeGarbage(21);

		baseGenome.writeHeader();
		GeneWriter::write5(baseGenome, Gene::BODY);
		BodyGene::create(Direction::RIGHT_UP, BodyPart::PHOTOSYNTHESIZER).writeBody(baseGenome);

		baseGenome.writeGarbage(21);

		baseGenome.writeHeader();
		GeneWriter::write5(baseGenome, Gene::BODY);
		BodyGene::create(Direction::RIGHT_DOWN, BodyPart::PHOTOSYNTHESIZER).writeBody(baseGenome);

		baseGenome.writeGarbage(21);

		return baseGenome;
	}
};
