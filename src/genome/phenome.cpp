//
// Created by Emmett on 11/10/2022.
//

#include <algorithm>
#include "genomeView.h"
#include "genome/gene/bodyGene.h"
#include "genome/gene/foodGene.h"

#include "phenome.h"

Sense::Sense(i32 x, i32 y, BodyPart senseCell) : x(x), y(y), senseCell(senseCell) {}

/**
 * INTERNAL USE FOR GENOME DECODER
 * points i to after the pattern
 * @return the base value of double pattern found, or -1 if end reached
 */
auto seekDoubles(const Genome & genome, i32 & i) -> i32 {
    if (i >= genome.size() - 1) return -1;

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

Phenome::Phenome(Genome && inGenome, Body && inBody):
	mutationModifiers { 0, 0, 0 },
	genome(std::move(inGenome)),
	body(std::move(inBody)),
	foodStats(),
	moveTries(0),
	senses(),
	eyeReactions(),
	environmentReactions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder();

    auto index = 0;

	auto initialGene = readNBases(genome, index, 2);
	if (!initialGene.empty()) {
		auto center = (BodyPart)Gene::read5(initialGene, 0);
		body.directAddPart(center, 0, 0);
		if (center == BodyPart::MOVER) ++moveTries;
	}

    while (true) {
		auto geneType = seekDoubles(genome, index);

        if (geneType == Genome::A) {
	        auto gene = readNBases(genome, index, 6);
	        if (gene.empty()) return;

	        auto bodyGene = BodyGene(gene);

	        for (auto d = 0; d <= bodyGene.duplicate; ++d) {
		        body.addPart(bodyBuilder, bodyGene.direction, bodyGene.bodyPart, bodyGene.usingAnchor);
	        }

	        if (bodyGene.setsAnchor()) {
		        bodyBuilder.anchors[bodyGene.setAnchor] = { bodyBuilder.currentX, bodyBuilder.currentY };
	        }

	        if (bodyGene.bodyPart == BodyPart::MOVER) ++moveTries;

		} else if (geneType == Genome::B) {
	        auto gene = readNBases(genome, index, 12);
	        if (gene.empty()) break;

	        auto senseType = Gene::read2(gene, 0);

			if (senseType == 0) {
				eyeReactions.emplace_back(gene);
			} else {
				environmentReactions.emplace_back(gene);
			}

		} else if (geneType == Genome::C) {
			auto gene = readNBases(genome, index, 6);
			if (gene.empty()) break;

			auto foodGene = FoodGene(gene);
			auto & foodStat = foodStats[foodGene.foodType];
			foodStat.digestionBonus += foodGene.digestionBonus();
	        foodStat.absoprtionBonus += foodGene.absorptionBonus();

		} else if (geneType == Genome::D) {
			auto gene = readNBases(genome, index, 8);
			if (gene.empty()) break;

			auto bonuses = MutationRateGene(gene).getResults();
			mutationModifiers[MutationRateGene::SUBSTITUTION] += bonuses.substitution;
	        mutationModifiers[MutationRateGene::INSERTION] += bonuses.insertion;
	        mutationModifiers[MutationRateGene::DELETION] += bonuses.deletion;

        } else {
	        break;
		}
    }

	auto descendingPriority = [](ReactionGene & left, ReactionGene & right) {
		return left.priority > right.priority;
	};

	std::sort(eyeReactions.begin(), eyeReactions.end(), descendingPriority);
	std::sort(environmentReactions.begin(), environmentReactions.end(), descendingPriority);

	/* somehow the genome was too small to have an initial body part */
	if (body.getNumCells() == 0) {
		body.directAddPart(BodyPart::MOUTH, 0, 0);
	}
}

auto Phenome::maxAge(i32 lifetimeFactor) const -> i32 {
	return body.getNumCells() * lifetimeFactor;
}

auto Phenome::survivalEnergy(const Settings & settings) const -> i32 {
	return body.getNumCells() * settings.energyFactor;
}
