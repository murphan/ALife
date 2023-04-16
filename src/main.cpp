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
	auto random = std::default_random_engine(std::random_device()());

	auto controls = Controls { .playing=true, .fps=20, .updateDisplay=true };
	auto settings = Settings {
		.lifetimeFactor = 64,
		.energyFactor = 16,
		.photosynthesisFactor = 1,
		.startingEnergy = 4,
		.reproductionCost = 1,
		.reproductionThreshold = 2,
		.foodEfficiency = 1.0_f32,
		.maxFoodAge = 100000000,
		.baseMutationRates = {
			0.005,
			0.005,
			0.005,
		},
		.mutationFactor = 1.5,
		.factorNoises = {
			Noise(Factor::TEMPERATURE, false, -1.0_f32, 0.01_f32, 100.0_f32, 1.0_f32),
			Noise(Factor::LIGHT, false, 0.35_f32, 0.0_f32, 50.0_f32, 1.0_f32),
			Noise(Factor::OXYGEN, false, -1.0_f32, 0.01_f32, 100.0_f32, 1.0_f32),
		}
	};

	auto simulationController = SimulationController(Environment(120, 80), random);
	simulationController.refreshFactors(settings);

	auto initialPhenome = Phenome(InitialGenome::create(), Body(2));

	OrganismSeeder::insertInitialOrganisms(simulationController.organisms, simulationController.environment, initialPhenome, settings, 80, random);

	auto simulationMutex = std::mutex();
	auto lowPriorityMutex = std::mutex();
	auto nextAccessMutex = std::mutex();

	auto lowPriorityLock = [&]() {
		lowPriorityMutex.lock();
		nextAccessMutex.lock();
		simulationMutex.lock();
		nextAccessMutex.unlock();
	};

	auto lowPriorityUnlock = [&]() {
		simulationMutex.unlock();
		lowPriorityMutex.unlock();
	};

	auto highPriorityLock = [&]() {
		nextAccessMutex.lock();
		simulationMutex.lock();
		nextAccessMutex.unlock();
	};

	auto highPriorityUnlock = [&]() {
		simulationMutex.unlock();
	};

	auto socket = Socket();

	socket.init("51679", [&](const std::vector<char> & message) -> void {
		auto messageString = std::string(message.begin(), message.end());

		auto messageResult = MessageReceiver::receive(messageString);
		if (!messageResult.has_value()) return;
		auto && parsedMessage = messageResult.value();

		if (parsedMessage.type == "init") {
			highPriorityLock();

			auto json = MessageCreator::initMessage(
				simulationController.serialize(),
				controls.serialize(),
				settings.serialize()
			).dump();

			highPriorityUnlock();

			socket.send(json.begin(), json.end());

		} else if (parsedMessage.type == "control") {
			if (!parsedMessage.body.contains("control")) return;

			highPriorityLock();

			controls.updateFromSerialized(parsedMessage.body["control"]);
			auto json = MessageCreator::controlsMessage(controls.serialize()).dump();

			highPriorityUnlock();

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

	/* don't send more than 21 fps */
	auto minSendTime = Loop::resolution((u64)((1._f64 / 21._f64) * Loop::resolution(Loop::seconds(1)).count()));

	auto lastSendTime = Loop::timePoint();

	auto loop = Loop();

	loop.enter([&](Loop::timePoint now) -> Fps {
		lowPriorityLock();

		if (controls.playing) {
			simulationController.tick(settings);
		}

		if (socket.isConnected() && controls.updateDisplay && controls.playing &&
		    (now - lastSendTime) >= minSendTime) {
			auto stateJson = MessageCreator::frameMessage(simulationController.serialize());

			auto jsonData = stateJson.dump();
			socket.send(jsonData.begin(), jsonData.end());

			lastSendTime = now;
		}

		lowPriorityUnlock();

		return controls.unlimitedFPS() ? Fps::unlimited() : Fps(controls.fps);
	});
}
