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

Sense::Sense(i32 x, i32 y, Body::Cell * senseCell) :
	x(x), y(y), senseCell(senseCell) {}

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

auto Phenome::onAddCell(Body::Cell & cell, i32 x, i32 y, Settings & settings) -> void {
	if (cell.bodyPart() == BodyPart::MOVER) ++moveTries;

	else if (cell.bodyPart() == BodyPart::EYE) {
		senses.emplace_back(x, y, &cell);
	}

	bodyEnergy += cell.cost(settings);

	++numAliveCells;
	if (numAliveCells > maxCells) maxCells = numAliveCells;
}

auto Phenome::onRemoveCell(Body::Cell & cell, Settings & settings) -> void {
	if (cell.bodyPart() == BodyPart::MOVER) --moveTries;

	else if (cell.bodyPart() == BodyPart::EYE) {
		std::erase_if(senses, [&](Sense & sense) {
			return sense.senseCell == &cell;
		});
	}

	bodyEnergy -= cell.cost(settings);

	--numAliveCells;
}

Phenome::Phenome(Genome && inGenome, Body && inBody, Settings & settings):
	mutationModifiers { 0, 0, 0 },
	genome(std::move(inGenome)),
	body(std::move(inBody)),
	maxCells(0),
	numAliveCells(0),
	bodyEnergy(0),
	moveTries(0),
	moveLength(settings.baseMoveLength),
	senses(),
	eyeReactions()
{
	if (genome.size() == 0) return;

	auto bodyBuilder = BodyBuilder();

	auto upgradeGenes = std::vector<UpgradeGene>();
	auto geneMap = GeneMap(genome);

	/* default organism for too-short genome */
	if (geneMap.segments.empty() || !geneMap.segments[0].isCoding) {
		auto cell = Body::Cell::make(BodyPart::MOUTH, 0, 0);
		body.directAddCell(bodyBuilder, cell, 0, 0);

	/* read center cell section */
	} else {
		auto initialGene = readSegment(genome, geneMap.segments[0]);
		auto bodyPart = (BodyPart)(GeneWriter::read7(initialGene, 0) + 1);
		auto data = GeneWriter::read8(initialGene, 3);

		auto cell = Body::Cell::make(bodyPart, data, 0);
		body.directAddCell(bodyBuilder, cell, 0, 0);
	}

	for (auto i = 1; i < geneMap.segments.size(); ++i) {
		auto segment = geneMap.segments[i];
		if (!segment.isCoding) continue;

		auto gene = readSegment(genome, segment.startOffset(6));

		if (segment.type == Gene::BODY) {
			auto bodyGene = BodyGene(gene);
			auto cell = Body::Cell::make(bodyGene.bodyPart, bodyGene.data, 0);

			body.addCell(
				bodyBuilder,
				bodyGene.direction,
				cell,
				bodyGene.usingAnchor
			);

			if (bodyGene.setsAnchor()) {
				bodyBuilder.anchors[bodyGene.setAnchor] = {
					bodyBuilder.currentX, bodyBuilder.currentY,
				};
			}

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

			auto coord = bodyBuilder.getNextCellofType(onBodyPart, lastFoundIndex);
			if (!coord.has_value()) continue;

			auto [x, y] = coord.value();

			body.directAccess(x, y).modify(upgradeGene.getModifier());
			bodyEnergy += settings.upgradedPartCosts[onBodyPart];
		}
	}

	/* calculate cell based stats */
	for (auto && insertion : bodyBuilder.insertedOrder) {
		onAddCell(body.directAccess(insertion.x, insertion.y), insertion.x, insertion.y, settings);
	}
}

auto Phenome::maxAge(Settings & settings) const -> i32 {
	return (maxCells * settings.lifetimeFactor) / 2;
}
