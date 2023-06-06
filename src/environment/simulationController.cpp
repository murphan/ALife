
#include "environment/simulationController.h"
#include "renderer.h"
#include "../genome/geneMap.h"
#include "genome/rotation.h"
#include "environment/cell/mouth.h"
#include "environment/cell/weapon.h"
#include "environment/cell/photosynthesizer.h"

SimulationController::SimulationController(
	Settings & settings,
	Environment && environment,
	OrganismGrid && organismGrid,
	std::default_random_engine random,
	Ids && ids,
	Tree && tree
) :
	random(random),
	organismGrid(std::move(organismGrid)),
	environment(std::move(environment)),
	organisms(),
	currentTick(0),
	ids(std::move(ids)),
	settings(settings),
	tree(std::move(tree)) {}

auto SimulationController::operator=(SimulationController && other) noexcept -> SimulationController & {
	environment = std::move(other.environment);
	organismGrid = std::move(other.organismGrid);
	random = other.random;
	ids = std::move(other.ids);
	tree = std::move(other.tree);
	organisms = std::move(other.organisms);

	return *this;
}

auto SimulationController::tick(Tree::Node *& activeNode) -> void {
	shuffleOrganisms();

	renderOrganismGrid();

	updateFactors();

	ageOrganismCells();

	moveOrganisms();

	organismCellsTick();

	checkOrganismsDie(activeNode);

    organismsReproduce();

	++currentTick;
}

auto SimulationController::shuffleOrganisms() -> void {
	/* organism priority determined by number of move cells */
	std::shuffle(organisms.begin(), organisms.end(), random);
	std::sort(organisms.begin(), organisms.end(), [](Organism & left, Organism & right) {
		return left.phenome.moveTries > right.phenome.moveTries;
	});
}

auto SimulationController::renderOrganismGrid() -> void {
	organismGrid.clear();

	for (auto index = 0; index < organisms.size(); ++index) {
		organismGrid.placeOrganism(organisms[index], index);
	}

	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto && food = environment.accessUnsafe(x, y).food;

			if (food.filled() && !food.broken()) {
				organismGrid.accessUnsafe(x, y) = OrganismGrid::Space::makeFood(x, y);
			}
		}
	}
}

auto SimulationController::organismSeeingDirection(Organism & organism, i32 index) -> std::optional<Direction> {
	for (auto && cell : organism.body().getCells()) {
		if (cell.bodyPart() != BodyPart::EYE) continue;
		auto && eye = cell;

		auto eyeDirection = organism.rotation; //Direction(eye.data()).rotate(organism.rotation);
		auto eyePos = organism.absoluteXY(eye);

		for (auto i = 1; i <= settings.sightRange; ++i) {
			auto seeX = eyePos.x + i * eyeDirection.x();
			auto seeY = eyePos.y + i * eyeDirection.y();

			if (!organismGrid.inBounds(seeX, seeY)) break;

			auto && space = organismGrid.access(seeX, seeY);

			if (space.fromEnvironment()) {
				return std::make_optional<Direction>(eyeDirection);

			} else if (space.fromOrganism() && space.index() != index) {
				for (auto && reaction : organism.phenome.reactions) {
					if (space.cell(organisms).bodyPart() == reaction.seeing) {
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
	for (auto index = 0; index < organisms.size(); ++index) {
		auto && organism = organisms[index];
		auto moveTries = organism.phenome.moveTries;
		if (moveTries == 0 || (organism.energy == 0 && settings.needEnergyToMove)) continue;

		auto seeingDirection = organismSeeingDirection(organism, index);

		auto tryMovingDirection = seeingDirection.has_value() ? seeingDirection.value() : organism.rotation;

		auto tryMove = [&](i32 deltaX, i32 deltaY, i32 deltaRotation) -> bool {
			if (organismGrid.canMoveOrganism(organism, index, deltaX, deltaY, deltaRotation)) {
				organismGrid.moveOrganism(organism, index, deltaX, deltaY, deltaRotation);
				return true;
			}
			return false;
		};

		auto doMove = [&]() -> bool {
			if (organism.ticksStuck >= 5) {
				for (auto i = 0; i < moveTries; ++i) {
					auto deltaRotation = std::uniform_int_distribution<i32>(1, 7)(random);
					auto deltaX = std::uniform_int_distribution<i32>(-1, 1)(random);
					auto deltaY = std::uniform_int_distribution<i32>(-1, 1)(random);

					if (tryMove(deltaX, deltaY, deltaRotation)) return true;
				}
				return false;
			}

			if (organism.ticksSinceCollision < organism.phenome.moveLength) {
				if (tryMove(tryMovingDirection.x(), tryMovingDirection.y(), 0)) return true;
			}

			organism.ticksSinceCollision = 0;

			auto baseDeltaRotation = std::uniform_int_distribution<i32>(0, 4)(random);
			for (auto i = 0; i < 5; ++i) {
				auto deltaRotation = ((baseDeltaRotation + i) % 5) - 2;
				auto newDirection = organism.rotation.rotate(deltaRotation);

				if (tryMove(
					newDirection.x(),
					newDirection.y(),
					deltaRotation
				)) return true;
			}

			return false;
		};

		if (doMove()) {
			++organism.ticksSinceCollision;
			organism.ticksStuck = 0;

			organism.addEnergy(-settings.moveCost, settings);
		} else {
			if (++organism.ticksStuck >= settings.crushTime) {
				organism.addEnergy(-settings.moveCost, settings);
			}
		}
	}
}

constexpr i32 HEAL_STRIDE = 6;

auto SimulationController::checkOrganismsDie(Tree::Node *& activeNode) -> void {
	std::erase_if(organisms, [&, this](Organism & organism) {
		if (organism.phenome.numAliveCells == 0) {
			replaceOrganismWithFood(organism);
			ids.removeId(organism.id);
			tree.kill(organism.node, activeNode);
			return true;
		}
		return false;
	});
}

auto SimulationController::replaceOrganismWithFood(Organism & organism) -> void {
	for (auto && cell : organism.body().getCells()) {
		auto && [x, y] = organism.absoluteXY(cell);
		environment.accessUnsafe(x, y).food = cell;
	}
}

auto SimulationController::serialize(Controls & controls) -> json {
	auto organismsArray = json::array();
	for (auto && organism : organisms)
		organismsArray.push_back(organism.id);

	auto messageBody = json {
		{ "width",     environment.getWidth() },
		{ "height",    environment.getHeight() },
		{ "tick",      currentTick },
		{ "organisms", std::move(organismsArray) },
	};

	if (controls.displayMode == Controls::DisplayMode::ENVIRONMENT) {
		messageBody.push_back({ "grid", Util::base64Encode(Renderer::render(environment, organisms, controls.activeNode)) });
	} else {
		messageBody.push_back({ "tree", tree.serialize(controls.activeNode) });
	}

	return messageBody;
}

static auto fiftyFifty = std::uniform_int_distribution(0, 1);

auto SimulationController::ageOrganismCells() -> void {
	for (auto && organism : organisms) {
		/* cells age with a half life */
		if (fiftyFifty(random) == 0) continue;

		/* select one cell to age */
		auto numCells = organism.body().getCells().size();
		auto startIndex = std::uniform_int_distribution(0, (i32)numCells - 1)(random);

		for (auto i = 0; i < numCells; ++i) {
			auto index = (i + startIndex) % numCells;
			auto && cell = organism.body().getCells()[index];

			if (cell.dead()) continue;

			auto newAge = cell.age() + 1;
			cell.setAge(newAge);

			if (newAge >= settings.lifetimeFactor) {
				Weapon::killCell(organism, cell);
			}
			break;
		}
	}
}

auto SimulationController::organismCellsTick() -> void {
	for (auto index = 0; index < organisms.size(); ++index) {
		auto && organism = organisms[index];

		for (auto && cell : organism.body().getCells()) {
			auto [x, y] = organism.absoluteXY(cell);

			if (cell.dead()) continue;

			if (cell.bodyPart() == BodyPart::PHOTOSYNTHESIZER) {
				Photosynthesizer::tick(x, y, organism, environment, organismGrid, settings, organisms, random);

			} else if (cell.bodyPart() == BodyPart::MOUTH) {
				Mouth::tick(x, y, organism, index, environment, organismGrid, organisms, settings);

			} else if (cell.bodyPart() == BodyPart::WEAPON) {
				Weapon::tick(x, y, index, cell, environment, organismGrid, organisms, settings);
			}
		}
	}
}

auto SimulationController::organismsReproduce() -> void {
	auto newOrganisms = std::vector<Organism>();

    for (auto && organism : organisms) {
		if (!organism.storedChild.has_value()) {
			auto && phenome = organism.phenome;
			auto bodyEnergy = phenome.baseBodyEnergy;

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

			auto childBodyEnergy = childPhenome.baseBodyEnergy;

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

auto SimulationController::findChildSpawnPoint(Organism & organism, Phenome & childPhenome) -> std::optional<ChildSpawnPoint> {
	auto rotation = organism.rotation;

	auto left = organism.body().getLeft(rotation);
	auto right = organism.body().getRight(rotation);
	auto up = organism.body().getUp(rotation);
	auto down = organism.body().getDown(rotation);

	auto findSpawnTries = (std::uniform_int_distribution<i32>(0, 2)(random) / 2) + organism.phenome.moveTries * 2;

	for (auto tri = 0; tri < findSpawnTries; ++tri) {
		auto childRotation = Direction(randomDirection(random));

		auto childLeft = childPhenome.body.getLeft(childRotation);
		auto childRight = childPhenome.body.getRight(childRotation);
		auto childUp = childPhenome.body.getUp(childRotation);
		auto childDown = childPhenome.body.getDown(childRotation);

		auto xRange = std::uniform_int_distribution<i32>(left - 1 - childRight, right + 1 - childLeft);
		auto yRange = std::uniform_int_distribution<i32>(down - 1 - childUp, up + 1 - childDown);

		auto spawnX = organism.x + xRange(random);
		auto spawnY = organism.y + yRange(random);

		if (organismGrid.isSpaceAvailable(childPhenome.body, spawnX, spawnY, childRotation))
			return std::make_optional(ChildSpawnPoint { spawnX, spawnY, childRotation });
	}

	return std::nullopt;
}

auto SimulationController::tryReproduce(Phenome & childPhenome, Organism & organism, i32 reproductionEnergy, i32 childBodyEnergy, i32 childEnergy) -> std::optional<Organism> {
	auto spawnPoint = findChildSpawnPoint(organism, childPhenome);
	if (!spawnPoint.has_value()) return std::nullopt;

	auto [x, y, rotation] = spawnPoint.value();

	organism.addEnergy(-(reproductionEnergy + childBodyEnergy + childEnergy), settings);

	return std::make_optional<Organism>(
		std::move(childPhenome),
        ids.newId(random),
        x, y,
        rotation,
        childEnergy,
		tree.add(organism.node, childPhenome.genome)
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
	settings.factors[Factor::LIGHT].tick();

	refreshFactors();
}

auto SimulationController::refreshFactors() -> void {
	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto && cell = environment.accessUnsafe(x, y);

			cell.setFactor(Factor::LIGHT, settings.factors[Factor::LIGHT].getValue(x, y));
		}
	}
}
