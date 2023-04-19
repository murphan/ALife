//
// Created by Emmet on 11/9/2022.
//

#include <iostream>

#include "genome.h"
#include "util.h"
#include "random"

char Genome::baseName[4] = { 'A', 'B', 'C', 'D' };

/**
 * default constructor initialized with empty code
 */
Genome::Genome(): code(), length(0) {}

/**
 * move constructor with externally built data
 */
Genome::Genome(std::vector<u8> && code, i32 length): code(std::move(code)), length(length) {}

/**
 * constructs a genome based on its string representation of a sequence of 'A', 'B', 'C', and 'D'
 * throws if a bad string is passed
 */
Genome::Genome(std::string && string) : code(), length((i32)string.length()) {
    code.reserve(Util::ceilDiv(length, 4_u32));

    //TODO should probably be replaced by the write() function

    /* keep a buffer of 4 bases that are written to code all at once */
    auto constructingByte = 0_u8;
    auto baseIndex = 0_i32;

    for (auto &base : string) {
        auto part = base == 'A' ? 0
            : base == 'B' ? 1
            : base == 'C' ? 2
            : base == 'D' ? 3
            : -1;
        if (part == -1) throw std::exception("bad character in genome");

        /* write to the buffer until it is full, then reset */
        constructingByte |= part << (3_i32 - baseIndex) * 2;
        if (++baseIndex == 4) {
            code.push_back(constructingByte);
            constructingByte = 0_u8;
            baseIndex = 0;
        }
    }

    /* write to code any remaining bases if the buffer is not clear */
    if (baseIndex != 0) code.push_back(constructingByte);
}

/**
 * accesses a base at an index in the code
 */
auto Genome::get(i32 index) const -> Base {
	/*
	 * bases are packed in big endian order
	 *
	 * byte:  00 00 00 00
	 * index: 0  1  2  3
	 */
	auto byte = code[index / 4_i32];
	return (Base)(byte >> ((3_i32 - (index % 4_i32)) * 2_i32) & 0x3_u8);
}

auto Genome::size() const -> i32 {
	return length;
}

/**
 * converts internal bit representation to string "ABCD" representation
 * for displaying
 */
auto Genome::toString() const -> std::string {
	auto string = std::string();
	string.reserve(length);

	for (auto i = 0; i < length; ++i) {
		auto base = get(i);
		string.push_back(baseName[base]);
	}

	return string;
}

/**
 * appends a base to the end of the genome
 * used when constructing genome
 */
auto Genome::write(Base base) -> void {
	/* insert in first index into a new byte */
	if (length % 4 == 0) {
		code.push_back(base << 6);

	/* insert within the existing trailing byte */
	} else {
		code[Util::ceilDiv(length, 4) - 1] |= base << ((3 - (length % 4)) * 2);
	}
	++length;
}

/**
 * dynamic programming solution to the edit distance problem
 *
 * @return the intermediate grid produced by the algorithm, of width string0.length() + 1 and height string1.length() + 1,
 * the bottom right corner at (w, h) is the edit distance
 */
auto Genome::editDistance(Genome & genomeX, Genome & genomeY) -> std::vector<i32> {
    auto width = genomeX.size() + 1;
    auto height = genomeY.size() + 1;

    auto grid = std::vector<i32>(width * height);

    /* fill in the first row and column with their index */
    for (auto i = 0; i < width; ++i) grid[i] = i;
    for (auto i = 1; i < height; ++i) grid[i * width] = i;

    /*
     * in row order, first check if the character at string0[x - 1] & string1[y - 1] are equal
     * if so, copy the value to the top left
     * if not, copy the smallest value to the left, up, or left up and add 1
     */
    for (auto y = 1; y < height; ++y) {
        for (auto x = 1; x < width; ++x) {
            if (genomeX.get(x - 1) == genomeY.get(y - 1)) {
                grid[y * width + x] = grid[(y - 1) * width + (x - 1)];
            } else {
                auto a = grid[(y - 1) * width + x];
                auto b = grid[y * width + (x - 1)];
                auto c = grid[(y - 1) * width + (x - 1)];

                grid[y * width + x] = std::min(std::min(a, b), c) + 1;
            }
        }
    }

    return grid;
}

auto Genome::mutateCombine(
	Genome & first,
	Genome & second,
	f32 tolerance,
	f32 substitutionChance,
	f32 insertionChance,
	f32 deletionChance
) -> std::optional<Genome> {
    auto device = std::random_device();
    auto random = std::default_random_engine(device());

    /* does not work on empty genomes */
    if (first.size() == 0 || second.size() == 0) {
	    return std::nullopt;
    }

    using Pair = std::tuple<Genome &, Genome &>;
    auto [ shorter, longer ] = first.size() < second.size() ? Pair { first, second } : Pair { second, first };

    auto grid = longer.editDistance(longer, shorter);

	/* tolerance check for similar genomes */
	auto difference = grid[grid.size() - 1];
	if ((f32)difference / (f32)longer.size() > tolerance) {
		return std::nullopt;
	}

    auto width = longer.size() + 1;

    auto includes = std::vector<std::vector<Util::Coord>>();
    includes.reserve(second.size());

    /**
     * -1: stop
     * 0: up left
     * 1: up
     * 2: left
     */
    auto findDirection = [& grid, width](i32 x, i32 y) -> i32 {
        auto upLeft = grid[(y - 1) * width + (x - 1)]; // 0
        auto up = grid[(y - 1) * width + x]; // 1
        auto left = grid[y * width + (x - 1)]; // 2

        if (x == 1 && y == 1) return -1; /* in corner, nowhere to go */
        if (x == 1) return 1; /* force up when at left wall*/
        if (y == 1) return 2; /* force left when at up wall */

        /* prioritize up left */
        return upLeft <= left && upLeft <= up ? 0 : up <= left ? 1 : 2;
    };

    auto x = longer.size(), y = shorter.size();

    auto advanceXY = [& x, & y](i32 direction) {
        switch (direction) {
            case 0: {
                x -= 1;
                y -= 1;
            } break;
            case 1: { y -= 1; } break;
            default: { x -= 1; }
        }
    };

    auto totalOptionals = 0;

    while (true) {
        auto direction = findDirection(x, y);

		/* list to collect all positions on this y level */
		/* start with this position */
	    auto list = std::vector<Util::Coord>(1, { x, y });

        /* going left, meaning we collect multiple */
		if (direction == 2) {
			++totalOptionals;

			while (true) {
		        /* add subsequent spaces to the left */
		        advanceXY(direction);
		        list.emplace_back(x, y);
		        ++totalOptionals;

		        /* stop if the added one moves up a y level or stops */
				direction = findDirection(x, y);
				if (direction != 2) break;
			}
		}

        includes.push_back(std::move(list));
        if (direction == -1) {
			break;
		} else {
            advanceXY(direction);
		}
    }

	/* debug (display edit distance grid and path) */
    //for (auto & list : includes) {
    //    for (auto & [px, py] : list) {
    //        grid[py * width + px] = -(i32)list.size();
    //    }
    //}
    //Util::printGrid(grid, width);

	auto genomeWrapper = std::make_optional<Genome>();
	auto & genome = genomeWrapper.value();

	/* note that coordinates in the include trail are in grid indices, so have to be -1 to get string indices */
	auto selectBase = [&](Genome & longer, Genome & shorter, std::vector<Util::Coord> & level, i32 i) {
		/* mutations */
		auto chance = std::uniform_real_distribution<f32>(0.0_f32, std::nextafter(1.0_f32, FLT_MAX));

		if (chance(random) < insertionChance) {
			genome.write((Base)std::uniform_int_distribution<i32>(0, 3)(random));
		}

		/* only select if it wasn't deleted */
		if (chance(random) >= deletionChance) {
			/* select from a parent at random */
			i32 selectedBase;
			if (std::uniform_int_distribution<i32>(0, 2)(random) == 0)
				selectedBase = longer.get(level[i].x - 1);
			else
				selectedBase = second.get(level[i].y - 1);

			if (chance(random) < substitutionChance)
				genome.write((Base)((selectedBase + std::uniform_int_distribution<i32>(1, 3)(random)) % 4));
			else
				genome.write((Base)selectedBase);
		}
	};

	/* go backwards in includes list (forwards in genome since include list is constructed backwards) */
	for (auto i = (i32)includes.size() - 1; i >= 0; --i) {
		auto & level = includes.at(i);

		if (level.size() == 1) {
			selectBase(longer, shorter, level, 0);

		} else {
			/* each space in this run has an equal chance of being selected */
			auto chance = std::uniform_int_distribution<i32>(0, (i32)level.size());
			/* keep track of how many of this run got included, so we can always include at least 1 */
			auto numIncluded = 0;

			for (auto j = (i32)level.size() - 1; j >= 0; --j) if (chance(random) == 0) {
				selectBase(longer, shorter, level, j);
				++numIncluded;
			}

			if (numIncluded == 0) {
				selectBase(longer, shorter, level, chance(random));
			}
		}
	}

    return genomeWrapper;
}

inline auto makeNotEither(Genome::Base base, i32 avoid0, i32 avoid1) -> Genome::Base {
	for (auto i = 0; i < 4; ++i) {
		auto tryBase = (base + i) % 4;
		if (tryBase != avoid0 && tryBase != avoid1) return (Genome::Base) tryBase;
	}
	throw std::exception("impossible to reach");
}

auto Genome::writeGarbage(i32 n, Base avoidEnd) -> void {
	internalwriteGarbage(n, avoidEnd);
}

auto Genome::writeGarbage(i32 n) -> void {
	internalwriteGarbage(n, -1);
}

auto Genome::internalwriteGarbage(i32 n, i32 avoidEnd) -> void {
	auto device = std::random_device();
	auto random = std::default_random_engine(device());

	auto last = std::uniform_int_distribution<i32>(0, 3)(random);
	if (n == 1) last = makeNotEither((Base) last, avoidEnd, avoidEnd);

	write((Base) last);

	auto otherRange = std::uniform_int_distribution<i32>(1, 3);
	for (auto i = 1; i < n; ++i) {
		auto current = (last + otherRange(random)) % 4;
		if (i == n - 1) current = makeNotEither((Base) current, (Base) last, avoidEnd);
		write((Base) current);
		last = current;
	}
}

#ifdef DEBUG
Genome::Genome(const Genome & other) : code(other.code), length(other.length) {
    std::cout << "copied genome C";
}

auto Genome::operator=(const Genome & other) -> Genome & {
    code = other.code;
    length = other.length;
    std::cout << "copied genome =";
    return *this;
}
#endif
