//
// Created by Rosa on 1/9/2023.
//

export module GenomeView;

import Types;
import Genome;
import Base;

/**
 * accesses a section of a genome without allocating any new memory
 * backed by a pointer to the original genome
 * all accesses are treated as if they start from offset
 */
export class GenomeView {
private:
	const Genome * internal;
	i32 offset;

public:
	GenomeView(): internal(nullptr), offset(0), length(0) {}
	GenomeView(const Genome * genome, i32 offset, i32 length) :
		internal(genome), offset(offset), length(length) {};

	i32 length;

	[[nodiscard]] auto empty() const -> bool {
		return internal == nullptr;
	}

	[[nodiscard]] auto operator[](i32 index) const -> Base {
		return internal->operator[](index + offset);
	}
};
