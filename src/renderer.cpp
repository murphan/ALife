//
// Created by Emmet on 4/11/2023.
//

#include "renderer.h"
#include "util/color.h"

constexpr static i32 BYTES_PER_TILE = 9;

constexpr static f32 BACKGROUND_DIM = 0.3333333333_f32;

inline auto getFactorsColor(const MapCell & mapCell) -> u32 {
	auto value = ((f32)mapCell.getFactor(Factor::LIGHT) / 127.0_f32) * BACKGROUND_DIM;

	return Color::channelFloats2Int(value, value, value);
}

template<usize SIZE>
constexpr auto colorShiftArray(const u32 (&colorArray)[SIZE], f32 saturationFactor, f32 valueFactor) -> std::array<u32, SIZE> {
	auto array = std::array<u32, SIZE>();

	for (auto i = 0; i < SIZE; ++i) {
		auto hsv = Color::rgb2hsv(colorArray[i]);

		hsv.s *= saturationFactor;
		hsv.v *= valueFactor;

		array[i] = Color::hsv2rgb(hsv);
	}

	return array;
}

auto modifyColor(bool dead, bool inactive, u32 color) -> u32 {
	if (!dead && !inactive) return color;

	auto hsv = Color::rgb2hsv(color);

	if (dead) hsv.s *= 0.75_f32;

	if (dead) hsv.v *= 0.50_f32;
	if (inactive) hsv.v *= 0.25_f32;

	return Color::hsv2rgb(hsv);
}

constexpr u32 bodyPartColors[] = {
	0xf2960c, /* Mouth */
	0x11f0e8, /* Mover */
	0x086603, /* Photosynthesizer */
	0xe31045, /* Weapon */
	0x4e2ba6, /* Armor */
	0x80a9d1, /* Eye */
	0xede6da, /* Scaffolding */
};

constexpr u32 upgradeColors[][3] = {
	{},
	{},
	{},
	{ /* Weapon */
		0x820c29,
		0xa24f64,
		0xe9b2c0,
	},
	{ /* Armor */
		0x220b5d,
		0x5a23e7,
		0xc8bde6,
	},
	{},
	{ /* Scaffolding */
		0xf0c47a,
	}
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

auto Renderer::render(const Environment & environment, const std::vector<Organism> & organisms, const Controls & controls) -> std::vector<u8> {
	auto buffer = std::vector<u8>(environment.mapSize() * BYTES_PER_TILE, 0);

	auto bufferIndex = [&](i32 x, i32 y) {
		return (y * environment.getWidth() + x) * BYTES_PER_TILE;
	};

	for (auto y = 0; y < environment.getHeight(); ++y) {
		for (auto x = 0; x < environment.getWidth(); ++x) {
			auto && mapCell = environment.accessUnsafe(x, y);
			auto && food = mapCell.food;

			insert3(
				buffer,
				bufferIndex(x, y) + 3,
				controls.activeNode == nullptr ? getFactorsColor(mapCell) : 0x000000
			);

			if (food.filled()) {
				buffer[bufferIndex(x, y)] = META_FOOD | (food.broken() ? CIRCLE_FLAG : 0);
				insert3(
					buffer,
					bufferIndex(x, y) + (food.broken() ? 6 : 3),
					modifyColor(food.dead(), controls.activeNode != nullptr,  bodyPartColors[food.bodyPart() - 1])
				);
				if (food.isModified()) {
					insert3(
						buffer,
						bufferIndex(x, y) + 6,
						modifyColor(food.dead(), controls.activeNode != nullptr, upgradeColors[food.bodyPart() - 1][food.modifier()])
					);
				}
			}
		}
	}

    for (auto index = 0; index < organisms.size(); ++index) {
        auto && organism = organisms[index];
		for (auto && cell : organism.body().getCells()) {
			auto [x, y] = organism.absoluteXY(cell);

			auto active = !controls.doHighlight || controls.activeNode == nullptr || organism.node->active;

			buffer[bufferIndex(x, y)] = META_ORGANISM | (cell.isModified() ? CIRCLE_FLAG : 0);
			insert2(
				buffer,
				bufferIndex(x, y) + 1,
				index
			);
			insert3(
				buffer,
				bufferIndex(x, y) + 3,
				modifyColor(cell.dead(), !active,  bodyPartColors[cell.bodyPart() - 1])
			);
			if (cell.isModified()) {
				insert3(
					buffer,
					bufferIndex(x, y) + 6,
					modifyColor(cell.dead(), !active, upgradeColors[cell.bodyPart() - 1][cell.modifier()])
				);
			}
		}
	}

	return buffer;
}
