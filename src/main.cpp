//
// Created by Emmet on 11/9/2022.
//

#include "genome/phenome.h"
#include "genome/genome.h"
#include "types.h"

#include <tuple>
#include <thread>
#include <iostream>

#include "genome/gene/bodyGene.h"
#include "environment/simulationController.h"
#include "messageCreator.h"
#include "messageReceiver.h"
#include "socket.h"
#include "environment/organismSeeder.h"

auto main () -> int {
	Socket::init("51679");

	auto controls = Controls { .playing=false, .fps=60, .updateDisplay=true };

	auto simulationController = SimulationController(Environment(150, 72));

	auto baseGenome = Genome();

	BodyGene::create(Direction::RIGHT, BodyPart::BASIC).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, BodyPart::BASIC).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::createUseAnchor(Direction::UP, BodyPart::UNKNOWN03, 0).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::UP, BodyPart::UNKNOWN03).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT_UP, BodyPart::UNKNOWN03).write(baseGenome);
	baseGenome.writeGarbage(4, Genome::A);

	auto initialPhenome = Phenome(std::move(baseGenome), Body(2, BodyPart::MOUTH));

	OrganismSeeder::insertInitialOrganisms(simulationController.organisms, simulationController.environment, initialPhenome, 15);

	while (true) {
		while (true) {
			auto message = Socket::queueMessage();
			if (!message.has_value()) break;

			auto messageString = std::string(message.value().begin(), message.value().end());

			auto parsedMessage = MessageReceiver::receive(messageString);
			if (!parsedMessage.has_value()) continue;

			if (parsedMessage->type == "info") {
				auto infoJson = MessageCreator::initMessage(simulationController.serialize(),
				                                            controls.serialize()).dump();
				Socket::send(infoJson.begin(), infoJson.end());

			} else if (parsedMessage->type == "controls") {
				if (!parsedMessage->body.contains("controls")) continue;

				controls.updateFromSerialized(parsedMessage->body["controls"]);

				auto response = MessageCreator::controlsMessage(controls.serialize()).dump();
				Socket::send(response.begin(), response.end());

			} else {
				std::cout << "unknown message of type" << parsedMessage->type << std::endl;
			}
		}

		simulationController.step();

		if (Socket::isConnected()) {
			auto stateJson = MessageCreator::frameMessage(simulationController.serialize());

			auto jsonData = stateJson.dump();
			Socket::send(jsonData.begin(), jsonData.end());
		}

		std::this_thread::sleep_for(std::chrono::milliseconds (100));
	}
}
