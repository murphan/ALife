//
// Created by Emmett on 11/10/2022.
//

#include <algorithm>

#include "genomeView.h"
#include "genome/gene/bodyGene.h"
#include "genome/gene/foodGene.h"
#include "geneMap.h"
#include "phenome.h"

Sense::Sense(i32 x, i32 y, BodyPart senseCell, Direction direction) :
	x(x), y(y), senseCell(senseCell), direction(direction) {}

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

inline auto readSegment(const Genome & genome, GeneMap::Segment segment) -> GenomeView {
	return GenomeView { &genome, segment.begin, segment.length() };
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

	auto geneMap = GeneMap(genome);

	/* default organism for too-short genome */
	if (geneMap.segments.empty() || !geneMap.segments[0].isCoding) {
		body.directAddCell(Body::Cell(BodyPart::MOUTH, Food::FOOD0), 0, 0);
		onAddPart(0, 0, BodyPart::MOUTH);
	}

	{
		auto initialGene = readSegment(genome, geneMap.segments[0]);
		auto center = (BodyPart)Gene::read5(initialGene, 0);
		auto foodType = (Food::Type)Gene::read4(initialGene, 3);

		body.directAddCell(Body::Cell(center, foodType), 0, 0);
		onAddPart(0, 0, center);
	}

	for (auto i = 1; i < geneMap.segments.size(); ++i) {
		auto segment = geneMap.segments[i];
		if (!segment.isCoding) continue;

		auto gene = readSegment(genome, segment.startOffset(2));

		if (segment.type == Genome::A) {
			auto bodyGene = BodyGene(gene);

			body.addCell(
				bodyBuilder,
				bodyGene.direction,
				Body::Cell(bodyGene.bodyPart, bodyGene.foodType),
				bodyGene.usingAnchor
			);
			onAddPart(bodyBuilder.currentX, bodyBuilder.currentY, bodyGene.bodyPart);

			if (bodyGene.setsAnchor()) {
				bodyBuilder.anchors[bodyGene.setAnchor] = { bodyBuilder.currentX, bodyBuilder.currentY };
			}

		} else if (segment.type == Genome::B) {
			eyeReactions.emplace_back(gene);

		} else if (segment.type == Genome::C) {
			auto foodGene = FoodGene(gene);
			auto & foodStat = foodStats[foodGene.foodType];
			foodStat.digestionBonus += foodGene.digestionBonus();
			foodStat.absoprtionBonus += foodGene.absorptionBonus();

		} else if (segment.type == Genome::D) {
			auto change = MutationRateGene(gene).getChange();
			mutationModifiers[0] += change;
			mutationModifiers[1] += change;
			mutationModifiers[2] += change;
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
