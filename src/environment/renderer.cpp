//
// Created by Emmet on 4/11/2023.
//

#include "renderer.h"

constexpr static i32 BYTES_PER_TILE = 9;

inline auto minMax(i32 value, u32 min, u32 max) -> u32 {
	return value < min ? min : value > max ? max : value;
}

inline auto rgbToColor(f32 red, f32 green, f32 blue) -> u32 {
	return (minMax((i32)(std::max(red, 0._f32) * 255), 0, 255) << 16) |
		(minMax((i32)(std::max(green, 0._f32) * 255), 0, 255) << 8) |
		minMax((i32)(std::max(blue, 0._f32) * 255), 0, 255);
}

constexpr static f32 BACKGROUND_DIM = 0.3333333333_f32;

inline auto getFactorsColor(MapCell & mapCell) -> u32 {
	return rgbToColor(
		((f32)mapCell.getFactor(Factor::TEMPERATURE) / 127.0_f32) * BACKGROUND_DIM,
		((f32)mapCell.getFactor(Factor::LIGHT) / 127.0_f32) * BACKGROUND_DIM,
		((f32)mapCell.getFactor(Factor::OXYGEN) / 127.0_f32) * BACKGROUND_DIM
	);
}

constexpr u32 bodyPartColors[] = {
	0xf2960c, /* Mouth */
	0x11f0e8, /* Mover */
	0x086603, /* Photosynthesizer */
	0xe31045, /* Weapon */
	0x4e2ba6, /* Armor */
	0x74c2e3, /* Eye */
};

constexpr u32 foodColors[] = {
	0x8d9400,
	0x945900,
	0x940000,
	0x940059,
};

inline auto insert3(std::vector<u8> & buffer, i32 index, u32 value) -> void {
	buffer[index] = (value >> 16) & 0xff;
	buffer[index + 1] = (value >> 8) & 0xff;
	buffer[index + 2] = value & 0xff;
}

inline auto insert2(std::vector<u8> & buffer, i32 index, u32 value) -> void {
	buffer[index] = (value >> 8) & 0xff;
	buffer[index + 1] = value & 0xff;
}

constexpr static u8 META_EMPTY = 0;
constexpr static u8 META_ORGANISM = 1;
constexpr static u8 META_FOOD = 2;
constexpr static u8 META_WALL = 3;

constexpr static u8 CIRCLE_FLAG = 1 << 7;

auto Renderer::render(Environment & environment, std::vector<Organism> & organisms) -> std::vector<u8> {
	auto buffer = std::vector<u8>(environment.mapSize() * BYTES_PER_TILE);

	auto bufferIndex = [&](i32 x, i32 y) {
		return (y * environment.getWidth() + x) * BYTES_PER_TILE;
	};

	for (auto j = 0; j < environment.getHeight(); ++j) {
		for (auto i = 0; i < environment.getWidth(); ++i) {
			auto && cell = environment.getCell(i, j);
			if (cell.getHasFood()) {
				buffer[bufferIndex(i, j)] = META_FOOD | CIRCLE_FLAG;
				insert3(
					buffer,
					bufferIndex(i, j) + 6,
					foodColors[cell.getFood().getType()]
				);
			}
			insert3(
				buffer,
				bufferIndex(i, j) + 3,
				getFactorsColor(environment.getCell(i, j))
			);
		}
	}

	for (auto index = 0; index < organisms.size(); ++index) {
		auto && organism = organisms[index];
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto cell = body.access(i, j, rotation).bodyPart();

				auto x = organism.x + i, y = organism.y + j;

				if (cell != BodyPart::NONE) {
					buffer[bufferIndex(x, y)] = META_ORGANISM;
					insert2(
						buffer,
						bufferIndex(x, y) + 1,
						index
					);
					insert3(
						buffer,
						bufferIndex(x, y) + 3,
						bodyPartColors[cell - 1]
					);
				}
			}
		}
	}

	return buffer;
}
