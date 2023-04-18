//
// Created by Emmet on 4/17/2023.
//
#include "geneMap.h"
#include "gene/bodyGene.h"
#include "gene/foodGene.h"
#include "gene/mutationRateGene.h"
#include "gene/reactionGene.h"

/**
 * points index to the start of the pattern
 * @return the base value of double pattern found, or -1 if end reached
 */
inline auto seekDoubles(const Genome & genome, i32 & index) -> Genome::Base {
	if (index >= genome.size() - 1) {
		index = -1;
		return Genome::A;
	}

	auto last = genome.get(index);
	for (++index; index < genome.size(); ++index) {
		auto current = genome.get(index);
		if (last == current) {
			--index;
			return last;
		}

		last = current;
	}

	index = -1;
	return Genome::A;
}

auto GeneMap::Segment::junk(i32 begin, i32 end) -> GeneMap::Segment {
	return { false, Genome::A, begin, end };
}

auto GeneMap::Segment::length() const -> i32 {
	return end - begin;
}

auto GeneMap::Segment::startOffset(i32 offset) -> GeneMap::Segment & {
	begin += offset;
	return *this;
}

GeneMap::GeneMap(Genome & genome): segments() {
	auto pushJunkSegment = [this](i32 begin, i32 end) {
		auto length = end - begin;
		if (length > 0) {
			segments.push_back(Segment::junk(begin, end));
		}
	};

	if (genome.size() < 6) {
		pushJunkSegment(0, genome.size());
		return;
	}

	segments.push_back({ true, Genome::A, 0, 6 });

	auto index = 6;
	while (true) {
		auto lastStart = index;
		auto headerBase = seekDoubles(genome, index);

		if (index == -1) {
			pushJunkSegment(lastStart, genome.size());
			break;
		} else {
			auto geneLength = 2 + (
				headerBase == Genome::A ? BodyGene::LENGTH :
				headerBase == Genome::B ? ReactionGene::LENGTH :
				headerBase == Genome::C ? FoodGene::LENGTH :
				/*         == Genome::D*/ MutationRateGene::LENGTH
            );

			if (index + geneLength <= genome.size()) {
				pushJunkSegment(lastStart, index);
				segments.push_back({true, headerBase, index, index + geneLength });
				index += geneLength;
			} else {
				pushJunkSegment(lastStart, genome.size());
				break;
			}
		}
	}
}

