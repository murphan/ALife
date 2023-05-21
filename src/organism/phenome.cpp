//
// Created by Emmet on 11/10/2022.
//

module;

#define _USE_MATH_DEFINES
#include <vector>
#include <algorithm>

export module Phenome;

import Genome;
import Types;
import Cell;
import BodyPart;
import GeneMap;
import GenomeView;
import Body;
import Settings;
import EyeGene;
import GeneWriter;
import UpgradeGene;
import Gene;
import MutationRateGene;
import MovementGene;
import BodyGene;

inline auto readSegment(const Genome & genome, GeneMap::Segment segment) -> GenomeView {
	return GenomeView { &genome, segment.begin, segment.length() };
}

export class Phenome {
public:
	Genome genome;
	Body body;

	/** cells this organism had originally */
	i32 numAliveCells;
	/** how much energy it takes to produce this organism */
	i32 baseBodyEnergy;

	/**
	 * these are not the actual mutation rates
	 * the mutations rates are calculated later
	 * these are just ints some distance away from 0
	 * negative for a smaller rate, positive for a larger rate
	 */
	i32 mutationModifiers[3];

	/**
	 * how many mover cells this organism has,
	 * determines how many tries it gets in a turn to find a valid move
	 */
	i32 moveTries;

	i32 moveLength;
	i32 regenFrequency;

	std::vector<EyeGene> reactions;

	explicit Phenome(Genome && inGenome, Body && inBody, Settings & settings) :
		mutationModifiers { 0, 0, 0 },
		genome(std::move(inGenome)),
		body(std::move(inBody)),
		numAliveCells(0),
		baseBodyEnergy(0),
		moveTries(0),
		moveLength(settings.baseMoveLength),
		regenFrequency(-1),
		reactions()
	{
		if (genome.size() == 0) return;

		auto upgradeGenes = std::vector<UpgradeGene>();
		auto geneMap = GeneMap(genome);

		/* default organism for too-short genome */
		if (geneMap.segments.empty() || !geneMap.segments[0].isCoding) {
			body.directAddCell(Cell::make(BodyPart::MOUTH, 0), 0, 0);

			/* read center cell section */
		} else {
			auto initialGene = readSegment(genome, geneMap.segments[0]);
			auto bodyPart = (BodyPart)(GeneWriter::read7(initialGene, 0) + 1);

			body.directAddCell(Cell::make(bodyPart, 0), 0, 0);
		}

		body.addAnchor({ 0, 0, 0 });

		for (auto i = 1; i < geneMap.segments.size(); ++i) {
			auto segment = geneMap.segments[i];
			if (!segment.isCoding) continue;

			auto gene = readSegment(genome, segment.startOffset(6));

			if (segment.type == Gene::BODY) {
				auto bodyGene = BodyGene(gene);

				body.addCell(
					bodyGene.direction,
					Cell::make(bodyGene.bodyPart, 0),
					bodyGene.usingAnchor,
					bodyGene.setAnchor
				);

			} else if (segment.type == Gene::EYE) {
				reactions.emplace_back(gene);

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
			}
		}

		/* calculate cell based stats */
		for (auto && cell : body.getCells()) {
			onAddCell(cell);
			baseBodyEnergy += cell.cost(settings);
		}

		if (baseBodyEnergy < 1) baseBodyEnergy = 1;
	}

	Phenome(const Phenome & other) = default;
	Phenome(Phenome && other) = default;
	auto operator=(Phenome && other) noexcept -> Phenome & = default;

	auto onAddCell(Cell cell) -> void {
		if (cell.bodyPart() == BodyPart::MOVER) ++moveTries;
		++numAliveCells;
	}

	auto onKilledCell(Cell cell) -> void {
		if (cell.bodyPart() == BodyPart::MOVER) --moveTries;
		--numAliveCells;
	}
};
