//
// Created by Rosa on 1/9/2023.
//

#include "genomeView.h"

GenomeView::GenomeView(): internal(nullptr), offset(0), length(0) {}

GenomeView::GenomeView(const Genome * genome, i32 offset, i32 length) :
	internal(genome), offset(offset), length(length) {}

auto GenomeView::empty() -> bool {
	return internal == nullptr;
}

auto GenomeView::operator[](i32 index) -> Genome::Base {
	return internal->get(index + offset);
}
