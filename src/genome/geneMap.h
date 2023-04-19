//
// Created by Emmet on 4/17/2023.
//

#ifndef ALIFE_GENEMAP_H
#define ALIFE_GENEMAP_H

#include <random>

#include "genome.h"

/**
 * splits geneone into logical areas of:
 * junk - non coding in between genes
 * gene - the coding parts
 *
 * genomes always start with a 6-long special body part coding section
 * (unless too short)
 */
class GeneMap {
public:
	/**
	 * end exclusive
	 */
	struct Segment {
		bool isCoding;
		Genome::Base type;
		i32 begin;
		i32 end;

		auto length() const -> i32;

		auto startOffset(i32 offset) -> Segment &;

		static auto junk(i32 begin, i32 end) -> Segment;
	};

	std::vector<Segment> segments;

	GeneMap(const Genome & genome);

	auto smartMutateCopy(
		const Genome & original,
		f32 substitutionChance,
		f32 insertionChance,
		f32 deletionChance,
		std::default_random_engine random
	 ) -> Genome;
};

#endif //ALIFE_GENEMAP_H
