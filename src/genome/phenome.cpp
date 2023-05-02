//
// Created by Emmett on 11/10/2022.
//

#include <algorithm>

#include "genomeView.h"
#include "genome/gene/bodyGene.h"

#include "genome/gene/upgradeGene.h"
#include "geneMap.h"
#include "phenome.h"
#include "geneWriter.h"
#include "genome/gene/movementGene.h"

inline auto readSegment(const Genome & genome, GeneMap::Segment segment) -> GenomeView {
	return GenomeView { &genome, segment.begin, segment.length() };
}

auto Phenome::onAddCell(Body::Cell & cell) -> void {
	if (cell.bodyPart() == BodyPart::MOVER) ++moveTries;
	++numAliveCells;
}

auto Phenome::onKilledCell(Body::Cell & cell) -> void {
	if (cell.bodyPart() == BodyPart::MOVER) --moveTries;
	--numAliveCells;
}

Phenome::Phenome(Genome && inGenome, Body && inBody, Settings & settings):
	mutationModifiers { 0, 0, 0 },
	genome(std::move(inGenome)),
	body(std::move(inBody)),
	numAliveCells(0),
	baseBodyEnergy(0),
	moveTries(0),
	moveLength(settings.baseMoveLength),
	eyeReactions()
{
	if (genome.size() == 0) return;

	auto upgradeGenes = std::vector<UpgradeGene>();
	auto geneMap = GeneMap(genome);

	/* default organism for too-short genome */
	if (geneMap.segments.empty() || !geneMap.segments[0].isCoding) {
		body.directAddCell(Body::Cell::make(BodyPart::MOUTH, 0, 0), 0, 0);

	/* read center cell section */
	} else {
		auto initialGene = readSegment(genome, geneMap.segments[0]);
		auto bodyPart = (BodyPart)(GeneWriter::read7(initialGene, 0) + 1);
		auto data = GeneWriter::read8(initialGene, 3);

		body.directAddCell(Body::Cell::make(bodyPart, data, 0), 0, 0);
	}

	for (auto i = 1; i < geneMap.segments.size(); ++i) {
		auto segment = geneMap.segments[i];
		if (!segment.isCoding) continue;

		auto gene = readSegment(genome, segment.startOffset(6));

		if (segment.type == Gene::BODY) {
			auto bodyGene = BodyGene(gene);

			body.addCell(
				bodyGene.direction,
				Body::Cell::make(bodyGene.bodyPart, bodyGene.data, 0),
				bodyGene.usingAnchor,
				bodyGene.setAnchor
			);

		} else if (segment.type == Gene::EYE) {
			eyeReactions.emplace_back(gene);

		} else if (segment.type == Gene::UPGRADE) {
			upgradeGenes.emplace_back(gene);

		} else if (segment.type == Gene::MUTATION) {
			auto change = MutationRateGene(gene).getChange();
			mutationModifiers[0] += change;
			mutationModifiers[1] += change;
			mutationModifiers[2] += change;
		} else if (segment.type == Gene::MOVEMENT) {
			auto change = MovementGene(gene).getChange();

			moveLength += change;
			if (moveLength < 1) moveLength = 1;
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

			auto * cell = body.getNextCellofType(onBodyPart, lastFoundIndex);
			if (cell == nullptr) continue;

			cell->modify(upgradeGene.getModifier());
			baseBodyEnergy += settings.upgradedPartCosts[onBodyPart];
		}
	}

	/* calculate cell based stats */
	for (auto && cell : body.getCells()) {
		onAddCell(cell);
		baseBodyEnergy += cell.cost(settings);
	}
}
