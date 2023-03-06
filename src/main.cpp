//
// Created by Emmet on 11/9/2022.
//

#include "genome/phenome.h"
#include "genome/genome.h"
#include "types.h"

#include <thread>
#include <iostream>

#include "environment/simulationController.h"
#include "messageCreator.h"
#include "messageReceiver.h"
#include "socket.h"
#include "environment/organismSeeder.h"
#include "loop.h"
#include "genome/initialGenome.h"

auto main () -> int {
	auto controls = Controls { .playing=false, .fps=20, .updateDisplay=true };

	auto simulationController = SimulationController(Environment(150, 72));

	auto initialPhenome = Phenome(InitialGenome::create(), Body(2, BodyPart::MOUTH));

	OrganismSeeder::insertInitialOrganisms(simulationController.organisms, simulationController.environment, initialPhenome, 15);

	auto simulationMutex = std::mutex();

	auto socket = Socket();

	socket.init("51679", [&](const std::vector<char> & message) -> void {
		auto messageString = std::string(message.begin(), message.end());

		auto parsedMessage = MessageReceiver::receive(messageString);
		if (!parsedMessage.has_value()) return;

		if (parsedMessage->type == "init") {
			auto lock = std::unique_lock(simulationMutex);

			auto json = MessageCreator::initMessage(
				simulationController.serialize(),
				controls.serialize()
			).dump();

			socket.send(json.begin(), json.end());

		} else if (parsedMessage->type == "control") {
			if (!parsedMessage->body.contains("control")) return;

			auto lock = std::unique_lock(simulationMutex);
			controls.updateFromSerialized(parsedMessage->body["control"]);

			auto json = MessageCreator::controlsMessage(controls.serialize()).dump();

			socket.send(json.begin(), json.end());

		} else {
			std::cout << "unknown message of type" << parsedMessage->type << std::endl;
		}
	});

	std::thread([&]() {
		/* don't send more than 25 fps */
		auto minSendTime = Loop::resolution((u64)((1._f64 / 25._f64) * Loop::resolution(Loop::seconds(1)).count()));

		auto lastSendTime = Loop::timePoint();

		auto loop = Loop();

		loop.enter([&](Loop::timePoint now) -> Fps {
			auto lock = std::unique_lock(simulationMutex);

			if (controls.playing) {
				simulationController.step();
			}

			if (socket.isConnected() && controls.updateDisplay && controls.playing && (now - lastSendTime) >= minSendTime) {
				auto stateJson = MessageCreator::frameMessage(simulationController.serialize());

				auto jsonData = stateJson.dump();
				socket.send(jsonData.begin(), jsonData.end());

				lastSendTime = now;
			}

			return controls.unlimitedFPS() ? Fps::unlimited() : Fps(controls.fps);
		});
	}).join();
}
