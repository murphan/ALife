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
		organismsArray.push_back(json {
			{ "id", "TEST" },
			{ "body", "TEST" },
			{ "width", 9 },
			{ "height", 9 },
			{ "centerX", 2 },
			{ "centerY", 2 },
			{ "rotation", 1 },
			{ "x", 4 },
			{ "y", 4 },
			{ "energy", 34 },
			{ "age", 128 },
		});
	}

	return json {
		{ "width", environment.getWidth() },
		{ "height", environment.getHeight() },
		{ "tick", tick },
		{ "grid", Util::base64Encode(byteEncodedGrid) },
		{ "organisms", organismsArray },
	};
}
