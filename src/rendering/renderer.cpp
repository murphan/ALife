//
// Created by Emmet on 4/11/2023.
//

module;

#include <array>
#include <vector>

export module Renderer;

import Types;
import Environment;
import Organism;
import Color;
import MapCell;
import Factor;
import Controls;
import Direction;
import Body;

namespace Renderer {
	constexpr static i32 BYTES_PER_TILE = 9;

	constexpr static f32 BACKGROUND_DIM = 0.3333333333_f32;

	inline auto getFactorsColor(const MapCell & mapCell) -> u32 {
		auto value = ((f32) mapCell.getFactor(Factor::LIGHT) / 127.0_f32) * BACKGROUND_DIM;

		return Color::channelFloats2Int(value, value, value);
	}

	template<usize SIZE>
	constexpr auto
	colorShiftArray(const u32 (&colorArray)[SIZE], f32 saturationFactor, f32 valueFactor) -> std::array<u32, SIZE> {
		auto array = std::array<u32, SIZE>();

		for (auto i = 0; i < SIZE; ++i) {
			auto hsv = Color::rgb2hsv(colorArray[i]);

			hsv.s *= saturationFactor;
			hsv.v *= valueFactor;

			array[i] = Color::hsv2rgb(hsv);
		}

		return array;
	}

	constexpr auto modifyColor(bool dead, bool inactive, u32 color) -> u32 {
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

	inline auto insert3(std::vector<u8> &buffer, i32 index, u32 value) -> void {
		buffer[index] = (value >> 16) & 0xff;
		buffer[index + 1] = (value >> 8) & 0xff;
		buffer[index + 2] = value & 0xff;
	}

	inline auto insert2(std::vector<u8> &buffer, i32 index, u32 value) -> void {
		buffer[index] = (value >> 8) & 0xff;
		buffer[index + 1] = value & 0xff;
	}

	constexpr static u8 META_EMPTY = 0;
	constexpr static u8 META_ORGANISM = 1;
	constexpr static u8 META_FOOD = 2;
	constexpr static u8 META_WALL = 3;

	constexpr static u8 CIRCLE_FLAG = 1 << 7;

	template<typename T>
	concept EnvironmentLike = requires(const T t, i32 x, i32 y) {
		{ t.accessUnsafe(x, y) } -> std::same_as<const MapCell &>;
		{ t.getWidth() } -> std::same_as<i32>;
		{ t.getHeight() } -> std::same_as<i32>;
	};

	export class Render {
	private:
		std::vector<u8> buffer;
		bool isHighlighting;
		i32 width;

		[[nodiscard]] auto bufferIndex(i32 x, i32 y) const -> i32 {
			return (y * width + x) * BYTES_PER_TILE;
		};

	public:
		template<EnvironmentLike E>
		explicit Render(const E &environment, const Controls & controls) :
			buffer(environment.getWidth() * environment.getHeight() * BYTES_PER_TILE, 0),
			isHighlighting(controls.doHighlight && controls.activeNode != nullptr),
			width(environment.getWidth()) {
			for (auto y = 0; y < environment.getHeight(); ++y) {
				for (auto x = 0; x < environment.getWidth(); ++x) {
					auto &&mapCell = environment.accessUnsafe(x, y);
					auto &&food = mapCell.food;

					insert3(
						buffer,
						bufferIndex(x, y) + 3,
						isHighlighting ? 0x000000 : getFactorsColor(mapCell)
					);

					if (food.filled()) {
						buffer[bufferIndex(x, y)] = META_FOOD | (food.broken() ? CIRCLE_FLAG : 0);
						insert3(
							buffer,
							bufferIndex(x, y) + (food.broken() ? 6 : 3),
							modifyColor(food.dead(), isHighlighting, bodyPartColors[food.bodyPart() - 1])
						);
						if (food.isModified()) {
							insert3(
								buffer,
								bufferIndex(x, y) + 6,
								modifyColor(food.dead(), isHighlighting,
								            upgradeColors[food.bodyPart() - 1][food.modifier()])
							);
						}
					}
				}
			}
		}

		auto
		renderOrganism(const Organism &organism, i32 centerX, i32 centerY, Direction direction, i32 index) -> void {
			for (auto &&cell: organism.body().getCells()) {
				auto [x, y] = Body::absoluteXY(cell, centerX, centerY, direction);

				auto active = !isHighlighting || organism.node->active;

				buffer[bufferIndex(x, y)] = META_ORGANISM | (cell.isModified() ? CIRCLE_FLAG : 0);
				insert2(
					buffer,
					bufferIndex(x, y) + 1,
					index
				);
				insert3(
					buffer,
					bufferIndex(x, y) + 3,
					modifyColor(cell.dead(), !active, bodyPartColors[cell.bodyPart() - 1])
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

		auto finalize() -> std::vector<u8> && {
			return std::move(buffer);
		}
	};
}
