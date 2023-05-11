//
// Created by Emmet on 4/27/2023.
//
#include "photosynthesizer.h"

auto Photosynthesizer::tick(i32 x, i32 y, Organism & organism, Environment & environment, OrganismGrid & organismGrid, Settings & settings, std::vector<Organism> & organisms, std::default_random_engine & random) -> void {
	auto && environmentCell = environment.accessUnsafe(x, y);
	auto lightPercentage = (f32)environmentCell.getFactor(Factor::LIGHT) / 127.0_f32;

	auto isOtherAround = [&](i32 deltaX, i32 deltaY) {
		auto && space = organismGrid.access(x + deltaX, y + deltaY);
		return (space.fromOrganism() && space.cell(organisms).bodyPart() == BodyPart::PHOTOSYNTHESIZER) ? 1 : 0;
	};

	auto othersAround = isOtherAround(0, 1) +
	                    isOtherAround(1, 0) +
	                    isOtherAround(0, -1) +
	                    isOtherAround(-1, 0);

	if (
		std::uniform_real_distribution<f32>(0.0_f32, 1.0_f32)(random) <
		lightPercentage * (1.0_f32 - ((f32)othersAround * 0.25_f32))
	) organism.addEnergy(settings.photosynthesisFactor, settings);
}
