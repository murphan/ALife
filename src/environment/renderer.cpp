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
	0xede6da, /* Scaffolding */
};

constexpr u32 bodyPartDeadColors[] = {
	0x705d41, /* Mouth */
	0x316e6b, /* Mover */
	0x425945, /* Photosynthesizer */
	0x54333c, /* Weapon */
	0x423b54, /* Armor */
	0x4a5559, /* Eye */
	0x63625f, /* Scaffolding */
};

constexpr u32 weaponUpgradeColors[] = {
	0x820c29,
	0xa24f64,
	0xe9b2c0,
};

constexpr u32 armorUpgradeColors[] = {
	0x220b5d,
	0x5a23e7,
	0xc8bde6,
};

constexpr u32 scaffoldingUpgradeColor = 0xf0c47a;

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
				getFactorsColor(mapCell)
			);

			if (food.filled()) {
				buffer[bufferIndex(x, y)] = META_FOOD | (food.broken() ? CIRCLE_FLAG : 0);
				insert3(
					buffer,
					bufferIndex(x, y) + (food.broken() ? 6 : 3),
					food.dead() ? bodyPartDeadColors[food.bodyPart() - 1] : bodyPartColors[food.bodyPart() - 1]
				);
			}
		}
	}

	for (auto && organism : organisms) {
		auto && body = organism.body();
		auto rotation = organism.rotation;

		for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
			for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
				auto && cell = body.access(i, j, rotation);

				auto x = organism.x + i;
				auto y = organism.y + j;

				if (cell.filled()) {
					buffer[bufferIndex(x, y)] = META_ORGANISM | (cell.isModified() ? CIRCLE_FLAG : 0);
					insert2(
						buffer,
						bufferIndex(x, y) + 1,
						organism.id
					);
					insert3(
						buffer,
						bufferIndex(x, y) + 3,
						cell.dead() ? bodyPartDeadColors[cell.bodyPart() - 1] : bodyPartColors[cell.bodyPart() - 1]
					);
					if (cell.isModified()) {
						insert3(
							buffer,
							bufferIndex(x, y) + 6,
							cell.bodyPart() == BodyPart::WEAPON ? weaponUpgradeColors[cell.modifier()] :
							cell.bodyPart() == BodyPart::ARMOR ? armorUpgradeColors[cell.modifier()] :
							scaffoldingUpgradeColor
						);
					}
				}
			}
		}
	}

	return buffer;
}
