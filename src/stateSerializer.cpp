//
// Created by Emmet on 2/3/2023.
//

#include "stateSerializer.h"

#include "util.h"

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

auto StateSerializer::serialize(i32 tick, Environment & environment, std::vector<Organism> & organisms) -> json {
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
		{ "type", "environment_frame" },
		{ "width", environment.getWidth() },
		{ "height", environment.getHeight() },
		{ "tick", tick },
		{ "grid", Util::base64Encode(byteEncodedGrid) },
		{ "organisms", organismsArray },
	};
}
