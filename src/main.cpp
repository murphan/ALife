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
	auto settings = Settings { .ageFactor = 1000, .factorNoises = {
		Noise(0,true, 0.0_f32, 0.01_f32, 100.0_f32, 1.0_f32),
		Noise(1,true, 0.0_f32, 0.1_f32, 50.0_f32, 1.0_f32),
		Noise(2,true, 0.0_f32, 0.2_f32, 35.0_f32, 1.0_f32),
	} };

	auto simulationController = SimulationController(Environment(150, 72));

	auto initialPhenome = Phenome(InitialGenome::create(), Body(2, BodyPart::MOUTH));

	OrganismSeeder::insertInitialOrganisms(simulationController.organisms, simulationController.environment, initialPhenome, 35);

	simulationController.scatterFood(Food::FOOD0, 50, 1);

	auto simulationMutex = std::mutex();

	auto socket = Socket();

	socket.init("51679", [&](const std::vector<char> & message) -> void {
		auto messageString = std::string(message.begin(), message.end());

		auto messageResult = MessageReceiver::receive(messageString);
		if (!messageResult.has_value()) return;
		auto && parsedMessage = messageResult.value();

		if (parsedMessage.type == "init") {
			auto lock = std::unique_lock(simulationMutex);

			auto json = MessageCreator::initMessage(
				simulationController.serialize(),
				controls.serialize(),
				settings.serialize()
			).dump();

			socket.send(json.begin(), json.end());

		} else if (parsedMessage.type == "control") {
			if (!parsedMessage.body.contains("control")) return;

			auto lock = std::unique_lock(simulationMutex);
			controls.updateFromSerialized(parsedMessage.body["control"]);

			auto json = MessageCreator::controlsMessage(controls.serialize()).dump();

			socket.send(json.begin(), json.end());

		} else if (parsedMessage.type == "request") {
			if (!parsedMessage.body.contains("id")) return;
			auto id = parsedMessage.body["id"];
			if (!id.is_string()) return;

			auto idString = id.get<std::string>();
			auto uuid = UUID::fromString(idString);
			if (!uuid.has_value()) return;

			auto * organism = simulationController.getOrganism(uuid.value());
			if (organism == nullptr) {
				auto json = MessageCreator::emptyOrganismRequestMessage().dump();
				socket.send(json.begin(), json.end());
				return;
			}

			auto json = MessageCreator::organismRequestMessage(organism->serialize(true)).dump();
			socket.send(json.begin(), json.end());

		} else if (parsedMessage.type == "settings") {
			try {
				settings.handleSettingsMessage(parsedMessage.body);

				auto json = MessageCreator::settingsMessage(settings.serialize()).dump();
				socket.send(json.begin(), json.end());

			} catch (...) {
				std::cout << "bad settings message" << std::endl;
			}
		} else {
			std::cout << "unknown message of type" << parsedMessage.type << std::endl;
			std::cout << parsedMessage.body << std::endl;
		}
	});

	/* don't send more than 25 fps */
	auto minSendTime = Loop::resolution((u64)((1._f64 / 25._f64) * Loop::resolution(Loop::seconds(1)).count()));

	auto lastSendTime = Loop::timePoint();

	auto loop = Loop();

	loop.enter([&](Loop::timePoint now) -> Fps {
		auto lock = std::unique_lock(simulationMutex);

		if (controls.playing) {
			simulationController.tick(settings);
		}

		if (socket.isConnected() && controls.updateDisplay && controls.playing && (now - lastSendTime) >= minSendTime) {
			auto stateJson = MessageCreator::frameMessage(simulationController.serialize());

			auto jsonData = stateJson.dump();
			socket.send(jsonData.begin(), jsonData.end());

			lastSendTime = now;
		}

		return controls.unlimitedFPS() ? Fps::unlimited() : Fps(controls.fps);
	});
}
