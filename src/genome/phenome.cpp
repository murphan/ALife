//
// Created by Emmet on 11/10/2022.
//

#include "phenome.h"

constexpr auto BODY_GENE_LENGTH = 7;

i32 directionsWeights[4] {
    /* A */ 2,
    /* B */ 1,
    /* C */ -1,
    /* D */ -2,
};

/**
 * maps any combination of 3 base pairs to a number 1 - 16
 * indexed by converting those base pairs into a 6 digit binary number
 */
i32 encodedNumberTable[64] {
    /* AAA */ 1,
    /* AAB */ 1,
    /* AAC */ 1,
    /* AAD */ 1,
    /* ABA */ 6,
    /* ABB */ 5,
    /* ABC */ 13,
    /* ABD */ 13,
    /* ACA */ 8,
    /* ACB */ 7,
    /* ACC */ 14,
    /* ACD */ 14,
    /* ADA */ 4,
    /* ADB */ 4,
    /* ADC */ 4,
    /* ADD */ 4,
    /* BAA */ 15,
    /* BAB */ 15,
    /* BAC */ 10,
    /* BAD */ 12,
    /* BBA */ 6,
    /* BBB */ 5,
    /* BBC */ 10,
    /* BBD */ 12,
    /* BCA */ 8,
    /* BCB */ 7,
    /* BCC */ 12,
    /* BCD */ 10,
    /* BDA */ 16,
    /* BDB */ 16,
    /* BDC */ 10,
    /* BDD */ 12,
    /* CAA */ 15,
    /* CAB */ 15,
    /* CAC */ 9,
    /* CAD */ 11,
    /* CBA */ 6,
    /* CBB */ 5,
    /* CBC */ 9,
    /* CBD */ 11,
    /* CCA */ 8,
    /* CCB */ 7,
    /* CCC */ 9,
    /* CCD */ 11,
    /* CDA */ 16,
    /* CDB */ 16,
    /* CDC */ 9,
    /* CDD */ 11,
    /* DAA */ 3,
    /* DAB */ 3,
    /* DAC */ 3,
    /* DAD */ 3,
    /* DBA */ 6,
    /* DBB */ 5,
    /* DBC */ 13,
    /* DBD */ 13,
    /* DCA */ 8,
    /* DCB */ 7,
    /* DCC */ 14,
    /* DCD */ 14,
    /* DDA */ 2,
    /* DDB */ 2,
    /* DDC */ 2,
    /* DDD */ 2,
};

/**
 * INTERNAL USE FOR GENOME DECODER
 * points i to after the pattern
 * @return if end of genome is reached
 */
auto seekPattern(const Genome & genome, i32 & i, i32 first, i32 second) -> bool {
    if (i >= genome.size() - 1) return true;

    auto last = genome.get(i);
    for (++i; i < genome.size(); ++i) {
        auto current = genome.get(i);
        if (last == first && current == second) {
            ++i;
            return false;
        }

        last = current;
    }

    return true;
}

/**
 * INTERNAL USE FOR GENOME DECODER
 * points i to after the chunk read
 * @return 0 length if end of genome is reached
 */
auto readNBases(const Genome & genome, i32 & i, i32 length) -> std::vector<i32> {
    if (i + length > genome.size()) return {};

    auto vector = std::vector<i32>();
    vector.reserve(7);

    for (auto j = 0; j < length; ++j) {
        vector.push_back(genome.get(i + j));
    }

    i += length;

    return vector;
}

Phenome::Phenome(Genome & genome):
	body(2, 1), mutationRate(0._f32), actions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder();

    auto i = 0;
    while (true) {
        if (seekPattern(genome, i, Genome::A, Genome::A)) break;
        auto bodyGene = readNBases(genome, i, 7);
        if (bodyGene.empty()) break;

        Direction direction;
        if (bodyGene[0] == bodyGene[1]) {
            direction = Direction(directionsWeights[bodyGene[0]]);
        } else {
            auto magnitude = directionsWeights[bodyGene[0]] + directionsWeights[bodyGene[1]];
            if (magnitude < -2) magnitude = -2;
            else if (magnitude > 2) magnitude = 2;
            direction = Direction(magnitude);
        }

        auto usingAnchor = -1;
        auto setAnchor = -1;
        auto doDuplicate = false;

        if (bodyGene[2] == Genome::B) {
            usingAnchor = bodyGene[3];
        } else if (bodyGene[2] == Genome::C) {
            setAnchor = bodyGene[3];
        } else if (bodyGene[2] == Genome::D) {
            doDuplicate = true;
        }

        auto bodyPart = encodedNumberTable[(bodyGene[4] << 4) | (bodyGene[5] << 2) | bodyGene[6]];

        for (auto d = 0; d <= doDuplicate; ++d) {
            body.addPart(bodyBuilder, direction, bodyPart, usingAnchor);
        }

        if (setAnchor != -1) {
            bodyBuilder.anchors[setAnchor] = { bodyBuilder.currentX, bodyBuilder.currentY };
        }
    }
}
