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

	auto simulationController = SimulationController(Environment(20, 9));

	simulationController.organisms.emplace_back();
	simulationController.organisms.emplace_back();

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

	Socket::close();

    auto g0 = Genome("AAACAAADAAABAAACAAAD");
    auto g1 = Genome("AAADAAABAAABAAACAAAC");

	for (auto i = 0; i < 20; ++i) {
		auto recombined = Genome::mutateCombine(g0, g1, 0.3_f32, 0.01_f32, 0.01_f32, 0.01_f32);
		std::cout << "Combined: " << (recombined.has_value() ? recombined->toString() : "Not compatible") << std::endl;
	}

    //auto [ r0, r1 ] = std::tuple<Genome &, Genome &> { g0, g1 };
    //auto grid = Genome::editDistance(g0, g1);
    //Util::printGrid(grid, g0.size() + 1);

    /* mutate genome test */
    auto ogGenome = Genome(std::string("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
    auto mutatedGenome = ogGenome.mutateCopy(0.01_f32, 0.01_f32, 0.01_f32);

    std::cout << ogGenome.toString() << std::endl;
    std::cout << mutatedGenome.toString() << '\n' << std::endl;

    /* create body test */
	auto creator = Genome();

	BodyGene::create(Direction::RIGHT, 2).write(creator);
	creator.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, 3).write(creator);
	creator.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, 4).write(creator);
	creator.writeGarbage(4, Genome::A);
	BodyGene::create(Direction::RIGHT, 5).write(creator);
	creator.writeGarbage(4, Genome::A);

	std::cout << "created: " << creator.toString() << std::endl;

    auto phenome = Phenome(creator);
    std::cout << phenome.body.debugToString() << std::endl;
}
