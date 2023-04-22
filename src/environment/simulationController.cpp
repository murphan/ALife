
#include <iostream>
#include "environment/simulationController.h"
#include "renderer.h"
#include "../genome/geneMap.h"
#include "genome/rotation.h"

SimulationController::SimulationController(
	Environment && environment,
	OrganismGrid && organismGrid,
	std::default_random_engine & random,
	Ids & ids,
	Settings & settings
) :
	random(random),
	organismGrid(std::move(organismGrid)),
	environment(std::move(environment)),
	organisms(),
	currentTick(0),
	ids(ids),
	settings(settings) {}

auto SimulationController::tick() -> void {
	shuffleOrganisms();

	updateOrganismIndices();

	updateFactors();

	ageOrganismCells();

	moveOrganisms();

	organismCellsTick();

	checkOrganismsDie();

    organismsReproduce();

	updateOrganismIndices();

	++currentTick;
}

inline auto killCell(Organism & organism, Body::Cell & bodyCell, OrganismGrid::Space & gridCell, Settings & settings) -> void {
	bodyCell.setDead(true);
	gridCell.cell() = bodyCell;
	organism.phenome.onRemoveCell(gridCell.getX(), gridCell.getY(), settings);
}

auto SimulationController::shuffleOrganisms() -> void {
	/* organism priority determined by number of move cells */
	std::shuffle(organisms.begin(), organisms.end(), random);
	std::sort(organisms.begin(), organisms.end(), [](Organism & left, Organism & right) {
		return left.phenome.moveTries > right.phenome.moveTries;
	});
}

auto SimulationController::updateOrganismIndices() -> void {
	for (auto index = 0; index < organisms.size(); ++index) {
		organismGrid.placeOrganism(organisms[index], index);
	}
}

auto symmetricRange = std::uniform_int_distribution<i32>(-1, 1);

auto SimulationController::organismSeeingDirection(Organism & organism, i32 index) -> std::optional<Direction> {
	auto && eyeReactions = organism.phenome.eyeReactions;
	auto && eyes = organism.phenome.senses;

	if (eyeReactions.empty() || eyes.empty()) return std::nullopt;

	for (auto && eye : eyes) {
		auto eyeDirection = Direction(eye.senseCell.data()).rotate(organism.rotation);
		auto eyePos = Rotation::rotate({ eye.x, eye.y }, organism.rotation);

		for (auto i = 1; i <= settings.sightRange; ++i) {
			auto seeX = eyePos.x + i * eyeDirection.x();
			auto seeY = eyePos.y + i * eyeDirection.y();

			if (!organismGrid.inBounds(seeX, seeY)) break;

			auto && space = organismGrid.access(seeX, seeY);

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

auto SimulationController::moveOrganisms() -> void {
	for (auto i = 0; i < organisms.size(); ++i) {
		auto && organism = organisms.at(i);
		auto moveTries = organism.phenome.moveTries;
		if (moveTries == 0) continue; //organism.energy == 0 ||

		if (moveTries < std::uniform_int_distribution(1, 2)(random)) continue;

		auto seeingDirection = organismSeeingDirection(organism, i);
		auto lockedOn = seeingDirection.has_value();

		//TODO this section can be cleaned up
		//TODO could also be controlled by a new gene type

		for (auto j = 0; j < moveTries * 2; ++j) {
			auto direction = seeingDirection.has_value() ? seeingDirection.value() : organism.movementDirection;

			if (lockedOn) organism.ticksSinceCollision = 0;

			auto deltaX = direction.x(), deltaY = direction.y();
			auto deltaRotation = (j == 0 && organism.ticksSinceCollision < 12) ? 0 : symmetricRange(random);

			organism.addEnergy(-1);

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

auto SimulationController::checkOrganismsDie() -> void {
	std::erase_if(organisms, [&, this](Organism & organism) {
		auto && body = organism.body();
		auto rotation = organism.rotation;
		auto isAlive = false;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto && cell = body.access(i, j, rotation);

				if (cell.bodyPart() != BodyPart::NONE && !cell.dead()) isAlive = true;
			}
		}

		if (!isAlive) {
			// numAliveCells can't be trusted for some reason........
			if (organism.phenome.numAliveCells != 0) {
				auto putBreakpointHere = organism.phenome.numAliveCells;
			}
			replaceOrganismWithFood(organism);
			ids.removeId(organism.id);
			return true;
		}
		return false;
	});
}

auto SimulationController::replaceOrganismWithFood(Organism & organism) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j, x = organism.x + i;
			auto bodyCell = body.access(i, j, rotation);

			if (bodyCell.bodyPart() != BodyPart::NONE) {
				organismGrid.accessUnsafe(x, y) = OrganismGrid::Space::makeFood(bodyCell);
			}
		}
	}
}

auto SimulationController::serialize() -> json {
	auto renderedBuffer = Renderer::render(environment, organisms, organismGrid);

	auto organismsArray = json::array();
	for (auto && organism : organisms)
		organismsArray.push_back(organism.id);

	return json {
		{ "width",     environment.getWidth() },
		{ "height",    environment.getHeight() },
		{ "tick",      currentTick },
		{ "grid",      Util::base64Encode(renderedBuffer) },
		{ "organisms", organismsArray },
	};
}
auto SimulationController::ageOrganismCells() -> void {
	for (auto && organism : organisms) {
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = organism.y + j;
				auto x = organism.x + i;

				auto && [bodyCell, directX, directY] = body.accessCoord(i, j, rotation);
				auto && gridCell = organismGrid.accessUnsafe(x, y);

				if (bodyCell.bodyPart() == BodyPart::NONE || bodyCell.dead()) continue;

				static auto fiftyFifty = std::uniform_int_distribution<i32>(0, 1);
				if (fiftyFifty(random) == 1) {
					auto newAge = bodyCell.age() + 1;
					bodyCell.setAge(newAge);
					gridCell.cell() = bodyCell;

					if (newAge >= organism.phenome.maxAge(settings)) {
						killCell(organism, bodyCell, gridCell, settings);
					}
				}
			}
		}
	}
}

auto SimulationController::organismCellsTick() -> void {
	for (auto index = 0; index < organisms.size(); ++index) {
		auto && organism = organisms[index];
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = organism.y + j;
				auto x = organism.x + i;

				auto && [bodyCell, directX, directY] = body.accessCoord(i, j, rotation);

				if (bodyCell.bodyPart() == BodyPart::NONE || bodyCell.dead()) continue;

				if (bodyCell.bodyPart() == BodyPart::PHOTOSYNTHESIZER) {
					auto && environmentCell = environment.getCell(x, y);
					auto lightPercentage = (f32)environmentCell.getFactor(Factor::LIGHT) / 127.0_f32;

					auto isOtherAround = [&](i32 deltaX, i32 deltaY) {
						auto && other = organismGrid.access(x + deltaX, y + deltaY);
						return (other.filled() && other.cell().bodyPart() == BodyPart::PHOTOSYNTHESIZER) ? 1 : 0;
					};

					auto othersAround = isOtherAround(0, 1) +
						isOtherAround(1, 0) +
						isOtherAround(0, -1) +
						isOtherAround(-1, 0);

					if (
						std::uniform_real_distribution<f32>(0.0_f32, 1.0_f32)(random) <
						lightPercentage * (1.0_f32 - ((f32)othersAround * 0.25_f32))
					) organism.addEnergy(settings.photosynthesisFactor);

				} else if (bodyCell.bodyPart() == BodyPart::MOUTH) {
					auto tryEatAround = [&](i32 deltaX, i32 deltaY) {
						auto && gridCell = organismGrid.access(x + deltaX, y + deltaY);
						/* organisms can eat dead cells, part of a living organism or not */
						if (gridCell.filled() && gridCell.index() != index && gridCell.cell().dead() && gridCell.cell().bodyPart() != BodyPart::NONE) {
							auto eatGridCell = [&]() {
								auto energy = (i32)((f32)gridCell.cell().cost(settings) * settings.foodEfficiency);

								gridCell = OrganismGrid::Space::makeEmpty();

								organism.addEnergy(energy);
							};

							if (gridCell.isFood()) {
								eatGridCell();

							} else {
								auto && prey = organisms[gridCell.index()];
								auto && originalCell = gridCell.getOriginalCell(prey);
								originalCell = Body::Cell::makeEmpty();

								eatGridCell();
							}
						}
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

						auto && space = organismGrid.access(spaceX, spaceY);
						if (!space.filled() || space.cell().dead()) return;

						auto defenderIndex = space.index();
						if (defenderIndex == index) return;

						constexpr static i32 BLOCK_NONE = 0x0;
						constexpr static i32 BLOCK_PART = 0x1;
						constexpr static i32 BLOCK_FULL = 0x2;
						constexpr static i32 BLOCK_FULL_BUT_COOLER = 0x2;

						auto armorDoesBlock = [&](i32 deltaX, i32 deltaY) -> i32 {
							auto space = organismGrid.access(spaceX + deltaX, spaceY + deltaY);
							if (!space.filled() || space.index() != defenderIndex) return BLOCK_NONE;

							auto isDirectlyAttacked = deltaX == 0 && deltaY == 0;
							auto bodyPart = space.cell().bodyPart();

							auto isAmored = (bodyPart == BodyPart::ARMOR) ||
								(bodyPart == BodyPart::SCAFFOLD && isDirectlyAttacked);

							if (!isAmored) return BLOCK_NONE;

							volatile auto superArmor = bodyPart == BodyPart::ARMOR ?
				                  !space.cell().isModified() ? -1 : space.cell().modifier() : -1;

							auto superCounters = superArmor == superAttack;

							if (isDirectlyAttacked) {
								if (superAttack == -1) {
									return superArmor == -1 ? BLOCK_FULL : BLOCK_FULL_BUT_COOLER;
								} else {
									if (superArmor == -1) {
										return BLOCK_PART;
									} else {
										return superCounters ? BLOCK_FULL : BLOCK_PART;
									}
								}
							} else {
								if (superAttack == -1) {
									return superArmor == -1 ? BLOCK_PART : BLOCK_FULL;
								} else {
									if (superArmor == -1) {
										return BLOCK_NONE;
									} else {
										return superCounters ? BLOCK_PART : BLOCK_NONE;
									}
								}
							}
						};

						auto blockLevel = armorDoesBlock(0, 0) |
						                  armorDoesBlock(1, 0) |
						                  armorDoesBlock(0, 1) |
						                  armorDoesBlock(-1, 0) |
						                  armorDoesBlock(0, -1);

						auto damageDealt = (blockLevel >> 1) & 0x1 ? 0 :
							blockLevel & 0x1 ? settings.weaponDamage - settings.armorPrevents :
							settings.weaponDamage;

						if (damageDealt > 0) {
							auto && defender = organisms[defenderIndex];
							defender.addEnergy(-damageDealt);

							if (defender.energy == 0) {
								killCell(defender, space.getOriginalCell(defender), space, settings);
							}
						}
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

auto SimulationController::organismsReproduce() -> void {
	auto newOrganisms = std::vector<Organism>();

    for (auto && organism : organisms) {
		if (!organism.storedChild.has_value()) {
			auto && phenome = organism.phenome;
			auto bodyEnergy = phenome.bodyEnergy;

			auto reproductionEnergy = settings.reproductionCost;
			auto estimatedChildEnergy = bodyEnergy + settings.startingEnergy;
			auto thresholdEnergy = settings.reproductionThreshold;

			if (organism.energy >= reproductionEnergy + estimatedChildEnergy + thresholdEnergy) {
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
			auto && childPhenome = organism.storedChild.value();

			auto childBodyEnergy = childPhenome.bodyEnergy;

			auto reproductionEnergy = settings.reproductionCost;
			auto childEnergy = settings.startingEnergy;
			auto thresholdEnergy = settings.reproductionThreshold;

			if (organism.energy >= reproductionEnergy + childBodyEnergy + childEnergy + thresholdEnergy) {
				auto newOrganism = tryReproduce(childPhenome, organism, reproductionEnergy, childBodyEnergy, childEnergy);
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

//TODO make it more flexible
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

	auto tries = noMoreThan(1 + organism.phenome.moveTries, 8);

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

auto SimulationController::tryReproduce(Phenome & childPhenome, Organism & organism, i32 reproductionEnergy, i32 childBodyEnergy, i32 childEnergy) -> std::optional<Organism> {
	auto spawnPoint = findChildSpawnPoint(organism, childPhenome);
	if (!spawnPoint.has_value()) return std::nullopt;

	auto [x, y] = spawnPoint.value();

	organism.addEnergy(-(reproductionEnergy + childBodyEnergy + childEnergy));

	return std::make_optional<Organism>(
		std::move(childPhenome),
        ids.newId(),
        x, y,
        organism.rotation,
        childEnergy,
		organism.movementDirection.opposite()
	);
}

/**
 * PLEASE PLEASE PLEASE CHECK FOR NULL
 * C++ HAS NO WAY TO RETURN OPTIONAL REFERENCES
 */
auto SimulationController::getOrganism(u32 id) -> Organism * {
	for (auto & organism : organisms) {
		if (organism.id == id) return &organism;
	}
	return nullptr;
}

auto SimulationController::updateFactors() -> void {
	settings.factorNoises[Factor::TEMPERATURE].tick();
	settings.factorNoises[Factor::LIGHT].tick();
	settings.factorNoises[Factor::OXYGEN].tick();

	refreshFactors();
}

auto SimulationController::refreshFactors() -> void {
	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto && cell = environment.getCell(x, y);

			cell.setFactor(Factor::TEMPERATURE, settings.factorNoises[Factor::TEMPERATURE].getValue(x, y));
			cell.setFactor(Factor::LIGHT, settings.factorNoises[Factor::LIGHT].getValue(x, y));
			cell.setFactor(Factor::OXYGEN, settings.factorNoises[Factor::OXYGEN].getValue(x, y));
		}
	}
}
