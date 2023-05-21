//
// Created by Emmet on 5/20/2023.
//

module;

#include "json.hpp"

export module SerializeOrganism;

import Organism;
import Body;
import Direction;
import Phenome;
import Controls;
import Util;
import Renderer;
import RenderBackground;
import EyeGene;
import BodyPart;

using json = nlohmann::json;

namespace SerializeOrganism {
	export auto serialize(const Organism & organism, bool detailed) -> json {
		auto && body =  organism.phenome.body;

		auto nonDetailedPart = json {
			{ "id", organism.id },
			{ "rotation",  organism.rotation.value() },
			{ "x", organism.x },
			{ "y", organism.y },
			{ "energy",  organism.energy },
			{ "numAliveCells", organism.phenome.numAliveCells },
			{ "baseBodyEnergy", organism.phenome.baseBodyEnergy },
		};

		if (!detailed) return nonDetailedPart;

		nonDetailedPart.push_back({ "genome",  organism.phenome.genome.toString() });

		auto render = Renderer::Render(
			RenderBackground(body.getWidth(), body.getHeight()),
			Controls { .activeNode = organism.node }
		);

		render.renderOrganism(
			organism,
			-body.getLeft(Direction::RIGHT),
			-body.getDown(Direction::RIGHT),
			Direction::RIGHT,
			0
		);
		nonDetailedPart.push_back({ "body", Util::base64Encode(render.finalize()) });

		auto mutationModifiers = json::array();
		for (auto && mutationModifier : organism.phenome.mutationModifiers) mutationModifiers.push_back(mutationModifier);
		nonDetailedPart.push_back({ "mutationModifiers", mutationModifiers });

		nonDetailedPart.push_back({ "moveTries", organism.phenome.moveTries });

		nonDetailedPart.push_back({ "moveLength", organism.phenome.moveLength });

		auto reactions = json::array();
		for (auto && reaction : organism.phenome.reactions) reactions.push_back(json {
				{ "seeing", BODY_PART_NAMES[reaction.seeing] },
				{ "action", EyeGene::ACTION_NAMES[reaction.actionType] },
			});
		nonDetailedPart.push_back({ "reactions", reactions });

		auto && completeParts = nonDetailedPart;
		return completeParts;
	}
}
