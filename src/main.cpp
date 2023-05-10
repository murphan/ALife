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
#include "ids.h"
#include "priorityMutex.h"

constexpr auto WIDTH = 250, HEIGHT = 150;

auto createSimulation(Settings & settings, Controls & controls, i32 width, i32 height) -> SimulationController {
	controls.playing = false;
	controls.fps = 20;
	controls.updateDisplay = true;
	controls.activeNode = nullptr;
	controls.displayMode = Controls::DisplayMode::ENVIRONMENT;
	controls.smartTree = false;

	return SimulationController(
		settings,
		Environment(width, height),
		OrganismGrid(width, height),
		std::default_random_engine(std::random_device()()),
		Ids(),
		Tree()
	);
}

auto initSimulation(Settings & settings, Controls & controls, SimulationController & simulationController) -> void {
	simulationController.refreshFactors();

	OrganismSeeder::insertInitialOrganisms(
		simulationController.organisms,
		simulationController.environment,
		simulationController.random,
		simulationController.ids,
		simulationController.tree,
		Phenome(InitialGenome::create(), Body(2), settings),
		settings,
		1
	);

	simulationController.tree.update(controls.smartTree, true, controls.activeNode);
}

auto main () -> int {
	auto settings = Settings {
		.lifetimeFactor = 100,
		.photosynthesisFactor = 1,
		.startingEnergy = 31,
		.reproductionCost = 15,
		.reproductionThreshold = 21,
		.foodEfficiency = 0.9_f32,
		.baseMutationRates = {
			0.005,
			0.005,
			0.005,
		},
		.mutationFactor = 1.5,
		.sightRange = 8,
		.weaponDamage = 32,
		.armorPrevents = 16,
		.moveCost = 1,
		.baseMoveLength = 16,
		.bodyPartCosts = {
			16, /* MOUTH */
			16, /* MOVER */
			16, /* PHOTOSYNTHESIZER */
			16, /* WEAPON */
			16, /* ARMOR */
			8, /* EYE */
			2, /* SCAFFOLD */
		},
		.upgradedPartCosts = {
			0, /* MOUTH */
			0, /* MOVER */
			0, /* PHOTOSYNTHESIZER */
			4, /* WEAPON */
			4, /* ARMOR */
			0, /* EYE */
			2, /* SCAFFOLD */
		},
		.factorNoises = {
			Noise(Factor::TEMPERATURE, false, -1.0_f32, 0.01_f32, 100.0_f32, 1.0_f32),
			Noise(Factor::LIGHT, false, 0.35_f32, 0.0_f32, 50.0_f32, 1.0_f32),
			Noise(Factor::OXYGEN, false, -1.0_f32, 0.01_f32, 100.0_f32, 1.0_f32),
		}
	};
	auto controls = Controls {};

	auto simulationController = createSimulation(settings, controls, WIDTH, HEIGHT);
	initSimulation(settings, controls, simulationController);

	auto mutex = PriorityMutex();
	auto socket = Socket();

	socket.init("51679", [&](const std::vector<char> & message) -> void {
		auto messageString = std::string(message.begin(), message.end());

		auto messageResult = MessageReceiver::receive(messageString);
		if (!messageResult.has_value()) return;
		auto && parsedMessage = messageResult.value();

		if (parsedMessage.type == "init") {
			std::string json;

			mutex.highPriorityLock([&]() {
				json = MessageCreator::initMessage(
					simulationController.serialize(controls),
					controls.serialize(),
					settings.serialize()
				).dump();
			});

			socket.send(json.begin(), json.end());

		} else if (parsedMessage.type == "controls") {
			if (!parsedMessage.body.contains("controls")) return;

			std::string json;

			mutex.highPriorityLock([&]() {
				auto displayModeBefore = controls.displayMode;

				controls.updateFromSerialized(parsedMessage.body["controls"], simulationController.tree);

				if (displayModeBefore != controls.displayMode) {
					simulationController.tree.update(controls.smartTree, controls.displayMode == Controls::DisplayMode::TREE, controls.activeNode);
					json = MessageCreator::controlsMessageAndFrame(controls.serialize(), simulationController.serialize(controls)).dump();
				} else {
					json = MessageCreator::controlsMessage(controls.serialize()).dump();
				}
			});

			socket.send(json.begin(), json.end());

		} else if (parsedMessage.type == "request") {
			if (!parsedMessage.body.contains("id")) return;
			auto && idField = parsedMessage.body["id"];
			if (!idField.is_number()) return;

			auto id = idField.get<i32>();

			auto && organism = simulationController.getOrganism(id);
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
		} else if  (parsedMessage.type == "reset") {
			std::string json;

			mutex.highPriorityLock([&]() {
				simulationController = std::move(createSimulation(settings, controls, WIDTH, HEIGHT));
				initSimulation(settings, controls, simulationController);

				json = MessageCreator::initMessage(
					simulationController.serialize(controls),
					controls.serialize(),
					settings.serialize()
				).dump();
			});

			socket.send(json.begin(), json.end());

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
		std::string jsonData;

		mutex.lowPriorityLock([&]() {
			if (controls.playing) {
				simulationController.tick(controls.activeNode);
			}

			if (
				socket.isConnected() &&
				controls.updateDisplay &&
				controls.playing &&
				(now - lastSendTime) >= minSendTime
			) {
				simulationController.tree.update(
					controls.smartTree,
					controls.displayMode == Controls::DisplayMode::TREE,
					controls.activeNode
				);

				jsonData = MessageCreator::frameMessage(simulationController.serialize(controls)).dump();

				lastSendTime = now;
			}
		});

		if (!jsonData.empty()) {
			socket.send(jsonData.begin(), jsonData.end());
		}

		return controls.unlimitedFPS() ? Fps::unlimited() : Fps(controls.fps);
	});
}
