//
// Created by Emmet on 11/10/2022.
//

#include <algorithm>

#include "src/phenome/phenome.h"

constexpr auto BODY_GENE_LENGTH = 7;

i32 directionsWeights[4] {
    /* A */ 2,
    /* B */ 1,
    /* C */ -1,
    /* D */ -2,
};

i32 encoded5Table[16] {
	/* AA */ 0,
	/* AB */ 0,
	/* AC */ 1,
	/* AD */ 2,
	/* BA */ 0,
	/* BB */ 1,
	/* BC */ 2,
	/* BD */ 3,
	/* CA */ 1,
	/* CB */ 2,
	/* CC */ 3,
	/* CD */ 4,
	/* DA */ 2,
	/* DB */ 3,
	/* DC */ 4,
	/* DD */ 4,
};

i32 encoded4Table[16] {
	/* AA */ 0,
	/* AB */ 0,
	/* AC */ 1,
	/* AD */ 0,
	/* BA */ 0,
	/* BB */ 1,
	/* BC */ 1,
	/* BD */ 2,
	/* CA */ 1,
	/* CB */ 2,
	/* CC */ 2,
	/* CD */ 3,
	/* DA */ 3,
	/* DB */ 2,
	/* DC */ 3,
	/* DD */ 3,
};

/**
 * maps 2 bases to a number 0 - 1
 */
i32 encoded3Table[16] {
	/* AA */ 0,
	/* AB */ 0,
	/* AC */ 0,
	/* AD */ 1,
	/* BA */ 0,
	/* BB */ 0,
	/* BC */ 1,
	/* BD */ 1,
	/* CA */ 1,
	/* CB */ 1,
	/* CC */ 2,
	/* CD */ 2,
	/* DA */ 1,
	/* DB */ 2,
	/* DC */ 2,
	/* DD */ 2,
};

/**
 * maps 2 bases to a number 0 - 1
 */
i32 encoded2Table[16] {
	/* AA */ 0,
	/* AB */ 0,
	/* AC */ 0,
	/* AD */ 1,
	/* BA */ 0,
	/* BB */ 0,
	/* BC */ 1,
	/* BD */ 1,
	/* CA */ 0,
	/* CB */ 0,
	/* CC */ 1,
	/* CD */ 1,
	/* DA */ 0,
	/* DB */ 1,
	/* DC */ 1,
	/* DD */ 1,
};

/**
 * maps 3 bases to a number 0 - 15
 */
i32 encoded16Table[64] {
    /* AAA */ 0,
    /* AAB */ 0,
    /* AAC */ 0,
    /* AAD */ 0,
    /* ABA */ 5,
    /* ABB */ 4,
    /* ABC */ 12,
    /* ABD */ 12,
    /* ACA */ 7,
    /* ACB */ 6,
    /* ACC */ 13,
    /* ACD */ 13,
    /* ADA */ 3,
    /* ADB */ 3,
    /* ADC */ 3,
    /* ADD */ 3,
    /* BAA */ 14,
    /* BAB */ 14,
    /* BAC */ 9,
    /* BAD */ 11,
    /* BBA */ 5,
    /* BBB */ 4,
    /* BBC */ 9,
    /* BBD */ 11,
    /* BCA */ 7,
    /* BCB */ 6,
    /* BCC */ 11,
    /* BCD */ 9,
    /* BDA */ 15,
    /* BDB */ 15,
    /* BDC */ 9,
    /* BDD */ 11,
    /* CAA */ 14,
    /* CAB */ 14,
    /* CAC */ 8,
    /* CAD */ 10,
    /* CBA */ 5,
    /* CBB */ 4,
    /* CBC */ 8,
    /* CBD */ 10,
    /* CCA */ 7,
    /* CCB */ 6,
    /* CCC */ 8,
    /* CCD */ 10,
    /* CDA */ 15,
    /* CDB */ 15,
    /* CDC */ 8,
    /* CDD */ 10,
    /* DAA */ 2,
    /* DAB */ 2,
    /* DAC */ 2,
    /* DAD */ 2,
    /* DBA */ 5,
    /* DBB */ 4,
    /* DBC */ 12,
    /* DBD */ 12,
    /* DCA */ 7,
    /* DCB */ 6,
    /* DCC */ 13,
    /* DCD */ 13,
    /* DDA */ 1,
    /* DDB */ 1,
    /* DDC */ 1,
    /* DDD */ 1,
};

/**
 * INTERNAL USE FOR GENOME DECODER
 * points i to after the pattern
 * @return the base value of double pattern found, or -1 if end reached
 */
auto seekDoubles(const Genome & genome, i32 & i) -> i32 {
    if (i >= genome.size() - 1) return true;

    auto last = genome.get(i);
    for (++i; i < genome.size(); ++i) {
        auto current = genome.get(i);
        if (last == current) {
            ++i;
            return last;
        }

        last = current;
    }

    return -1;
}

/**
 * INTERNAL USE FOR GENOME DECODER
 * points i to after the chunk read
 * @return 0 length if end of genome is reached
 */
auto readNBases(const Genome & genome, i32 & i, i32 length) -> std::vector<i32> {
    if (i + length > genome.size()) return {};

    auto vector = std::vector<i32>();
    vector.reserve(length);

    for (auto j = 0; j < length; ++j) {
        vector.push_back(genome.get(i + j));
    }

    i += length;

    return vector;
}

/**
 * same as readNBases() except optimized for 2
 * @return -1 for each if the end of genome is reached
 */
auto read2Bases(const Genome & genome, i32 & i) -> std::pair<i32, i32> {
	if (i + 2 > genome.size()) return std::make_pair(-1, -1);

	auto pair = std::make_pair(genome.get(i), genome.get(i + 1));

	i += 2;

	return pair;
}

auto decode16(std::vector<i32>& gene, i32 offset) -> i32 {
	return encoded16Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}

auto decode5(std::vector<i32>& gene, i32 offset) -> i32 {
	return encoded5Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto decode4(std::vector<i32>& gene, i32 offset) -> i32 {
	return encoded4Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto decode3(std::vector<i32>& gene, i32 offset) -> i32 {
	return encoded3Table[(gene[offset] << 2) | gene[offset + 1]];
}

auto decode2(std::vector<i32>& gene, i32 offset) -> i32 {
	return encoded2Table[(gene[offset] << 2) | gene[offset + 1]];
}

Phenome::Phenome(Genome & genome):
	body(2, 1),
	insertionModifier(0),
	selectionModifier(0),
	substitutionModifier(0),
	senses(),
	eyeReactions(),
	environmentReactions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder();

    auto i = 0;
    while (true) {
		auto geneType = seekDoubles(genome, i);

        if (geneType == Genome::A) {
	        auto gene = readNBases(genome, i, 7);
	        if (gene.empty()) break;

	        auto direction = Direction(decode5(gene, 0) - 2);

	        auto usingAnchor = -1;
	        auto setAnchor = -1;
	        auto doDuplicate = false;

	        if (gene[2] == Genome::B) {
		        usingAnchor = gene[3];
	        } else if (gene[2] == Genome::C) {
		        setAnchor = gene[3];
	        } else if (gene[2] == Genome::D) {
		        doDuplicate = true;
	        }

	        auto bodyPart = decode16(gene, 4) + 1;

	        for (auto d = 0; d <= doDuplicate; ++d) {
		        body.addPart(bodyBuilder, direction, bodyPart, usingAnchor);
	        }

	        if (setAnchor != -1) {
		        bodyBuilder.anchors[setAnchor] = { bodyBuilder.currentX, bodyBuilder.currentY };
	        }
		} else if (geneType == Genome::B) {
	        auto gene = readNBases(genome, i, 12);
	        if (gene.empty()) break;

			auto priority = decode5(gene, 8);
	        auto action = (Reaction::ActionType)decode4(gene, 10);

			auto senseType = decode2(gene, 0);
			if (senseType == 0) {
				auto seeingThing = (EyeReaction::SeeingThing)decode3(gene, 2);
				auto specific = decode2(gene, 4) == 0;
				auto modifier = decode4(gene, 6);

				eyeReactions.emplace_back(seeingThing, specific, modifier, priority, action);
			} else {
				auto factor = decode3(gene, 2);
				auto above = decode2(gene, 4) == 0;
				/* convert 0 - 4 to -1.0 - 1.0 */
				auto threshold = ((f32)decode5(gene, 6) * 0.5_f32) - 1.0_f32;

				environmentReactions.emplace_back(factor, above, threshold, priority, action);
			}

		} else if (geneType == Genome::C) {

		} else if (geneType == Genome::D) {

        } else {
	        break;
		}
    }

	auto descendingPriority = [](Reaction & left, Reaction & right) {
		return left.priority > right.priority;
	};

	std::sort(eyeReactions.begin(), eyeReactions.end(), descendingPriority);
	std::sort(environmentReactions.begin(), environmentReactions.end(), descendingPriority);
}
