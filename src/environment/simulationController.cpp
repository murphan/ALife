
#include "environment/simulationController.h"
#include <iostream>
#include "renderer.h"

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

	organismsEat(settings, damages);

	doDamageAndKill(settings, damages);

    organismsReproduce(settings);

	organismsAgeAndDie(settings);

	tickFood(settings);

	++currentTick;
}

auto symmetricRange = std::uniform_int_distribution<i32>(-1, 1);

auto SimulationController::organismSeeingAction(Organism & organism, Settings & settings) -> ActionReturn {
	auto && eyeReactions = organism.getPhenome().eyeReactions;
	auto && eyes = organism.getPhenome().senses;

	auto bestActionPriority = -1;
	auto bestAction = ActionReturn { ReactionGene::ActionType::WANDER, 0 };

	if (eyeReactions.empty() || eyes.empty()) return bestAction;

	for (auto && eye : eyes) {
		for (auto i = 1; i <= settings.sightRange; ++i) {
			auto seeX = eye.x + i * eye.direction.x();
			auto seeY = eye.y + i * eye.direction.y();

			if (!organismGrid.inBounds(seeX, seeY)) break;

			auto & accessOrganism = organismGrid.accessSafe(seeX, seeY);
			auto & accessMap = environment.getCellSafe(seeX, seeY);

			if (accessOrganism.getFilled() || accessMap.getHasFood() || accessMap.getHasWall()) {
				for (auto && reaction : eyeReactions) {
					if (reaction.priority < bestActionPriority) continue;

					if (
						(
							reaction.seeingThing == EyeGene::CREATURE &&
							accessOrganism.getFilled() && (
								!reaction.specific || reaction.getBodyPart() == accessOrganism.getBodyPart()
							)
						) ||
						(
							reaction.seeingThing == EyeGene::FOOD &&
							accessMap.getHasFood() && (
								!reaction.specific || reaction.getFoodType() == accessMap.getFood().getType()
							)
						) ||
						(
							reaction.seeingThing == EyeGene::WALL &&
							accessMap.getHasWall()
						)
					) {
						bestAction.direction = eye.direction;
						bestAction.actionType = reaction.actionType;
						bestActionPriority = reaction.priority;
					}
				}
				break;
			}
		}
	}

	return bestAction;
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

		auto [actionType, preferredDirection] = organismSeeingAction(organism, settings);

		if (actionType == ReactionGene::ActionType::STOP) continue;

		for (auto j = 0; j < moveTries * 3; ++j) {
			auto direction = actionType == EyeGene::ActionType::WANDER ? organism.movementDirection :
				actionType == EyeGene::ActionType::TOWARD ? preferredDirection : preferredDirection.opposite();

			auto deltaX = direction.x(), deltaY = direction.y();
			auto deltaRotation = symmetricRange(random);

			if (organismGrid.canMoveOrganism(organism, i, deltaX, deltaY, deltaRotation, [&](BodyPart bodyPart, OrganismGrid::Space gridSpace) {})) {
				organismGrid.moveOrganism(organism, i, deltaX, deltaY, deltaRotation);
				break;
			} else {
				organism.movementDirection = randomDirection(random);
			}
		}

		if (++organism.ticksSinceCollision > 8) {
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
		if (organism.energy < organism.getPhenome().survivalEnergy(settings)) {
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
				auto energy = (i32)((f32)settings.energyFactor * settings.foodEfficiency);

				/* replace food, do not add on top */
				environmentCell.setFood(Food(cell.foodType(), energy));
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

        if (!organismGrid.access(x, y).getFilled())
            addFood(x, y, type, energyDefault);
    }
}

auto SimulationController::organismsEat(Settings & settings, std::vector<i32> & damages) -> void {
	for (auto index = 0; index < organisms.size(); ++index) {
		auto && organism = organisms[index];
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = organism.y + j;
				auto x = organism.x + i;

				auto && bodyCell = body.access(i, j, rotation).bodyPart();
				auto && environmentCell = environment.getCell(x, y);

				if (bodyCell == BodyPart::PHOTOSYNTHESIZER) { // && organism.getPhenome().moveTries == 0
					auto lightPercentage = (f32)environmentCell.getFactor(Factor::LIGHT) / 127.0_f32;

					if (std::uniform_real_distribution<f32>(0.0_f32, 1.0_f32)(random) < lightPercentage)
						organism.energy += settings.photosynthesisFactor;

				} else if (bodyCell == BodyPart::MOUTH) {
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
				} else if (bodyCell == BodyPart::WEAPON) {
					auto damageAround = [&](i32 deltaX, i32 deltaY) {
						auto space = organismGrid.accessSafe(x + deltaX, y + deltaY);
						if (!space.getFilled()) return;

						auto enemyIndex = space.getIndex();
						if (enemyIndex == index || space.getBodyPart() == BodyPart::ARMOR) return;

						damages[enemyIndex] += 1 * settings.energyFactor;
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
			auto & phenome = organism.getPhenome();
			auto bodyEnergy = phenome.survivalEnergy(settings);

			auto reproductionEnergy = settings.energyFactor * settings.reproductionCost;
			auto estimatedChildEnergy = bodyEnergy + (settings.energyFactor * settings.startingEnergy);
			auto thresholdEnergy = settings.energyFactor * settings.reproductionThreshold;

			if (organism.energy >= bodyEnergy + reproductionEnergy + estimatedChildEnergy + thresholdEnergy) {
				organism.storedChild = std::make_optional<Phenome>(
					std::move(organism.getGenome().mutateCopy(
						settings.baseMutationRates[0] * pow(settings.mutationFactor, phenome.mutationModifiers[0]),
						settings.baseMutationRates[1] * pow(settings.mutationFactor, phenome.mutationModifiers[1]),
						settings.baseMutationRates[2] * pow(settings.mutationFactor, phenome.mutationModifiers[2])
					)),
					Body(2)
				);
			}
		} else {
			auto & childPhenome = organism.storedChild.value();

			auto bodyEnergy = organism.getPhenome().survivalEnergy(settings);
			auto childBodyEnergy = childPhenome.survivalEnergy(settings);

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
		std::uniform_int_distribution(0, 7)(random)
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
