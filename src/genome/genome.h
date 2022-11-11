//
// Created by Emmet on 11/9/2022.
//

#pragma once

#ifndef ALIFE_GENOME_H
#define ALIFE_GENOME_H

#include <vector>
#include <string>

#include "types.h"

class Genome {
private:
	std::vector<u8> code;
	i32 length;

public:
	static char baseName[4];

	enum {
		A,
		B,
		C,
		D
	};

	Genome();
	Genome(std::vector<u8> &&, i32);
	explicit Genome(std::string &);

	auto size() const -> i32;
	auto get(i32) const -> i32;

	auto toString() const -> std::string;

	auto write(i32) -> void;
};

#endif //ALIFE_GENOME_H
