
#include "environment/simulationController.h"
#include <iostream>

SimulationController::SimulationController(Environment && environment) :
	random(std::random_device()()),
	organismGrid(environment.getWidth(), environment.getHeight()),
	environment(std::move(environment)),
	organisms(),
	currentTick(0) {}

auto SimulationController::tempId(i32 index) -> i32 {
	return index + 1;
}

auto SimulationController::tick(Settings & settings) -> void {
	std::shuffle(organisms.begin(), organisms.end(), random);

    for (auto && organism : organisms)
        organism.tick();

	updateFactors(settings);

	auto damages = moveOrganisms(settings);

	doDamageAndKill(damages, settings);

    organismsEat(settings);

    organismsReproduce(settings);

	organismsAgeAndDie(settings);

	++currentTick;
}

auto symmetricRange = std::uniform_int_distribution<i32>(-1, 1);

auto SimulationController::moveOrganisms(Settings & settings) -> std::vector<i32> {
	organismGrid.clear();

	auto damages = std::vector<i32>(organisms.size());

	for (auto i = 0; i < organisms.size(); ++i) {
		organismGrid.placeOrganism(organisms.at(i), tempId(i));
	}

	for (auto i = 0; i < organisms.size(); ++i) {
		auto && organism = organisms.at(i);

		for (auto j = 0; j < organism.getPhenome().moveTries; ++j) {
			auto deltaX = symmetricRange(random);
			auto deltaY = symmetricRange(random);
			auto deltaRotation = symmetricRange(random);

			auto id = tempId(i);

			if (organismGrid.canMoveOrganism(organism, id, deltaX, deltaY, deltaRotation, [&](BodyPart bodyPart, u32 gridSpace) {
				/* attacking another organism */
				if (bodyPart == BodyPart::WEAPON && OrganismGridSpace::getBodyPart(gridSpace) != BodyPart::ARMOR) {
					damages[OrganismGridSpace::getTempId(gridSpace) - 1] +=  1 * settings.energyFactor;

				/* moving into another organism's weapon and getting hurt */
				} else if (bodyPart != BodyPart::ARMOR && OrganismGridSpace::getBodyPart(gridSpace) == BodyPart::WEAPON) {
					damages[i] += 1 * settings.energyFactor;
				}

			})) {
				organismGrid.moveOrganism(organism, id, deltaX, deltaY, deltaRotation);
				break;
			}
		}
	}

	return damages;
}

auto SimulationController::doDamageAndKill(std::vector<i32> & damages, Settings & settings) -> void {
	for (auto i = 0; i < organisms.size(); ++i) {
		organisms[i].energy -= damages[i];
	}

	std::erase_if(organisms, [&, this](Organism & organism) {
		if (organism.energy < organism.getPhenome().survivalEnergy(settings)) {
			replaceOrganismWithFood(organism, settings);
			return true;
		}
		return false;
	});
}

auto SimulationController::organismsAgeAndDie(Settings & settings) -> void {
	std::erase_if(organisms, [&, this](Organism & organism) {
		auto newAge = organism.age;

		if (newAge > organism.getPhenome().maxAge(settings.lifetimeFactor)) {
			replaceOrganismWithFood(organism, settings);
			return true;
		} else {
			return false;
		}
	});
}

auto SimulationController::replaceOrganismWithFood(const Organism & organism, Settings & settings) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j, x = organism.x + i;
			auto cell = body.access(i, j, rotation);

			if (cell != BodyPart::NONE) {
				auto && environmentCell = environment.getCell(x, y);
				auto energy = (i32)((f32)settings.energyFactor * settings.foodEfficiency);

				if (!environmentCell.getHasFood()) {
					environmentCell.setFood(Food(Food::FOOD0, energy));
				}
			}
		}
	}
}

template<typename T>
inline auto push32Bit(std::string & stream, T value) {
	stream.push_back((char)(value >> 24));
	stream.push_back((char)((value >> 16) & 0xff));
	stream.push_back((char)((value >> 8) & 0xff));
	stream.push_back((char)(value & 0xff));
}

auto SimulationController::serialize() -> json {
	auto byteEncodedGrid = std::string();
	byteEncodedGrid.reserve(environment.mapSize());

	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto & cell = environment.getCell(x, y);

			byteEncodedGrid.push_back(cell.getFactor((Factor)0));
			byteEncodedGrid.push_back(cell.getFactor((Factor)1));
			byteEncodedGrid.push_back(cell.getFactor((Factor)2));

			if (cell.getHasFood()) {
				auto & food = cell.getFood();

				byteEncodedGrid.push_back(food.getType());
				byteEncodedGrid.push_back((char)food.getEnergy());
				push32Bit(byteEncodedGrid, (i8)food.getAge());

			} else {
				byteEncodedGrid.push_back(cell.getHasWall() ? -1 : -2);
				byteEncodedGrid.push_back(0);
				push32Bit(byteEncodedGrid, 0);
			}

			byteEncodedGrid.push_back(0);
		}
	}

	auto organismsArray = json::array();
	for (auto && organism : organisms)
		organismsArray.push_back(organism.serialize(false));

	return json {
		{ "width",     environment.getWidth() },
		{ "height",    environment.getHeight() },
		{ "tick",      currentTick },
		{ "grid",      Util::base64Encode(byteEncodedGrid) },
		{ "organisms", organismsArray },
	};
}

auto SimulationController::addFood(i32 foodX, i32 foodY, Food::Type type, i32 energy) -> void {
    environment.getCell(foodX, foodY).setFood(Food(type, energy));
}

auto SimulationController::scatterFood(Food::Type type, i32 numFood, i32 energyDefault) -> void {
    for (int i = 0; i < numFood; ++i) {
        auto x = std::uniform_int_distribution(0, organismGrid.getWidth() - 1)(random);
        auto y = std::uniform_int_distribution(0, organismGrid.getHeight() - 1)(random);

        if (!organismGrid.organismInSpace(x, y))
            addFood(x, y, type, energyDefault);
    }
}

/**
 * For each Organism checks each body part looking for overlapping food cells
 * If found, eats food by adding the energy to the organism and destroying the food
 */
auto SimulationController::organismsEat(Settings & settings) -> void {
	for (auto && organism : organisms) {
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = organism.y + j;
				auto x = organism.x + i;

				auto && bodyCell = body.access(i, j, rotation);
				auto && environmentCell = environment.getCell(x, y);

				if (bodyCell == BodyPart::PHOTOSYNTHESIZER && organism.getPhenome().moveTries == 0) {
					auto lightPercentage = (f32)environmentCell.getFactor(Factor::LIGHT) / 127.0_f32;

					if (std::uniform_real_distribution<f32>(0.0_f32, 1.0_f32)(random) < lightPercentage)
						organism.energy += settings.photosynthesisFactor;

				} else if (bodyCell == BodyPart::MOUTH) {
					if (environmentCell.getHasFood()) {
						organism.energy += environmentCell.getFood().getEnergy();
						environmentCell.removeFood();
					}
				}
			}
		}
	}
}

auto SimulationController::organismsReproduce(Settings & settings) -> void {
	auto newOrganisms = std::vector<Organism>();

    for (auto && organism : organisms) {
		auto bodyEnergy = (f32)organism.getPhenome().survivalEnergy(settings);

		auto reproductionEnergy = (i32)(bodyEnergy * settings.reproductionCost);
		auto childEnergy = (i32)(bodyEnergy * (1.0_f32 + settings.startingEnergy));
		auto thresholdEnergy = (i32)(bodyEnergy * settings.reproductionThreshold);

        if (organism.energy > (i32)bodyEnergy + reproductionEnergy + childEnergy + thresholdEnergy) {
	        auto newOrganism = tryReproduce(organism, reproductionEnergy, childEnergy);
			if (newOrganism.has_value()) {
				/* make sure another organism can't spawn on top of this one */
				organismGrid.placeOrganism(newOrganism.value(), tempId(organisms.size() + newOrganisms.size()));

				newOrganisms.emplace_back(std::move(newOrganism.value()));
			}
        }
    }

	/* finally put the newly born organisms into the main array */
	for (auto && newOrganism : newOrganisms) {
		organisms.emplace_back(std::move(newOrganism));
	}
}

auto SimulationController::findChildSpawnPoint(Organism & organism, Phenome & childPhenome) -> std::optional<Util::Coord> {
	auto rotation = organism.rotation;

	auto childLeft = childPhenome.body.getLeft(rotation);
	auto childRight = childPhenome.body.getRight(rotation);
	auto childUp = childPhenome.body.getUp(rotation);
	auto childDown = childPhenome.body.getDown(rotation);

	auto range = std::uniform_int_distribution<i32>(0, 7);
	auto baseDirection = Direction(range(random));

	auto left = organism.body().getLeft(rotation);
	auto right = organism.body().getRight(rotation);
	auto up = organism.body().getUp(rotation);
	auto down = organism.body().getDown(rotation);

	auto spawnRight = organism.x + right + 1 - childLeft + 1;
	auto spawnLeft = organism.x + left - 1 - childRight - 1;
	auto spawnUp = organism.y + up + 1 - childDown + 1;
	auto spawnDown = organism.y + down - 1 - childUp - 1;

	for (auto angle = 0; angle < 8; ++angle) {
		auto direction = baseDirection.rotate(angle);

		i32 x;
		i32 y;

		switch (direction.value()) {
			case Direction::RIGHT: {
				x = spawnRight;
				y = organism.y;
			} break;
			case Direction::RIGHT_UP: {
				x = spawnRight;
				y = spawnUp;
			} break;
			case Direction::UP: {
				x = organism.x;
				y = spawnUp;
			} break;
			case Direction::LEFT_UP: {
				x = spawnLeft;
				y = spawnUp;
			} break;
			case Direction::LEFT: {
				x = spawnLeft;
				y = organism.y;
			} break;
			case Direction::LEFT_DOWN: {
				x = spawnLeft;
				y = spawnDown;
			} break;
			case Direction::DOWN: {
				x = organism.x;
				y = spawnDown;
			} break;
			case Direction::RIGHT_DOWN: {
				x = spawnRight;
				y = spawnDown;
			} break;
		}

		if (organismGrid.isSpaceAvailable(childPhenome.body, x, y, rotation))
			return std::make_optional<Util::Coord>(x, y);
	}

	return std::nullopt;
}

auto SimulationController::tryReproduce(Organism & organism, i32 reproductionEnergy, i32 childEnergy) -> std::optional<Organism> {
	auto childPhenome = Phenome(
		std::move(organism.getGenome().mutateCopy(0.01f, 0.01f, 0.01f)),
		Body(2, BodyPart::MOUTH)
	);

	auto spawnPoint = findChildSpawnPoint(organism, childPhenome);
	if (!spawnPoint.has_value()) return std::nullopt;

	auto [x, y] = spawnPoint.value();

	organism.energy -= (reproductionEnergy + childEnergy);

	return std::make_optional<Organism>(
		std::move(childPhenome),
        UUID::generateRandom(),
        x, y,
        organism.rotation,
        childEnergy
	);
}

auto SimulationController::howMuchFood() -> i32 {
    auto numFood = 0;
    for (int i = 0; i < environment.getWidth(); ++i) {
        for (int j = 0; j < environment.getHeight(); ++j) {
            if (environment.getCell(i, j).getHasFood()) ++numFood;
        }
    }
    return numFood;
}

/**
 * PLEASE PLEASE PLEASE CHECK FOR NULL
 * C++ HAS NO WAY TO RETURN OPTIONAL REFERENCES
 */
auto SimulationController::getOrganism(UUID & id) -> Organism * {
	auto str = id.asString();
	for (auto & organism : organisms) {
		if (organism.uuid == id) return &organism;
	}
	return nullptr;
}

auto SimulationController::updateFactors(Settings & settings) -> void {
	settings.factorNoises[Factor::TEMPERATURE].tick();
	settings.factorNoises[Factor::LIGHT].tick();
	settings.factorNoises[Factor::OXYGEN].tick();

	refreshFactors(settings);
}

auto SimulationController::refreshFactors(Settings & settings) -> void {
	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto && cell = environment.getCell(x, y);

			cell.setFactor(Factor::TEMPERATURE, settings.factorNoises[Factor::TEMPERATURE].getValue(x, y));
			cell.setFactor(Factor::LIGHT, settings.factorNoises[Factor::LIGHT].getValue(x, y));
			cell.setFactor(Factor::OXYGEN, settings.factorNoises[Factor::OXYGEN].getValue(x, y));
		}
	}
}
