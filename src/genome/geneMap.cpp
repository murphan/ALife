//
// Created by Emmet on 4/17/2023.
//

module;

#include <random>

export module GeneMap;

import Types;
import Gene;
import Genome;
import GeneWriter;
import BodyGene;
import EyeGene;
import MovementGene;
import MutationRateGene;
import UpgradeGene;
import Base;

/**
 * GENE HEADERS: any combination of three base pairs of A or B
 * examples:
 * AAA BBB ABA BBA ABB
 * @return the index of the start of the header, or -1 if no gene was found
 */
auto findGeneHeader(const Genome & genome, const i32 startIndex) -> i32 {
	if (startIndex >= genome.size() - 2) {
		return -1;
	}

	auto minus2 = genome[startIndex];
	auto minus1 = genome[startIndex + 1];

	auto isGood = [](Base base) -> bool { return base == Base::A || base == Base::B; };

	for (auto index = startIndex + 2; index < genome.size() - 3; ++index) {
		auto current = genome[index];
		if (isGood(minus2) && isGood(minus1) && isGood(current)) {
			return index - 2;
		}

		minus2 = minus1;
		minus1 = current;
	}

	return -1;
}

/**
 * splits geneone into logical areas of:
 * junk - non coding in between genes
 * gene - the coding parts
 *
 * genomes always start with a 6-long special body part coding section
 * (unless too short)
 */
export class GeneMap {
public:
	/**
	 * end exclusive
	 */
	struct Segment {
		bool isCoding;
		Gene::Type type;
		i32 begin;
		i32 end;

		[[nodiscard]] auto length() const -> i32 {
			return end - begin;
		}

		auto startOffset(i32 offset) -> Segment & {
			begin += offset;
			return *this;
		}

		static auto junk(i32 begin, i32 end) -> Segment {
			return { false, Gene::BODY, begin, end };
		}
	};

	std::vector<Segment> segments;

	explicit GeneMap(const Genome & genome): segments() {
		auto pushJunkSegment = [this](i32 begin, i32 end) {
			auto length = end - begin;
			if (length > 0) {
				segments.push_back(Segment::junk(begin, end));
			}
		};

		if (genome.size() < 3) {
			pushJunkSegment(0, genome.size());
			return;
		}

		segments.push_back({ true, Gene::BODY, 0, 3 });

		auto index = 3;
		while (true) {
			auto lastStart = index;
			index = findGeneHeader(genome, index);

			if (index == -1) {
				pushJunkSegment(lastStart, genome.size());
				break;
			} else {
				auto geneType = (Gene::Type)GeneWriter::read5(genome, index + 3);
				auto geneLength = 6 + (
					geneType == Gene::BODY ? BodyGene::LENGTH :
					geneType == Gene::EYE ? EyeGene::LENGTH :
					geneType == Gene::UPGRADE ? UpgradeGene::LENGTH :
					geneType == Gene::MUTATION ? MutationRateGene::LENGTH :
					geneType == Gene::MOVEMENT ? MovementGene::LENGTH :
					-1
				);

				if (geneLength == -1) throw std::exception("Bad gene type???");

				if (index + geneLength <= genome.size()) {
					pushJunkSegment(lastStart, index);
					segments.push_back({true, geneType, index, index + geneLength });
					index += geneLength;
				} else {
					pushJunkSegment(lastStart, genome.size());
					break;
				}
			}
		}
	};

	auto smartMutateCopy(
		const Genome & original,
		f32 substitutionChance,
		f32 insertionChance,
		f32 deletionChance,
		std::default_random_engine random
	) -> Genome {
		auto chance = std::uniform_real_distribution<f32>(0.0_f32, std::nextafter(1.0_f32, FLT_MAX));
		auto otherBase = std::uniform_int_distribution<i32>(1, 3);
		auto anyBase = std::uniform_int_distribution<i32>(0, 3);

		auto newGenome = Genome();

		auto copySegment = [&](Segment segment) {
			for (auto i = segment.begin; i < segment.end; ++i) {
				/* don't allow insertions and deletions inside a gene */
				if (!segment.isCoding) {
					/* prepend insertion */
					if (chance(random) < insertionChance)
						newGenome.write((Base)anyBase(random));

					/* don't copy over this base */
					if (chance(random) < deletionChance)
						continue;
				}

				auto base = original[i];

				/* substitute with a random *other* base, cannot remain the same */
				if (chance(random) < substitutionChance)
					base = (Base)((base + otherBase(random)) % 4);

				newGenome.write(base);
			}
		};

		for (auto && segment : segments) {
			/* entire gene deletion */
			if (segment.isCoding && chance(random) < deletionChance) {
				continue;
			}

			copySegment(segment);

			/* entire gene duplication */
			if (segment.isCoding && chance(random) < insertionChance) {
				copySegment(segment);
			}
		}

		/* chance for final insertion */
		if (chance(random) < insertionChance)
			newGenome.write((Base)anyBase(random));

		return newGenome;
	}
};
