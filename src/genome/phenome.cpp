//
// Created by Emmett on 11/10/2022.
//

#include <algorithm>

#include "genomeView.h"
#include "genome/gene/bodyGene.h"

#include "genome/gene/upgradeGene.h"
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

Phenome::Phenome(Genome && inGenome, Body && inBody, Settings & settings):
	mutationModifiers { 0, 0, 0 },
	genome(std::move(inGenome)),
	body(std::move(inBody)),
	survivalEnergy(0),
	foodStats(),
	moveTries(0),
	senses(),
	eyeReactions(),
	environmentReactions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder();

	auto onAddPart = [&](i32 x, i32 y, BodyPart bodyPart) {
		if (bodyPart == BodyPart::MOVER) ++moveTries;

		else if (bodyPart == BodyPart::EYE) {
			senses.emplace_back(x, y, bodyPart, Sense::determineDirection(x, y));
		}

		survivalEnergy += settings.bodyPartCosts[bodyPart - 1];
	};

	auto upgradeGenes = std::vector<UpgradeGene>();
	auto geneMap = GeneMap(genome);

	/* default organism for too-short genome */
	if (geneMap.segments.empty() || !geneMap.segments[0].isCoding) {
		body.directAddCell(bodyBuilder, Body::Cell::make(BodyPart::MOUTH, Food::FOOD0), 0, 0);
		onAddPart(0, 0, BodyPart::MOUTH);

	/* read center cell section */
	} else {
		auto initialGene = readSegment(genome, geneMap.segments[0]);
		auto center = (BodyPart)Gene::read5(initialGene, 0);
		auto foodType = (Food::Type)Gene::read4(initialGene, 3);

		body.directAddCell(bodyBuilder, Body::Cell::make(center, foodType), 0, 0);
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
				Body::Cell::make(bodyGene.bodyPart, bodyGene.foodType),
				bodyGene.usingAnchor
			);
			onAddPart(bodyBuilder.currentX, bodyBuilder.currentY, bodyGene.bodyPart);

			if (bodyGene.setsAnchor()) {
				bodyBuilder.anchors[bodyGene.setAnchor] = {
					{ bodyBuilder.currentX, bodyBuilder.currentY },
					bodyBuilder.currentDirection,
				};
			}

		} else if (segment.type == Genome::B) {
			eyeReactions.emplace_back(gene);

		} else if (segment.type == Genome::C) {
			upgradeGenes.emplace_back(gene);

		} else if (segment.type == Genome::D) {
			auto change = MutationRateGene(gene).getChange();
			mutationModifiers[0] += change;
			mutationModifiers[1] += change;
			mutationModifiers[2] += change;
		}
	}

	/* apply upgrade genes */
	{
		std::sort(upgradeGenes.begin(), upgradeGenes.end(), [](UpgradeGene & left, UpgradeGene & right) { return left.getBodyPart() < right.getBodyPart(); });

		auto onBodyPart = BodyPart::MOUTH;
		auto lastFoundIndex = 0;
		for (auto && upgradeGene : upgradeGenes) {
			if (upgradeGene.getBodyPart() != onBodyPart) {
				onBodyPart = upgradeGene.getBodyPart();
				lastFoundIndex = 0;
			}

			auto coord = bodyBuilder.getNextCellofType(onBodyPart, lastFoundIndex);
			if (!coord.has_value()) continue;

			auto [x, y] = coord.value();

			body.directAccess(x, y).modify(upgradeGene.getModifier());
			survivalEnergy += settings.upgradedPartCosts[onBodyPart];
		}
	}

	auto descendingPriority = [](ReactionGene & left, ReactionGene & right) {
		return left.priority > right.priority;
	};

	std::sort(eyeReactions.begin(), eyeReactions.end(), descendingPriority);
	std::sort(environmentReactions.begin(), environmentReactions.end(), descendingPriority);
}

auto Phenome::maxAge(i32 lifetimeFactor) const -> i32 {
	return body.getNumCells() * lifetimeFactor;
}
