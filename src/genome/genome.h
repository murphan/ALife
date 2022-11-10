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

	Genome(std::string &);

	auto size() -> i32;
	auto get(i32) -> i32;

	auto toString() -> std::string;
};

#endif //ALIFE_GENOME_H
