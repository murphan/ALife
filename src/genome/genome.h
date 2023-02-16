//
// Created by Emmet on 11/9/2022.
//

#pragma once

#ifndef ALIFE_GENOME_H
#define ALIFE_GENOME_H

#include <vector>
#include <string>
#include <optional>

#include "types.h"

//#define DEBUG

/**
 * wrapper for the genetic code of organism
 * genome represented by a sequence of 4 "base pairs" labeled A, B, C, and D
 */
class Genome {
private:
	/**
	 * bases are stored as 2 bits, 4 bases fit in each 8 bit integer
	 * accessing and inserting are taken care of by this class as if each base was stored individually
	 *
	 * there may be more data past the end specified by length if it is not a multiple of 4
	 * only ever access the code at indicies < length
	 *
	 * 00 -> A
	 * 01 -> B
	 * 10 -> C
	 * 11 -> D
	 */
	std::vector<u8> code;
	i32 length;

public:
	static char baseName[4];

	enum Base {
		A,
		B,
		C,
		D
	};

	Genome(Genome && genome) = default;
	Genome();
	Genome(std::vector<u8> &&, i32);

#ifdef DEBUG
    Genome(const Genome &);
    auto operator=(const Genome &) -> Genome &;
#endif

	auto operator=(Genome && other) noexcept -> Genome & = default;

    explicit Genome(std::string &&);

	auto size() const -> i32;
	auto get(i32) const -> Base;

	auto toString() const -> std::string;

	auto write(Base) -> void;

	/**
	 * used for filler in between genes
	 * @param n number of random bases to write
	 * @param avoidEnd the last written base will not be this
	 */
	auto writeGarbage(i32 n, Base avoidEnd) -> void;

    auto mutateCopy(
        f32 substitutionChance,
        f32 insertionChance,
        f32 deletionChance
    ) const -> Genome;

    static auto editDistance(Genome &, Genome &) -> std::vector<i32>;

    static auto mutateCombine(
		Genome & first,
		Genome & second,
		f32 tolerance,
		f32 substitutionChance,
		f32 insertionChance,
		f32 deletionChance
	) -> std::optional<Genome>;
};

#endif //ALIFE_GENOME_H
