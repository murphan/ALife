
#include "environment/simulationController.h"

SimulationController::SimulationController(Environment && environment) :
	random(std::random_device()()),
	organismGrid(environment.getWidth(), environment.getHeight()),
	environment(std::move(environment)),
	organisms(),
	currentTick(0) {}

auto SimulationController::tempId(i32 index) -> i32 {
	return index + 1;
}

auto SimulationController::tick() -> void {
	std::shuffle(organisms.begin(), organisms.end(), random);

    for (auto && organism : organisms) {
        organism.tick();
    }

	moveOrganisms();

	organismsAgeAndDie();

    organismsEat();

    organismsReproduce();

	++currentTick;
}

auto symmetricRange = std::uniform_int_distribution<i32>(-1, 1);

auto SimulationController::moveOrganisms() -> void {
	organismGrid.clear();

	for (auto i = 0; i < organisms.size(); ++i) {
		organismGrid.placeOrganism(organisms.at(i), tempId(i));
	}

	for (auto i = 0; i < organisms.size(); ++i) {
		auto deltaX = symmetricRange(random);
		auto deltaY = symmetricRange(random);
		auto deltaRotation = symmetricRange(random);

		auto && organism = organisms.at(i);
		auto id = tempId(i);

		if (organismGrid.canMoveOrganism(organism, id, deltaX, deltaY, deltaRotation)) {
			organismGrid.moveOrganism(organism, id, deltaX, deltaY, deltaRotation);
		}
	}
}

auto SimulationController::organismsAgeAndDie() -> void {
	std::erase_if(organisms, [this](auto && organism) {
		auto newAge = organism.age;

		//TODO put in settings
		constexpr auto AGE_FACTOR = 1600;

		if (newAge > organism.getPhenome().maxAge(AGE_FACTOR)) {
			replaceOrganismWithFood(organism);
			return true;
		} else {
			return false;
		}
	});
}

auto SimulationController::replaceOrganismWithFood(const Organism & organism) -> void {
	auto && body = organism.body();
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j, x = organism.x + i;
			auto cell = body.access(i, j, rotation);

			if (cell != BodyPart::NONE) {
				environment.getCell(x, y).setFood(Food(Food::FOOD0, 1));
			}
		}
	}
}

template<typename T>
concept Is32Bit = requires(T a) {
	sizeof(T) == 4;
};

template<Is32Bit T>
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
			byteEncodedGrid.push_back(cell.getFactor((Factor)3));

			if (cell.getHasFood()) {
				auto & food = cell.getFood();

				byteEncodedGrid.push_back(food.getType());
				byteEncodedGrid.push_back((char)food.getEnergy());
				push32Bit(byteEncodedGrid, food.getAge());

			} else {
				byteEncodedGrid.push_back(cell.getHasWall() ? -1 : -2);
				byteEncodedGrid.push_back(0);
				push32Bit(byteEncodedGrid, 0);
			}

			byteEncodedGrid.push_back(0);
		}
	}

	auto organismsArray = json::array();
	for (auto & organism : organisms) {
		auto & body = organism.body();
		auto rotation = organism.rotation;

		auto byteEncodedBody = std::string();
		byteEncodedBody.reserve(body.getWidth() * body.getHeight());

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				byteEncodedBody.push_back((char)body.access(i, j, rotation));
			}
		}

		organismsArray.push_back(json {
			{ "id", organism.uuid.asString() },
			{ "body", Util::base64Encode(byteEncodedBody) },
			{ "left", body.getLeft(rotation) },
			{ "right", body.getRight(rotation) },
			{ "down", body.getDown(rotation) },
			{ "up", body.getUp(rotation) },
			{ "rotation", organism.rotation.value() },
			{ "x", organism.x },
			{ "y", organism.y },
			{ "energy", organism.energy },
			{ "age", organism.age },
		});
	}

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
        auto y = std::uniform_int_distribution(0, organismGrid.getHight() - 1)(random);

        if (!organismGrid.organismInSpace(x, y))
            addFood(x, y, type, energyDefault);
    }
}

/**
 *
 * For each Organism checks each body part looking for overlapping food cells
 * If found, eats food by adding the energy to the organism and destroying the food
 *
 */
auto SimulationController::organismsEat() -> void {
	for (auto && organism : organisms) {
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto y = organism.y + j;
				auto x = organism.x + i;

				auto && environmentCell = environment.getCell(x, y);

				if (environmentCell.getHasFood() && body.access(i, j, rotation) != BodyPart::NONE) {
					organism.eatFood(environmentCell.getFood());
					environmentCell.removeFood();
				}
			}
		}
	}
}

auto SimulationController::organismsReproduce() -> void {
    for (auto && organism : organisms) {
        if (organism.energy > organism.getPhenome().repoductionThreshold)
            addChild(organism);
    }
}

auto SimulationController::addChild(auto && organism) -> void {

    return;
}

auto SimulationController::howMuchFood() -> i32 {
    auto numFood = 0;
    for (int i = 0; i < environment.getWidth(); ++i) {
        for (int j = 0; j < environment.getHeight(); ++j) {
            auto cell = environment.getCell(i,j);
            if (cell.getHasFood())
                numFood++;
        }
    }
    return numFood;
}
