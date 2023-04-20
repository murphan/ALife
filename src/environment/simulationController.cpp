
#include <iostream>

#include "environment/simulationController.h"
#include "renderer.h"
#include "../genome/geneMap.h"

SimulationController::SimulationController(Environment && environment, std::default_random_engine & random) :
	random(random),
	organismGrid(environment.getWidth(), environment.getHeight()),
	environment(std::move(environment)),
	organisms(),
	currentTick(0) {}

auto SimulationController::tick(Settings & settings) -> void {
	/* organism priority determined by number of move cells */
	std::shuffle(organisms.begin(), organisms.end(), random);
	std::sort(organisms.begin(), organisms.end(), [](Organism & left, Organism & right) {
		return left.getPhenome().moveTries > right.getPhenome().moveTries;
	});

	updateFactors(settings);

	moveOrganisms(settings);

	auto damages = std::vector<i32>(organisms.size());

	organismCellsTick(settings, damages);

	doDamageAndKill(settings, damages);

    organismsReproduce(settings);

	organismsAgeAndDie(settings);

	tickFood(settings);

	++currentTick;
}

auto symmetricRange = std::uniform_int_distribution<i32>(-1, 1);

auto SimulationController::organismSeeingDirection(Organism & organism, i32 index, Settings & settings) -> std::optional<Direction> {
	auto && eyeReactions = organism.getPhenome().eyeReactions;
	auto && eyes = organism.getPhenome().senses;

	if (eyeReactions.empty() || eyes.empty()) return std::nullopt;

	for (auto && eye : eyes) {
		for (auto i = 1; i <= settings.sightRange; ++i) {
			auto eyeDirection = Direction(eye.senseCell.data()).rotate(organism.rotation);

			auto seeX = eye.x + i * eyeDirection.x();
			auto seeY = eye.y + i * eyeDirection.y();

			if (!organismGrid.inBounds(seeX, seeY)) break;

			auto && space = organismGrid.accessSafe(seeX, seeY);

			if (space.filled() && space.index() != index) {
				for (auto && reaction : eyeReactions) {
					if (space.cell().bodyPart() == reaction.seeing) {
						return std::make_optional<Direction>(reaction.actionType == EyeGene::ActionType::TOWARD ? eyeDirection : eyeDirection.opposite());
					}
				}
			}
		}
	}

	return std::nullopt;
}

auto randomDirection = std::uniform_int_distribution(0, 7);

auto SimulationController::moveOrganisms(Settings & settings) -> void {
	organismGrid.clear();

	for (auto i = 0; i < organisms.size(); ++i) {
		organismGrid.placeOrganism(organisms.at(i), i);
	}

	for (auto i = 0; i < organisms.size(); ++i) {
		auto && organism = organisms.at(i);
		auto moveTries = organism.getPhenome().moveTries;
		if (moveTries == 0) continue;

		if (moveTries < std::uniform_int_distribution(1, 2)(random)) continue;

		auto seeingDirection = organismSeeingDirection(organism, i, settings);
		auto lockedOn = seeingDirection.has_value();

		for (auto j = 0; j < moveTries * 3; ++j) {
			auto direction = seeingDirection.has_value() ? seeingDirection.value() : organism.movementDirection;

			if (lockedOn) organism.ticksSinceCollision = 0;

			auto deltaX = direction.x(), deltaY = direction.y();
			auto deltaRotation = (j == 0 && organism.ticksSinceCollision < 12) ? 0 : symmetricRange(random);

			if (organismGrid.canMoveOrganism(organism, i, deltaX, deltaY, deltaRotation)) {
				organismGrid.moveOrganism(organism, i, deltaX, deltaY, deltaRotation);
				organism.movementDirection = direction;
				break;
			} else if (!lockedOn) {
				organism.movementDirection = randomDirection(random);
			}
		}

		if (++organism.ticksSinceCollision > 12) {
			organism.ticksSinceCollision = 0;
			organism.movementDirection = randomDirection(random);
		}
	}
}

auto SimulationController::doDamageAndKill(Settings & settings, std::vector<i32> & damages) -> void {
	for (auto i = 0; i < organisms.size(); ++i) {
		organisms[i].energy -= damages[i];
	}

	std::erase_if(organisms, [&, this](Organism & organism) {
		if (organism.energy < organism.getPhenome().survivalEnergy) {
			replaceOrganismWithFood(organism, settings);
			return true;
		}
		return false;
	});
}

auto SimulationController::organismsAgeAndDie(Settings & settings) -> void {
	std::erase_if(organisms, [&, this](Organism & organism) {
		if (++organism.age > organism.getPhenome().maxAge(settings.lifetimeFactor)) {
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

			if (cell.bodyPart() != BodyPart::NONE) {
				auto && environmentCell = environment.getCell(x, y);
				auto energy = (i32)(
					(f32)(
						settings.bodyPartCosts[cell.bodyPart() - 1] + (cell.isModified() ? settings.upgradedPartCosts[cell.bodyPart() - 1] : 0)
					) * settings.foodEfficiency
				);

				/* replace food, do not add on top */
				if (environmentCell.getHasFood()) {
					auto && food = environmentCell.getFood();
					food.age = 0;
					food.addEnergy(energy);
				} else {
					environmentCell.setFood(Food((Food::Type)(cell.data() / 2), energy));
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
	auto renderedBuffer = Renderer::render(environment, organisms);

	auto organismsArray = json::array();
	for (auto && organism : organisms)
		organismsArray.push_back(organism.uuid.asString());

	return json {
		{ "width",     environment.getWidth() },
		{ "height",    environment.getHeight() },
		{ "tick",      currentTick },
		{ "grid",      Util::base64Encode(renderedBuffer) },
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

        if (!organismGrid.access(x, y).filled())
            addFood(x, y, type, energyDefault);
    }
}

auto SimulationController::organismCellsTick(Settings & settings, std::vector<i32> & damages) -> void {
	for (auto index = 0; index < organisms.size(); ++index) {
		auto && organism = organisms[index];
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = organism.y + j;
				auto x = organism.x + i;

				auto && bodyCell = body.access(i, j, rotation);
				auto && environmentCell = environment.getCell(x, y);

				if (bodyCell.bodyPart() == BodyPart::PHOTOSYNTHESIZER) {
					auto lightPercentage = (f32)environmentCell.getFactor(Factor::LIGHT) / 127.0_f32;

					auto isOtherAround = [&](i32 deltaX, i32 deltaY) {
						auto && other = organismGrid.accessSafe(x + deltaX, y + deltaY);
						return (other.filled() && other.cell().bodyPart() == BodyPart::PHOTOSYNTHESIZER) ? 1 : 0;
					};

					auto othersAround = isOtherAround(0, 1) +
						isOtherAround(1, 0) +
						isOtherAround(0, -1) +
						isOtherAround(-1, 0);

					if (
						std::uniform_real_distribution<f32>(0.0_f32, 1.0_f32)(random) <
						lightPercentage * (1.0_f32 - ((f32)othersAround * 0.25_f32))
					) organism.energy += settings.photosynthesisFactor;

				} else if (bodyCell.bodyPart() == BodyPart::MOUTH) {
					auto tryEatAround = [&](i32 deltaX, i32 deltaY) {
						auto && cell = environment.getCellSafe(x + deltaX, y + deltaY);
						if (!cell.getHasFood()) return;

						organism.energy += cell.getFood().getEnergy();
						cell.removeFood();
					};

					tryEatAround(0, 0);
					tryEatAround(1, 0);
					tryEatAround(0, 1);
					tryEatAround(-1, 0);
					tryEatAround(0, -1);
				} else if (bodyCell.bodyPart() == BodyPart::WEAPON) {
					auto superAttack = !bodyCell.isModified() ? -1 : bodyCell.modifier();

					auto damageAround = [&](i32 deltaX, i32 deltaY) {
						auto spaceX = x + deltaX, spaceY = y + deltaY;

						auto space = organismGrid.accessSafe(spaceX, spaceY);
						if (!space.filled()) return;

						auto defenderIndex = space.index();
						if (defenderIndex == index) return;

						auto armorDoesBlock = [&](i32 deltaX, i32 deltaY) {
							auto space = organismGrid.accessSafe(spaceX + deltaX, spaceY + deltaY);
							if (!space.filled() || space.index() != defenderIndex) return false;

							if (space.cell().bodyPart() == BodyPart::ARMOR) {
								if (superAttack == -1) return true;

								auto superArmor = !space.cell().isModified() ? -1 : space.cell().modifier();
								return superArmor == superAttack;

							} else if (space.cell().bodyPart() == BodyPart::SCAFFOLD) {
								return superAttack == -1 && space.cell().isModified();

							} else {
								return false;
							}
						};

						damages[defenderIndex] += (
							armorDoesBlock(0, 0) ||
							armorDoesBlock(1, 0) ||
							armorDoesBlock(0, 1) ||
							armorDoesBlock(-1, 0) ||
							armorDoesBlock(0, -1)
						) ? settings.weaponDamage - settings.armorPrevents : settings.weaponDamage;
					};

					damageAround(1, 0);
					damageAround(0, 1);
					damageAround(-1, 0);
					damageAround(0, -1);
				}
			}
		}
	}
}

auto SimulationController::organismsReproduce(Settings & settings) -> void {
	auto newOrganisms = std::vector<Organism>();

    for (auto && organism : organisms) {
		if (!organism.storedChild.has_value()) {
			auto && phenome = organism.getPhenome();
			auto bodyEnergy = phenome.survivalEnergy;

			auto reproductionEnergy = settings.energyFactor * settings.reproductionCost;
			auto estimatedChildEnergy = bodyEnergy + (settings.energyFactor * settings.startingEnergy);
			auto thresholdEnergy = settings.energyFactor * settings.reproductionThreshold;

			if (organism.energy >= bodyEnergy + reproductionEnergy + estimatedChildEnergy + thresholdEnergy) {
				auto geneMap = GeneMap(phenome.genome);
				auto childGenome = geneMap.smartMutateCopy(
					phenome.genome,
					settings.baseMutationRates[0] * (f32)pow(settings.mutationFactor, phenome.mutationModifiers[0]),
					settings.baseMutationRates[1] * (f32)pow(settings.mutationFactor, phenome.mutationModifiers[1]),
					settings.baseMutationRates[2] * (f32)pow(settings.mutationFactor, phenome.mutationModifiers[2]),
					random
				);
				organism.storedChild = std::make_optional<Phenome>(std::move(childGenome), Body(2), settings);
			}
		} else {
			auto & childPhenome = organism.storedChild.value();

			auto bodyEnergy = organism.getPhenome().survivalEnergy;
			auto childBodyEnergy = childPhenome.survivalEnergy;

			auto reproductionEnergy = settings.energyFactor * settings.reproductionCost;
			auto childEnergy = childBodyEnergy + (settings.energyFactor * settings.startingEnergy);
			auto thresholdEnergy = settings.energyFactor * settings.reproductionThreshold;

			if (organism.energy >= bodyEnergy + reproductionEnergy + childEnergy + thresholdEnergy) {
				auto newOrganism = tryReproduce(childPhenome, organism, reproductionEnergy, childEnergy);
				if (newOrganism.has_value()) {
					organism.storedChild.reset();

					/* make sure another organism can't spawn on top of this one */
					organismGrid.placeOrganism(newOrganism.value(), (i32)(organisms.size() + newOrganisms.size()));

					newOrganisms.emplace_back(std::move(newOrganism.value()));
				}
			}
		}
    }

	/* finally put the newly born organisms into the main array */
	for (auto && newOrganism : newOrganisms) {
		organisms.emplace_back(std::move(newOrganism));
	}
}

inline auto noMoreThan(i32 a, i32 b) -> i32 {
	return a < b ? a : b;
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

	auto tries = noMoreThan(1 + organism.getPhenome().moveTries, 8);

	for (auto angle = 0; angle < tries; ++angle) {
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

auto SimulationController::tryReproduce(Phenome & childPhenome, Organism & organism, i32 reproductionEnergy, i32 childEnergy) -> std::optional<Organism> {
	auto spawnPoint = findChildSpawnPoint(organism, childPhenome);
	if (!spawnPoint.has_value()) return std::nullopt;

	auto [x, y] = spawnPoint.value();

	organism.energy -= (reproductionEnergy + childEnergy);

	return std::make_optional<Organism>(
		std::move(childPhenome),
        UUID::generateRandom(),
        x, y,
        organism.rotation,
        childEnergy,
		organism.movementDirection.opposite()
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

auto SimulationController::tickFood(Settings & settings) -> void {
	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto & cell = environment.getCell(x, y);

			if (cell.getHasFood()) {
				if (++cell.getFood().age > settings.maxFoodAge) {
					cell.removeFood();
				}
			}
		}
	}
}
