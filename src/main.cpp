//
// Created by Emmet on 11/9/2022.
//

#include <iostream>
#include "genome/phenome.h"

#include "genome/genome.h"
#include "types.h"
#include "util.h"
#include <tuple>
#include <chrono>
#include <thread>

#include <genome/gene/bodyGene.h>
#include "environment/simulationController.h"
#include "stateSerializer.h"

#include "socket.h"

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

	auto baseOrganism = Organism(
		Phenome(std::move(baseGenome), Body(2, BodyPart::MOUTH)),
		75,
		36
	);

	simulationController.organisms.push_back(std::move(baseOrganism));

	while (true) {
		while (true) {
			auto message = Socket::queueMessage();
			if (!message.has_value()) break;

			auto str = std::string(message.value().begin(), message.value().end());
			std::cout << "received message" << std::endl;
			std::cout << str << std::endl;
		}

		simulationController.step();

		if (Socket::isConnected()) {
			auto stateJson = StateSerializer::serialize(simulationController.currentStep, simulationController.environment, simulationController.organisms);

			std::cout << stateJson << std::endl;
			auto jsonData = stateJson.dump();
			Socket::send(jsonData.begin(), jsonData.end());
		}

		std::this_thread::sleep_for(std::chrono::milliseconds (1000));
	}
}
