//
// Created by Emmet on 4/17/2023.
//

#ifndef ALIFE_GENEMAP_H
#define ALIFE_GENEMAP_H

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

	GeneMap(Genome & genome);
};

#endif //ALIFE_GENEMAP_H
