//
// Created by Emmett on 11/10/2022.
//

#include <algorithm>
#include "genomeView.h"
#include "genome/gene/bodyGene.h"
#include "genome/gene/foodGene.h"

#include "phenome.h"

Sense::Sense(i32 x, i32 y, BodyPart senseCell, Direction direction) :
	x(x), y(y), senseCell(senseCell), direction(direction) {}

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

	auto onAddPart = [&](i32 x, i32 y, BodyPart bodyPart) {
		if (bodyPart == BodyPart::MOVER) ++moveTries;

		else if (bodyPart == BodyPart::EYE) {
			senses.emplace_back(x, y, bodyPart, Sense::determineDirection(x, y));
		}
	};

	auto initialGene = readNBases(genome, index, 6);
	if (!initialGene.empty()) {
		auto center = (BodyPart)Gene::read5(initialGene, 0);
		auto foodType = (Food::Type)Gene::read4(initialGene, 3);

		body.directAddCell(Body::Cell(center, foodType), 0, 0);
		onAddPart(0, 0, center);
	}

    while (true) {
		auto geneType = seekDoubles(genome, index);

        if (geneType == Genome::A) {
	        auto gene = readNBases(genome, index, BodyGene::LENGTH);
	        if (gene.empty()) return;

	        auto bodyGene = BodyGene(gene);

			body.addCell(bodyBuilder, bodyGene.direction, Body::Cell(bodyGene.bodyPart, bodyGene.foodType), bodyGene.usingAnchor);
			onAddPart(bodyBuilder.currentX, bodyBuilder.currentY, bodyGene.bodyPart);

	        if (bodyGene.setsAnchor()) {
		        bodyBuilder.anchors[bodyGene.setAnchor] = { bodyBuilder.currentX, bodyBuilder.currentY };
	        }

		} else if (geneType == Genome::B) {
	        auto gene = readNBases(genome, index, ReactionGene::LENGTH);
	        if (gene.empty()) break;

			eyeReactions.emplace_back(gene);

		} else if (geneType == Genome::C) {
			auto gene = readNBases(genome, index, FoodGene::LENGTH);
			if (gene.empty()) break;

			auto foodGene = FoodGene(gene);
			auto & foodStat = foodStats[foodGene.foodType];
			foodStat.digestionBonus += foodGene.digestionBonus();
	        foodStat.absoprtionBonus += foodGene.absorptionBonus();

		} else if (geneType == Genome::D) {
			auto gene = readNBases(genome, index, MutationRateGene::LENGTH);
			if (gene.empty()) break;

			auto change = MutationRateGene(gene).getChange();
			mutationModifiers[0] += change;
	        mutationModifiers[1] += change;
	        mutationModifiers[2] += change;

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
		body.directAddCell(Body::Cell(BodyPart::MOUTH, Food::FOOD0), 0, 0);
		onAddPart(0, 0, BodyPart::MOUTH);
	}
}

auto Phenome::maxAge(i32 lifetimeFactor) const -> i32 {
	return body.getNumCells() * lifetimeFactor;
}

auto Phenome::survivalEnergy(const Settings & settings) const -> i32 {
	return body.getNumCells() * settings.energyFactor;
}
