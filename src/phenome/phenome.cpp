//
// Created by Emmett on 11/10/2022.
//

#include <algorithm>
#include <genome/genomeView.h>

#include "src/phenome/phenome.h"

Sense::Sense(int x, int y, int sense) : x(x), y(y), sense(sense) {}

FoodStats::FoodStats() : digestionBonus(0), absorptionBonus(0) {}

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
auto readNBases(const Genome & genome, i32 & i, i32 length) -> GenomeView {
    if (i + length > genome.size()) return {};

    auto view = GenomeView { &genome, i, length };

	i += length;

	return view;
}

Phenome::Phenome(Genome & genome):
	body(2, 1),
	foodStats(),
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
				auto specific = decode2(gene, 4) == true;
				auto modifier = decode4(gene, 6);

				eyeReactions.emplace_back(seeingThing, specific, modifier, priority, action);
			} else {
				auto factor = decode3(gene, 2);
				auto above = decode2(gene, 4) == true;
				/* convert 0 - 4 to -1.0 - 1.0 */
				auto threshold = ((f32)decode5(gene, 6) * 0.5_f32) - 1.0_f32;

				environmentReactions.emplace_back(factor, above, threshold, priority, action);
			}

		/* food gene */
		} else if (geneType == Genome::C) {
			auto gene = readNBases(genome, i, 6);
			if (gene.empty()) break;

			auto foodType = decode4(gene, 0);
			for (auto j = 2; j < 6; j += 2) {
				if (decode2(gene, j) == FoodStats::DIGESTION)
					++foodStats[foodType].digestionBonus;
				else
					++foodStats[foodType].absorptionBonus;
			}

		/* mutation rate gene */
		} else if (geneType == Genome::D) {
			auto gene = readNBases(genome, i, 8);

			/* two duplicate sections */
			for (auto j = 0; j < 2; ++j) {
				auto mutationType = (MutationType)decode4(gene, j * 4);
				/* convert to -1 or 1 */
				auto change = decode2(gene, j * 4 + 2) * 2 - 1;

				if (mutationType == MutationType::ALL) {
					for (int & mutationModifier : mutationModifiers)
						mutationModifier += change;
				} else {
					mutationModifiers[mutationType] += change * 2;
				}
			}

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
