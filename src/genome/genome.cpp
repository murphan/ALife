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
auto Genome::get(i32 index) const -> i32 {
	/*
	 * bases are packed in big endian order
	 *
	 * byte:  00 00 00 00
	 * index: 0  1  2  3
	 */
	auto byte = code[index / 4_i32];
	return byte >> ((3_i32 - (index % 4_i32)) * 2_i32) & 0x3_u8;
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
auto Genome::write(i32 base) -> void {
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
 * creates a new genome from this genome, but mutated in some way
 *
 * substitutions: changes any single base pair to another random base pair,
 * insertions: adds a base pair somewhere in the code,
 * deletions: deletes a base pair,
 *
 * @param substitutionChance from 0 (never) to 1 (always)
 *
 * mutation rates should probably be nowhere near 1
 */
auto Genome::mutateCopy(
    f32 substitutionChance,
    f32 insertionChance,
    f32 deletionChance
) const -> Genome {
    auto device = std::random_device();
    auto random = std::default_random_engine(device());
    auto chance = std::uniform_real_distribution<f32>(0.0_f32, std::nextafter(1.0_f32, FLT_MAX));
    auto otherBase = std::uniform_int_distribution<i32>(0, 3);
    auto anyBase = std::uniform_int_distribution<i32>(0, 4);

    auto newGenome = Genome();

    for (auto i = 0; i < length; ++i) {
        /* prepend insertion */
        if (chance(random) < insertionChance)
            newGenome.write(anyBase(random));

        /* don't copy over this base */
        if (chance(random) < deletionChance)
            continue;
        
        auto base = get(i);

        /* substitute with a random *other* base, cannot remain the same */
        if (chance(random) < substitutionChance)
            base = (base + otherBase(random)) % 4;

        newGenome.write(base);
    }

    /* chance for final insertion */
    if (chance(random) < insertionChance)
        newGenome.write(anyBase(random));

    return newGenome;
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

auto Genome::mutateCombine(Genome & first, Genome & second) -> Genome {
    auto device = std::random_device();
    auto random = std::default_random_engine(device());

    auto genome = Genome();
    /* does not work on empty genomes */
    if (first.size() == 0 || second.size() == 0) return genome;

    using Pair = std::tuple<Genome &, Genome &>;
    auto [ shorter, longer ] = first.size() < second.size() ? Pair { first, second } : Pair { second, first };

    auto grid = longer.editDistance(longer, shorter);
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

        if (direction == -1) {
            break;

        /* the start of a run, multiple at the same y level */
        } else if (direction == 2) {
            /* this one belongs to the run */
            auto list = std::vector<Util::Coord>(1, { x, y });
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

            includes.push_back(std::move(list));
            if (direction == -1) break;

        } else {
            includes.emplace_back(1, Util::Coord { x, y });
            advanceXY(direction);
        }
    }

    auto differenceRange = std::uniform_int_distribution<i32>(0, longer.size() - shorter.size() + 1);
    auto addExtra = differenceRange(random);

    for (auto & list : includes) {
        for (auto & [px, py] : list) {
            grid[py * width + px] = -(i32)list.size();
        }
    }

    Util::printGrid(grid, width);

    return genome;
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
