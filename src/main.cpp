//
// Created by Emmet on 11/9/2022.
//

#include "genome/phenome.h"
#include "genome/genome.h"
#include "types.h"

#include <tuple>
#include <thread>

#include "genome/gene/bodyGene.h"
#include "environment/simulationController.h"
#include "stateSerializer.h"
#include "socket.h"
#include "environment/organismSeeder.h"

auto main () -> int {
	Socket::init("51679");

	auto simulationController = SimulationController(Environment(150, 72));

	auto baseGenome = Genome();

	BodyGene::create(Direction::RIGHT, BodyPart::BASIC).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::MOVER).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::BASIC).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::EYE).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);

	auto initialPhenome = Phenome(std::move(baseGenome), Body(2, BodyPart::MOUTH));

	OrganismSeeder::insertInitialOrganisms(simulationController.organisms, simulationController.environment, initialPhenome, 13);

	while (true) {
		while (true) {
			auto message = Socket::queueMessage();
			if (!message.has_value()) break;

			auto str = std::string(message.value().begin(), message.value().end());
			#ifdef DEBUG
			std::cout << "received message" << std::endl;
			std::cout << str << std::endl;
			#endif
		}

		simulationController.step();

		if (Socket::isConnected()) {
			auto stateJson = StateSerializer::serialize(simulationController.currentStep, simulationController.environment, simulationController.organisms);

			#ifdef DEBUG
			std::cout << stateJson << std::endl;
			#endif
			auto jsonData = stateJson.dump();
			Socket::send(jsonData.begin(), jsonData.end());
		}

		std::this_thread::sleep_for(std::chrono::milliseconds (1000));
	}
}
